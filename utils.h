#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

// Convert a numeric string to long, error if invalid
long str_to_long(const char *str);
// Return MIME type based on file extension
const char *get_mime_type(const char *path);

#endif // UTILS_H