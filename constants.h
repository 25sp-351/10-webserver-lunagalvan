#ifndef CONSTANTS_H
#define CONSTANTS_H

// Default port number (string for getaddrinfo)
#define DEFAULT_PORT "80"
// Maximum pending connections
#define BACKLOG 10
// Buffer size for request parsing and response building
#define BUF_SIZE 8192
// Directory for static files
#define STATIC_DIR "static"
// Maximum number of headers per request
#define MAX_HEADERS 100

#endif // CONSTANTS_H