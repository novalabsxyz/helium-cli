#include "util.h"

int
cli_channel_create(struct helium_ctx * ctx, struct options * options)
{
    const char * arg_name = optparse_arg(&options->optparse);
    if (NULL == arg_name)
    {
        printf("Channel create requires a name\n");
        return -1;
    }

    int8_t channel_id;
    int    status =
        helium_channel_create(ctx, arg_name, strlen(arg_name), &channel_id);

    if (status != helium_status_OK)
    {
        printf("Error creating channel: %s\n", str_helium_status(status));
        return -1;
    }
    else if (channel_id < 0)
    {
        printf("Error creating channel: %d\n", channel_id);
        return -1;
    }

    printf("Channel created: %d\n", channel_id);
    return 0;
}

int
cli_channel_send(struct helium_ctx * ctx, struct options * options)
{
    const char * arg_channel = optparse_arg(&options->optparse);
    const char * arg_str     = optparse_arg(&options->optparse);
    uint8_t      data[HELIUM_MAX_DATA_SIZE];
    int          data_len = 0;
    int          channel_id;

    if (NULL == arg_channel)
    {
        printf("Missing channel id\n");
        return -1;
    }
    else if (sscanf(arg_channel, "%d", &channel_id) != 1 || channel_id > 255)
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

    int8_t send_result;
    int    status =
        helium_channel_send(ctx, channel_id, data, data_len, &send_result);

    if (status != helium_status_OK)
    {
        printf("Error sending on channel: %s\n", str_helium_status(status));
        return -1;
    }
    else if (send_result != 0)
    {
        printf("Error sending on channel: %d\n", send_result);
        return -1;
    }

    return 0;
}

static struct cli_command commands[] = {
    {.name = "create", .func = cli_channel_create},
    {.name = "send", .func = cli_channel_send},
    {0, 0},
};

int
cli_channel(struct helium_ctx * ctx, struct options * options)
{
    const char * arg_cmd = optparse_arg(&options->optparse);
    cli_func     func    = cli_find(arg_cmd, commands);
    if (NULL == func)
    {
        printf("No such command: %s\n", arg_cmd);
        return -1;
    }

    return func(ctx, options);
}
