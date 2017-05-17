#include "util.h"

int
cli_connected(struct carbon_ctx * ctx, struct options * options)
{
    (void)options;

    int status = carbon_connected(ctx);

    if (status != carbon_status_OK)
    {
        printf("%s\n", str_carbon_status(status));
        return -1;
    }

    return 0;
}
