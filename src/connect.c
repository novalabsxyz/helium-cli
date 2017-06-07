#include "util.h"

int
cli_connect(struct helium_ctx * ctx, struct options * options)
{
    struct connection connection;
    if (options->filename)
    {
        if (load_file(options->filename, (uint8_t *)&connection, sizeof(connection))
            != sizeof(connection))
        {
            printf("Invalid connection data\n");
            return -1;
        }
    }

    int status = helium_connect(ctx, options->filename ? &connection : NULL);

    if (status != helium_status_OK)
    {
        printf("%s\n", str_helium_status(status));
        return -1;
    }

    return 0;
}
