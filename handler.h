#ifndef HANDLER_H
#define HANDLER_H

// pass this into handle_client()
struct client_info {
    int client_fd;
};

// Thread entry point for handling a client connection
void *handle_client(void *arg);

#endif // HANDLER_H