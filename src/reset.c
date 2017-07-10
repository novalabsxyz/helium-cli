#include "util.h"
#include <inttypes.h>

int
cli_reset(struct helium_ctx * ctx, struct options * options)
{
    (void)options;

    int status = helium_reset(ctx);

    if (status != helium_status_OK)
    {
        printf("%s\n", str_helium_status(status));
        return -1;
    }

    return 0;
}
