#include "util.h"

int
cli_connect(struct carbon_ctx * ctx, struct options * options)
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

    int status = carbon_connect(ctx, options->filename ? &connection : NULL);

    if (status != carbon_status_OK)
    {
        printf("%s\n", str_carbon_status(status));
        return -1;
    }

    return 0;
}
