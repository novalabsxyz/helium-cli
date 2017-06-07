#include "util.h"

int
cli_connected(struct helium_ctx * ctx, struct options * options)
{
    (void)options;

    int status = helium_connected(ctx);

    if (status != helium_status_OK)
    {
        printf("%s\n", str_helium_status(status));
        return -1;
    }

    return 0;
}
