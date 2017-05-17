#include "util.h"

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

    return save_file(outfile, (uint8_t *)&connection, sizeof(connection));
}
