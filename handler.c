#include "constants.h"
#include "utils.h"
#include "request.h"
#include "response.h"
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <limits.h>    // for LONG_MIN
#include "handler.h"    // for struct client_info

void *handle_client(void *arg) {
    struct client_info *ci = arg;
    int fd = ci->client_fd;
    free(ci);

    http_request_t req;
    while (parse_request(fd, &req) == 0) {
        http_response_t res;

        // Only support GET
        if (strcmp(req.method, "GET") != 0) {
            res.status_code = 405;
            strcpy(res.status_text, "Method Not Allowed");
            res.body = "405 Method Not Allowed";
            res.content_length = strlen(res.body);
            strcpy(res.content_type, "text/plain");
            send_response(fd, &res);
            break;
        }

        // Route: /static/*
        if (strncmp(req.path, "/static/", 8) == 0) {
            char filepath[512];
            snprintf(filepath, sizeof(filepath), STATIC_DIR "/%s", req.path + 8);
            int file_fd = open(filepath, O_RDONLY);
            if (file_fd < 0) {
                res.status_code = 404; strcpy(res.status_text, "Not Found");
                res.body = "404 Not Found"; res.content_length = strlen(res.body);
                strcpy(res.content_type, "text/plain");
            } else {
                struct stat st;
                fstat(file_fd, &st);
                char *filebuf = malloc(st.st_size);
                read(file_fd, filebuf, st.st_size);
                close(file_fd);

                res.status_code = 200; strcpy(res.status_text, "OK");
                res.body = filebuf; res.content_length = st.st_size;
                strcpy(res.content_type, get_mime_type(filepath));
            }
        }
        // Route: /calc/{add|mul|div}/a/b
        else if (strncmp(req.path, "/calc/", 6) == 0) {
            char op[4], a_str[32], b_str[32];
            if (sscanf(req.path, "/calc/%3[^/]/%31[^/]/%31s", op, a_str, b_str) == 3) {
                long a = str_to_long(a_str);
                long b = str_to_long(b_str);
                if (a == LONG_MIN || b == LONG_MIN) {
                    res.status_code = 400; strcpy(res.status_text, "Bad Request");
                    res.body = "Invalid operands"; res.content_length = strlen(res.body);
                    strcpy(res.content_type, "text/plain");
                } else {
                    long result;
                    if      (strcmp(op, "add") == 0) result = a + b;
                    else if (strcmp(op, "mul") == 0) result = a * b;
                    else if (strcmp(op, "div") == 0) {
                        if (b == 0) { a = 0; b = 1; } // avoid div by zero
                        result = a / b;
                    } else result = 0;

                    char *body = malloc(64);
                    int n = snprintf(body, 64, "%ld", result);
                    res.status_code = 200; strcpy(res.status_text, "OK");
                    res.body = body; res.content_length = n;
                    strcpy(res.content_type, "text/plain");
                }
            } else {
                res.status_code = 400; strcpy(res.status_text, "Bad Request");
                res.body = "Bad calc request"; res.content_length = strlen(res.body);
                strcpy(res.content_type, "text/plain");
            }
        }
        // Route: /sleep/{sec} (pipelining test)
        else if (strncmp(req.path, "/sleep/", 7) == 0) {
            char secs_str[16];
            if (sscanf(req.path, "/sleep/%15s", secs_str) == 1) {
                long secs = str_to_long(secs_str);
                if (secs > 0) sleep((unsigned)secs);
                res.status_code = 200; strcpy(res.status_text, "OK");
                res.body = "Slept"; res.content_length = strlen(res.body);
                strcpy(res.content_type, "text/plain");
            }
        }
        else {
            res.status_code = 404; strcpy(res.status_text, "Not Found");
            res.body = "404 Not Found"; res.content_length = strlen(res.body);
            strcpy(res.content_type, "text/plain");
        }

        send_response(fd, &res);

        // Free any heap-allocated bodies
        if (strncmp(req.path, "/static/", 8) == 0) free((void *)res.body);
        if (strncmp(req.path, "/calc/",   6) == 0) free((void *)res.body);
    }

    close(fd);
    return NULL;
}