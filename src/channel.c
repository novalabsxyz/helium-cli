#include "util.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int
cli_channel_create(struct carbon_ctx * ctx, struct options * options)
{
    const char * arg_name = optparse_arg(&options->optparse);
    if (NULL == arg_name)
    {
        printf("Channel create requires a name\n");
        return -1;
    }

    uint8_t channel_id;
    int     status = carbon_channel_create(ctx,
                                       options->channel,
                                       strlen(options->channel),
                                       &channel_id);

    if (status != carbon_status_OK)
    {
        printf("Error creating channel: %s\n", str_carbon_status(status));
        return -1;
    }

    printf("Channel created: %d\n", channel_id);
    return 0;
}

int
cli_channel_send(struct carbon_ctx * ctx, struct options * options)
{
    const char * arg_channel = optparse_arg(&options->optparse);
    const char * arg_str     = optparse_arg(&options->optparse);
    uint8_t      data[CARBON_MAX_DATA_SIZE];
    int          data_len = 0;
    int          channel_id;

    if (NULL == arg_channel)
    {
        printf("Missing channel id\n");
        return -1;
    }
    else if (sscanf(arg_channel, "%u", &channel_id) != 1 || channel_id > 255)
    {
        printf("Invalid channel id\n");
        return -1;
    }

    if (arg_str)
    {
        data_len = strlen(arg_str);
        memcpy(data, arg_str, data_len);
    }
    else if (options->filename)
    {
        data_len = load_file(options->filename, data, sizeof(data));
    }

    if (data_len < 0)
    {
        return data_len;
    }

    uint8_t send_result;
    int     status =
        carbon_channel_send(ctx, channel_id, data, data_len, &send_result);

    if (status != carbon_status_OK)
    {
        printf("Error sending on channel: %s\n", str_carbon_status(status));
        return -1;
    }

    return send_result;
}

static struct cli_command commands[] = {
    {.name = "create", .func = cli_channel_create},
    {.name = "send", .func = cli_channel_send},
    {0, 0},
};

int
cli_channel(struct carbon_ctx * ctx, struct options * options)
{
    const char * arg_cmd = optparse_arg(&options->optparse);
    for (struct cli_command * cmd = commands; cmd->name; cmd++)
    {
        if (strcmp(cmd->name, arg_cmd) == 0)
        {
            return cmd->func(ctx, options);
        }
    }

    printf("Invalid command: %s\n", arg_cmd);
    return -1;
}
