#include <inttypes.h>
#include "util.h"

int cli_info(struct carbon_ctx *ctx, struct options *options) {
    (void)options;

    struct res_info info;
    int status = carbon_info(ctx, &info);

    if (status != carbon_status_OK) {
        printf("%s\n", str_carbon_status(status));
        return -1;
    }

    printf("MAC:     %" PRIx64 "\n", info.mac);
    printf("Uptime:  %d\n", info.uptime);
    printf("Time:    %d\n", info.time);
    printf("Version: %x\n", info.fw_version);
    printf("Radios:  %d\n", info.radio_count);
    return 0;
}
