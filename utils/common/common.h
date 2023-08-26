#ifndef MP1_COMMON_H
#define MP1_COMMON_H

#include "../../headers.h"

#define SAFE_MALLOC(size) safe_malloc(size, __LINE__)

void die(const char *msg);

void *safe_malloc(size_t, unsigned long);

void err(const char*);

#endif //MP1_COMMON_H