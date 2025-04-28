#include "request.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>    // for recv()

int parse_request(int client_fd, http_request_t *req) {
    char buffer[BUF_SIZE + 1];
    ssize_t received;
    size_t total = 0;

    // Read until CRLFCRLF or error
    while ((received = recv(client_fd, buffer + total, BUF_SIZE - total, 0)) > 0) {
        total += received;
        buffer[total] = '\0';
        if (strstr(buffer, "\r\n\r\n")) break;
    }
    if (received <= 0) return -1;

    // Tokenize lines
    char *line = strtok(buffer, "\r\n");
    if (!line) return -1;

    // Parse request line
    sscanf(line, "%7s %255s %15s", req->method, req->path, req->version);

    // Parse headers
    size_t i = 0;
    while ((line = strtok(NULL, "\r\n")) && *line && i < MAX_HEADERS) {
        sscanf(line, "%63[^:]: %255[^\"]", req->headers[i].name, req->headers[i].value);
        i++;
    }
    req->header_count = i;
    return 0;
}