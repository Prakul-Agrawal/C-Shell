#include "common.h"

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void *safe_malloc(size_t size, unsigned long line) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "malloc() for %lu bytes failed at line %lu in file %s\n", size, line, __FILE__);
        exit(EXIT_FAILURE);
    }
    return ptr;
}