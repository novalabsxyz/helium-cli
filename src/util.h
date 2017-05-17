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
};

static inline void
options_init(struct options * options, char ** argv)
{
    optparse_init(&options->optparse, argv);
    options->port     = DEFAULT_PORT;
    options->filename = 0;
}


typedef int (*cli_func)(struct carbon_ctx * ctx, struct options * options);

struct cli_command
{
    const char * name;
    cli_func     func;
};

cli_func
cli_find(const char * needle, struct cli_command * commands);

const char *
str_carbon_status(enum carbon_status status);
int
load_file(const char * filename, uint8_t * data, size_t len);
int
save_file(const char * filename, uint8_t * data, size_t len);

#define ERR_NEXIT(e)                                                           \
    {                                                                          \
        result = e;                                                            \
        goto exit;                                                             \
    }

#define ERR_EXIT(e, fmt, ...)                                                  \
    {                                                                          \
        result = e;                                                            \
        printf(fmt, __VA_ARGS__);                                              \
        goto exit;                                                             \
    }

#endif // CARBON_CLI_UTILS_H
