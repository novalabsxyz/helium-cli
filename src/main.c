#include "util.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <poll.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

bool
carbon_serial_readable(void * param)
{
    struct pollfd pollfd = {
        .fd = (int)(intptr_t)param, .events = POLLIN,
    };
    if (poll(&pollfd, 1, -1) == 1)
    {
        return pollfd.revents & POLLIN;
    }
    return false;
}

bool
carbon_serial_getc(void * param, uint8_t * ch)
{
    return read((int)(intptr_t)param, ch, 1) > 0;
}

bool
carbon_serial_putc(void * param, uint8_t ch)
{
    return write((int)(intptr_t)param, &ch, 1) == 1;
}

void
carbon_wait_us(void * param, uint32_t us)
{
    (void)param;
    usleep(us);
}


int
set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0)
    {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD); /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;      /* 8-bit characters */
    tty.c_cflag &= ~PARENB;  /* no parity bit */
    tty.c_cflag &= ~CSTOPB;  /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS; /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &=
        ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int
open_serial_port(const char * portname, enum carbon_baud baud)
{
    int fd;
    fd = open(portname, O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (fd < 0)
    {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return -1;
    }

    if (ioctl(fd, TIOCEXCL) < 0)
    {
        printf("Error setting TIOCEXCL on %s - %s(%d).\n",
               portname,
               strerror(errno),
               errno);
        return -1;
    }

    if (fcntl(fd, F_SETFL, 0) < 0)
    {
        printf("Error clearing O_NONBLOCK %s - %s(%d).\n",
               portname,
               strerror(errno),
               errno);
        return -1;
    }

    speed_t baud_rate = B9600;
    switch (baud)
    {
    case carbon_baud_b9600:
        baud_rate = B9600;
        break;
    case carbon_baud_b14400:
        // B14400 does not exist on linux, default to a higher speed
    case carbon_baud_b19200:
        baud_rate = B19200;
        break;
    case carbon_baud_b38400:
        baud_rate = B38400;
        break;
    case carbon_baud_b57600:
        baud_rate = B57600;
        break;
    case carbon_baud_b115200:
        baud_rate = B115200;
        break;
    }

    /* Set baud rate,  8 bits, no parity, 1 stop bit */
    set_interface_attribs(fd, baud_rate);
    return fd;
}

static void
usage(const char * app)
{
    fprintf(
        stderr,
        "%s\n"
        "\n"
        "Usage: %s [OPTIONS] [cmd [arg [arg ...]]]\n"
        "\nOptions:\n"
        "  -h/--help      This help message.\n"
        "  -p/--port      The (USB) device to talk to.\n"
        "  -b/--baud      The baud rate to use on the port (default 9600).\n"
        "  -f/--file      The file to use for connect/sleep data or channel "
        "send.\n"

        "\nCommands:\n"
        "  connect                   Get the atom to connect to the network\n"
        "                            Use '-f' to specify quick connect data.\n"
        "  connected                 Check if the atom is connected.\n"
        "                            Returns 0 if connected.\n"
        "  sleep                     Disconnect from the network, saving\n"
        "                            quick connect data.\n"
        "                            Use '-f' to specify quick connect\n"
        "                            filename (default 'connection.dat').\n"
        "  info                      Print information of the atom.\n"

        "  channel create <name>     Create a channel with a given <name>.\n"
        "                            Prints out the id of the created\n"
        "                            channel.\n"
        "  channel send <id> [text]  Sends the given [text] to the given\n"
        "                            channel <id>. If [text] is not specified\n"
        "                            '-f' is checked for a filename to send.\n",
        app,
        app);
}

int(cli_connect)(struct carbon_ctx * ctx, struct options * options);
int(cli_connected)(struct carbon_ctx * ctx, struct options * options);
int(cli_sleep)(struct carbon_ctx * ctx, struct options * options);
int(cli_info)(struct carbon_ctx * ctx, struct options * options);
int(cli_baud)(struct carbon_ctx * ctx, struct options * options);
int(cli_channel)(struct carbon_ctx * ctx, struct options * options);

static struct cli_command commands[] = {
    {.name = "baud", .func = cli_baud},
    {.name = "connect", .func = cli_connect},
    {.name = "connected", .func = cli_connected},
    {.name = "sleep", .func = cli_sleep},
    {.name = "info", .func = cli_info},
    {.name = "channel", .func = cli_channel},
    {0, 0},
};

int
main(int argc, char ** argv)
{
    (void)argc;
    int fd     = -1;
    int result = 0;

    int                  opt, longindex;
    struct options       options;
    struct optparse_long longopts[] = {{"help", 'h', OPTPARSE_NONE},
                                       {"port", 'p', OPTPARSE_REQUIRED},
                                       {"baud", 'b', OPTPARSE_REQUIRED},
                                       {"file", 'f', OPTPARSE_REQUIRED},
                                       {"channel", 'c', OPTPARSE_REQUIRED},
                                       {0, 0, 0}};

    options_init(&options, argv);
    while ((opt = optparse_long(&options.optparse, longopts, &longindex)) != -1)
    {
        switch (opt)
        {
        case 'h':
            usage(argv[0]);
            ERR_NEXIT(0);
            break;
        case 'p':
            options.port = options.optparse.optarg;
            break;
        case 'b':
            if (baud_str(options.optparse.optarg, &options.baud) < 0)
            {
                ERR_EXIT(-1, "Invalid baud rate: %s\n", options.optparse.optarg);
            }
            break;
        case 'f':
            options.filename = options.optparse.optarg;
            break;
        case '?':
            printf("%s\n", options.optparse.errmsg);
            ERR_NEXIT(1);
            break;
        }
    }

    char * arg_cmd = optparse_arg(&options.optparse);
    if (arg_cmd == NULL)
    {
        ERR_EXIT(-1, "%s: Missing command\n", argv[0]);
    }

    ;
    if ((fd = open_serial_port(options.port, options.baud)) < 0)
    {
        ERR_NEXIT(-1);
    }

    struct carbon_ctx ctx;
    carbon_init(&ctx, (void *)(intptr_t)fd);

    cli_func func = cli_find(arg_cmd, commands);
    if (NULL == func)
    {
        ERR_EXIT(-1, "No such command: %s\n", arg_cmd);
    }

    result = func(&ctx, &options);

exit:
    if (fd >= 0)
    {
        close(fd);
    }
    return result;
}
