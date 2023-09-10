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
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>

#include "utils/common/common.h"
#include "utils/delimiter/delimiter.h"

#define rep(i, s, e) for (int i = s; i < e; i++)
#define min(a, b) (a <= b ? a : b)
#define max(a, b) (a >= b ? a : b)
#define MAX_COMMAND_LEN 4096
#define MAX_PATH_LEN 1024
#define MAX_HISTORY_SIZE 15

char home_path[MAX_PATH_LEN + 1], pwd[MAX_PATH_LEN + 1];
extern bool is_pwd_set;

#endif // MP1_HEADERS_H