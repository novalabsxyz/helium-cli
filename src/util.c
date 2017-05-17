#include "util.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

const char *str_carbon_status(enum carbon_status status) {
    switch(status) {
    case carbon_status_OK:
        return NULL;
    case carbon_status_ERR_COMMUNICATION:
        return "Error communication with Atom";
    case carbon_status_ERR_NOT_CONNECTED:
        return "Atom not connected to the network";
    case carbon_status_ERR_TIMEOUT:
        return "Timeout while trying to connect to network";
    case carbon_status_ERR_DROPPED:
        return "Atom dropped packet";
    case carbon_status_ERR_KEEP_AWAKE:
        return "Atom needs to stay awake";
    case carbon_status_ERR_FAILED:
        return "Network request failed";
    case carbon_status_ERR_NOT_FOUND:
        return "Network resource not found";
    }
}

int load_file(const char *filename, uint8_t *data, size_t len) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("Error opening file: %s\n", strerror(errno));
        return -1;
    }

    struct stat stat;
    if (fstat(fd, &stat) == -1) {
        printf("Error getting file stats: %s\n", strerror(errno));
        return -1;
    }

    if ((size_t)stat.st_size > len) {
        printf("File size %d is too big to be sent (max: %zu)\n",
               (int)stat.st_size, len);
        return -1;
    }

    if (read(fd, data, stat.st_size) != stat.st_size) {
        printf("Error reading data: %s\n", strerror(errno));
        return -1;
    }
    close(fd);

    return stat.st_size;
}
