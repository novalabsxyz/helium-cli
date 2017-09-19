#include "util.h"

static int
_parse_channel_id(const char * arg_channel, int * channel_id)
{
    if (NULL == arg_channel)
    {
        printf("Missing channel id\n");
        return -1;
    }
    else if (sscanf(arg_channel, "%d", channel_id) != 1 || *channel_id > 255)
    {
        printf("Invalid channel id\n");
        return -1;
    }

    return 0;
}

int
cli_channel_create(struct helium_ctx * ctx, struct options * options)
{
    const char * arg_name = optparse_arg(&options->optparse);
    if (NULL == arg_name)
    {
        printf("Channel create requires a name\n");
        return -1;
    }

    uint16_t token;
    int8_t   result;
    int status = helium_create_channel(ctx, arg_name, strlen(arg_name), &token);
    if (helium_status_OK == status)
    {
        status = helium_poll_result(ctx, token, &result, HELIUM_POLL_RETRIES_5S);
    }

    if (status != helium_status_OK)
    {
        printf("Error creating channel: %s\n", str_helium_status(status));
        return -1;
    }

    if (result < 0)
    {
        printf("Error creating channel: %d\n", result);
        return -1;
    }

    printf("Channel created: %d\n", result);
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

    if (_parse_channel_id(arg_channel, &channel_id) != 0)
    {
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

    struct helium_channel channel;
    helium_channel_init(&channel, ctx, channel_id);

    uint16_t token;
    int8_t   result;
    int      status = helium_channel_send(&channel, data, data_len, &token);
    if (helium_status_OK == status)
    {
        status = helium_poll_result(channel.helium,
                                    token,
                                    &result,
                                    HELIUM_POLL_RETRIES_5S);
    }

    if (status != helium_status_OK)
    {
        printf("Error sending on channel: %s\n", str_helium_status(status));
        return -1;
    }

    if (result != 0)
    {
        printf("Error sending on channel: %d\n", result);
        return -1;
    }

    return 0;
}

int
cli_channel_poll(struct helium_ctx * ctx, struct options * options)
{
    const char * arg_channel = optparse_arg(&options->optparse);
    int          channel_id;
    uint8_t      data[HELIUM_MAX_DATA_SIZE];
    size_t       used = 0;

    if (_parse_channel_id(arg_channel, &channel_id) != 0)
    {
        return -1;
    }

    struct helium_channel channel;
    helium_channel_init(&channel, ctx, channel_id);

    int status = helium_status_OK_NO_DATA;
    while (status == helium_status_OK || status == helium_status_OK_NO_DATA)
    {
        status = helium_channel_poll_data(&channel,
                                          data,
                                          HELIUM_MAX_DATA_SIZE,
                                          &used,
                                          HELIUM_POLL_RETRIES_5S);
        if (helium_status_OK == status)
        {
            printf("%.*s\n", (int)used, data);
        }
    }

    printf("Error polling channel: %s\n", str_helium_status(status));
    return -1;
}


int
cli_channel_ping(struct helium_ctx * ctx, struct options * options)
{
    const char * arg_channel = optparse_arg(&options->optparse);
    int          channel_id;

    if (_parse_channel_id(arg_channel, &channel_id) != 0)
    {
        return -1;
    }

    struct helium_channel channel;
    helium_channel_init(&channel, ctx, channel_id);

    uint16_t token;
    int8_t   result;
    int      status = helium_channel_ping(&channel, &token);
    if (helium_status_OK == status)
    {
        status = helium_poll_result(channel.helium,
                                    token,
                                    &result,
                                    HELIUM_POLL_RETRIES_5S);
    }

    if (status != helium_status_OK)
    {
        printf("Error pinging channel: %s\n", str_helium_status(status));
        return -1;
    }

    if (result != 0)
    {
        printf("Error pinging channel: %d\n", result);
        return -1;
    }

    return 0;
}


static bool
channel_config_get_handler(void *                  ctx,
                           const char *            key,
                           enum helium_config_type value_type,
                           void *                  value)
{
    (void)ctx;
    printf("%s: ", key);
    switch (value_type)
    {
    case helium_config_bool:
        printf("%s", *(bool *)value ? "true" : "false");
        break;
    case helium_config_i32:
        printf("%d", *(int32_t *)value);
        break;
    case helium_config_f32:
        printf("%f", *(float *)value);
        break;
    case helium_config_str:
        printf("%s", (char *)value);
        break;
    case helium_config_null:
        printf("null");
        break;
    }
    printf("\n");

    return true;
}

int
cli_channel_config_get(struct helium_ctx * ctx, struct options * options)
{
    const char * arg_channel    = optparse_arg(&options->optparse);
    const char * arg_config_key = optparse_arg(&options->optparse);
    int          channel_id;

    if (_parse_channel_id(arg_channel, &channel_id) != 0)
    {
        return -1;
    }

    if (NULL == arg_config_key)
    {
        printf("Missing config key\n");
        return -1;
    }

    struct helium_channel channel;
    helium_channel_init(&channel, ctx, channel_id);

    struct helium_config config;
    helium_config_init(&config, &channel);

    int8_t   result;
    uint16_t token;
    int      status = helium_config_get(&config, arg_config_key, &token);
    if (helium_status_OK == status)
    {
        status = helium_config_get_poll_result(&config,
                                               token,
                                               channel_config_get_handler,
                                               NULL,
                                               &result,
                                               HELIUM_POLL_RETRIES_5S);
    }

    if (helium_status_OK != status)
    {
        printf("Error requesting configuration: %s\n", str_helium_status(status));
        return -1;
    }

    if (result != 0)
    {
        printf("Error requesting configuration: %d\n", result);
        return -1;
    }

    return 0;
}


static int
_str_to_value(const char * str, enum helium_config_type * value_type, void * value)
{
    switch (*str)
    {
    case 't':
        *value_type    = helium_config_bool;
        *(bool *)value = true;
        return strcmp(str, "true");
    case 'f':
        *value_type     = helium_config_bool;
        *(float *)value = false;
        return strcmp(str, "false");
    case 'n':
        *value_type = helium_config_null;
        return strcmp(str, "null");
    case '"':
        *value_type = helium_config_str;
        memcpy(value, str + 1, strlen(str) - 2);
        ((char *)value)[strlen(str) - 2] = '\0';
        return str[strlen(str) - 1] == '"' ? 0 : -1;
    default:
    {
        char * endptr;
        if (*str == '-' || (*str >= '0' && *str <= '9'))
        {
            *value_type       = helium_config_i32;
            *(int32_t *)value = strtol(str, &endptr, 10);
            if (*endptr != '\0')
            {
                *value_type     = helium_config_f32;
                *(float *)value = strtof(str, &endptr);
            }
            if (*endptr != '\0')
            {
                return -1;
            }
        }
    }
    }
    return 0;
}

int
cli_channel_config_set(struct helium_ctx * ctx, struct options * options)
{
    const char * arg_channel    = optparse_arg(&options->optparse);
    const char * arg_config_key = optparse_arg(&options->optparse);
    const char * arg_config_val = optparse_arg(&options->optparse);
    int          channel_id;

    if (_parse_channel_id(arg_channel, &channel_id) != 0)
    {
        return -1;
    }

    if (NULL == arg_config_key || NULL == arg_config_val)
    {
        printf("Missing configuration key or value\n");
        return -1;
    }

    char                    value[100];
    enum helium_config_type value_type;
    if (_str_to_value(arg_config_val, &value_type, value) != 0)
    {
        printf("Invalid configuration value: %s\n", arg_config_val);
        return -1;
    }

    struct helium_channel channel;
    helium_channel_init(&channel, ctx, channel_id);

    struct helium_config config;
    helium_config_init(&config, &channel);

    uint16_t token;
    int      status = helium_config_set(&config,
                                   arg_config_key,
                                   value_type,
                                   (void *)value,
                                   &token);

    int8_t result;
    if (helium_status_OK == status)
    {
        status = helium_config_set_poll_result(&config,
                                               token,
                                               &result,
                                               HELIUM_POLL_RETRIES_5S);
    }

    if (status != helium_status_OK)
    {
        printf("Error setting configuration: %s\n", str_helium_status(status));
        return -1;
    }

    if (result != 0)
    {
        printf("Error setting configuration: %d\n", result);
        return -1;
    }

    return 0;
}


int
cli_channel_config_poll(struct helium_ctx * ctx, struct options * options)
{
    const char * arg_channel = optparse_arg(&options->optparse);
    int          channel_id;

    if (_parse_channel_id(arg_channel, &channel_id) != 0)
    {
        return -1;
    }

    struct helium_channel channel;
    helium_channel_init(&channel, ctx, channel_id);

    struct helium_config config;
    helium_config_init(&config, &channel);

    int  status = helium_status_OK_NO_DATA;
    bool invalidate;
    while (status == helium_status_OK || status == helium_status_OK_NO_DATA)
    {
        status = helium_config_poll_invalidate(&config,
                                               &invalidate,
                                               HELIUM_POLL_RETRIES_5S);
        if (helium_status_OK == status && invalidate)
        {
            printf("Configuration invalidated\n");
        }
    }

    printf("Error polling channel: %s\n", str_helium_status(status));
    return -1;
}




static struct cli_command channel_config_commands[] = {
    {.name = "get", .func = cli_channel_config_get},
    {.name = "set", .func = cli_channel_config_set},
    {.name = "poll", .func = cli_channel_config_poll},
    {0, 0},
};


int
cli_channel_config(struct helium_ctx * ctx, struct options * options)
{
    const char * arg_cmd = optparse_arg(&options->optparse);
    cli_func     func    = cli_find(arg_cmd, channel_config_commands);
    if (NULL == func)
    {
        printf("No such command: %s\n", arg_cmd);
        return -1;
    }

    return func(ctx, options);
}


static struct cli_command channel_commands[] = {
    {.name = "create", .func = cli_channel_create},
    {.name = "send", .func = cli_channel_send},
    {.name = "poll", .func = cli_channel_poll},
    {.name = "ping", .func = cli_channel_ping},
    {.name = "config", .func = cli_channel_config},
    {0, 0},
};

int
cli_channel(struct helium_ctx * ctx, struct options * options)
{
    const char * arg_cmd = optparse_arg(&options->optparse);
    cli_func     func    = cli_find(arg_cmd, channel_commands);
    if (NULL == func)
    {
        printf("No such command: %s\n", arg_cmd);
        return -1;
    }

    return func(ctx, options);
}
