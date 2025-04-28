#ifndef REQUEST_H
#define REQUEST_H

#include <stddef.h>
#include "constants.h"

// Structure for an HTTP request
typedef struct {
    char method[8];
    char path[256];
    char version[16];
    struct {
        char name[64];
        char value[256];
    } headers[MAX_HEADERS];
    size_t header_count;
} http_request_t;

// Parse a single request from client_fd; returns 0 on success, -1 on EOF/error
int parse_request(int client_fd, http_request_t *req);

#endif // REQUEST_H