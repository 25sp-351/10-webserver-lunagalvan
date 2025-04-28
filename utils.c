#include "utils.h"
#include <errno.h>
#include <limits.h>
#include <string.h>

long str_to_long(const char *str) {
    errno = 0;
    char *endptr = NULL;
    long val = strtol(str, &endptr, 10);
    if (errno != 0 || *endptr != '\0') {
        return LONG_MIN;  // indicate error
    }
    return val;
}

const char *get_mime_type(const char *path) {
    const char *ext = strrchr(path, '.');
    if (!ext) return "application/octet-stream";
    if (strcmp(ext, ".html") == 0) return "text/html";
    if (strcmp(ext, ".css")  == 0) return "text/css";
    if (strcmp(ext, ".js")   == 0) return "application/javascript";
    if (strcmp(ext, ".png")  == 0) return "image/png";
    if (strcmp(ext, ".jpg")  == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".gif")  == 0) return "image/gif";
    return "application/octet-stream";
}