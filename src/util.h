#ifndef CARBON_CLI_UTILS_H
#define CARBON_CLI_UTILS_H

#define OPTPARSE_IMPLEMENTATION
#define OPTPARSE_API static inline
#include "carbon.h"
#include "optparse.h"

#define DEFAULT_PORT "/dev/tty.usbserial-DN01IQCD"

struct options
{
    struct optparse optparse;
    const char **   argv;
    const char *    port;
    const char *    filename;
    const char *    channel;
};

static inline void
options_init(struct options * options, char ** argv)
{
    optparse_init(&options->optparse, argv);
    options->port     = DEFAULT_PORT;
    options->filename = 0;
    options->channel  = "azure";
}

struct cli_command
{
    const char * name;
    int (*func)(struct carbon_ctx * ctx, struct options * options);
};

const char *
str_carbon_status(enum carbon_status status);
int
load_file(const char * filename, uint8_t * data, size_t len);

#endif // CARBON_CLI_UTILS_H
