#include "util.h"
#include <inttypes.h>

int
cli_baud(struct carbon_ctx * ctx, struct options * options)
{
    const char * arg_baud = optparse_arg(&options->optparse);

    if (NULL == arg_baud)
    {
        printf("Missing baud rate\n");
        return -1;
    }

    enum carbon_baud baud;
    if (baud_str(arg_baud, &baud) < 0)
    {
        printf("Invalid baud rate: %s\n", arg_baud);
        return -1;
    }

    int status = carbon_baud(ctx, baud);
    if (status != carbon_status_OK)
    {
        printf("%s\n", str_carbon_status(status));
        return -1;
    }

    return 0;
}
