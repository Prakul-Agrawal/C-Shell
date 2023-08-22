#ifndef MP1_HEADERS_H
#define MP1_HEADERS_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdbool.h>

#define rep(i, s, e) for (int i = s; i < e; i++)
#define min(a, b) (a <= b ? a : b)
#define max(a, b) (a >= b ? a : b)
#define MAX_COMMAND_LEN 2048

static void crash(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

#endif // MP1_HEADERS_H