#include "util.h"
#include <inttypes.h>

int
cli_info(struct helium_ctx * ctx, struct options * options)
{
    (void)options;

    struct helium_info info;
    int                status = helium_info(ctx, &info);

    if (status != helium_status_OK)
    {
        printf("%s\n", str_helium_status(status));
        return -1;
    }

    printf("MAC:     %" PRIx64 "\n", info.mac);
    printf("Uptime:  %d\n", info.uptime);
    printf("Time:    %d\n", info.time);
    printf("Version: %x\n", info.fw_version);
    printf("Radios:  %d\n", info.radio_count);
    return 0;
}
