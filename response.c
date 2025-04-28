#include "response.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int send_response(int client_fd, const http_response_t *res) {
    char header[BUF_SIZE];
    int len = snprintf(header, sizeof(header),
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        res->status_code, res->status_text,
        res->content_type,
        res->content_length);
    if (write(client_fd, header, len) < 0) return -1;
    if (res->body && res->content_length > 0) {
        if (write(client_fd, res->body, res->content_length) < 0) return -1;
    }
    return 0;
}