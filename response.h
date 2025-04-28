#ifndef RESPONSE_H
#define RESPONSE_H

#include <stddef.h>
#include "request.h"

// Structure for an HTTP response
typedef struct {
    int status_code;
    char status_text[64];
    char content_type[64];
    const void *body;
    size_t content_length;
} http_response_t;

// Send the response over client_fd; returns 0 on success
int send_response(int client_fd, const http_response_t *res);

#endif // RESPONSE_H