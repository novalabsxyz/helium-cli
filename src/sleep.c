#include "util.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int
cli_sleep(struct carbon_ctx * ctx, struct options * options)
{
    const char * outfile =
        options->filename ? options->filename : "connection.dat";

    struct connection connection;
    int               status = carbon_sleep(ctx, &connection);

    if (status != carbon_status_OK)
    {
        printf("%s\n", str_carbon_status(status));
        return -1;
    }

    int fd = open(outfile, O_CREAT | O_WRONLY);
    if (fd == -1)
    {
        printf("Error opening output file: %s\n", strerror(errno));
        return -1;
    }

    if (write(fd, &connection, sizeof(connection)) != sizeof(connection))
    {
        printf("Error writing connection data: %s\n", strerror(errno));
        return -1;
    }

    close(fd);
    return 0;
}
