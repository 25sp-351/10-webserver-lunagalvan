#include "constants.h"
#include "handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    const char *port = DEFAULT_PORT;
    int opt;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        if (opt == 'p') port = optarg;
    }

    struct addrinfo hints = {0}, *res;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    int yes = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);

    listen(sockfd, BACKLOG);
    printf("Server listening on port %s\n", port);

    while (1) {
        int client_fd = accept(sockfd, NULL, NULL);
        struct client_info *ci = malloc(sizeof(*ci));
        ci->client_fd = client_fd;
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, ci);
        pthread_detach(tid);
    }

    close(sockfd);
    return 0;
}