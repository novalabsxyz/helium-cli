#include "util.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>


cli_func
cli_find(const char * needle, struct cli_command * commands)
{
    for (struct cli_command * cmd = commands; cmd->name; cmd++)
    {
        if (strcmp(cmd->name, needle) == 0)
        {
            return cmd->func;
        }
    }
    return NULL;
}

int
baud_str(const char * str, enum helium_baud * baud)
{
    struct baud_tuple
    {
        const char *     name;
        enum helium_baud baud;
    } baud_map[] = {
        {"9600", helium_baud_b9600},
        // 14400 is not supported by default on linux
        // {"14400", helium_baud_b14400},
        {"19200", helium_baud_b19200},
        {"38400", helium_baud_b38400},
        {"57600", helium_baud_b57600},
        {"115200", helium_baud_b115200},
        {0, 0},
    };

    for (struct baud_tuple * tuple = baud_map; tuple->name; tuple++)
    {
        if (strcmp(str, tuple->name) == 0)
        {
            *baud = tuple->baud;
            return 0;
        }
    }
    return -1;
}


const char *
str_helium_status(enum helium_status status)
{
    switch (status)
    {
    case helium_status_OK:
        return NULL;
    case helium_status_OK_NO_DATA:
        return "Timeout waiting for data";
    case helium_status_ERR_COMMUNICATION:
        return "Error communication with Atom";
    case helium_status_ERR_NOT_CONNECTED:
        return "Atom not connected to the network";
    case helium_status_ERR_DROPPED:
        return "Atom dropped packet";
    case helium_status_ERR_KEEP_AWAKE:
        return "Atom needs to stay awake";
    }

    return NULL;
}

int
load_file(const char * filename, uint8_t * data, size_t len)
{
    int result;
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        ERR_EXIT(-1, "Error opening file: %s\n", strerror(errno));
    }

    struct stat stat;
    if (fstat(fd, &stat) == -1)
    {
        ERR_EXIT(-1, "Error getting file stats: %s\n", strerror(errno));
    }

    ssize_t file_size = (ssize_t)stat.st_size;
    if (file_size > (ssize_t)len)
    {
        ERR_EXIT(-1,
                 "File size %zu is too big to be sent (max: %zu)\n",
                 file_size,
                 len);
    }

    if (read(fd, data, file_size) != file_size)
    {
        ERR_EXIT(-1, "Error reading data: %s\n", strerror(errno));
    }

    result = (int)file_size;

exit:
    if (fd >= 0)
    {
        close(fd);
    }
    return result;
}

int
save_file(const char * filename, uint8_t * data, size_t len)
{
    int fd =
        open(filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    int result = 0;

    if (fd == -1)
    {
        ERR_EXIT(-1, "Error opening output file: %s\n", strerror(errno));
    }

    if (write(fd, data, len) != (ssize_t)len)
    {
        ERR_EXIT(-1, "Error writing connection data: %s\n", strerror(errno));
    }

exit:
    if (fd >= 0)
    {
        close(fd);
    }
    return result;
}
