#include "cwd.h"

char *get_cwd() {
    char *cwd = SAFE_MALLOC((MAX_PATH_LEN + 1) * sizeof(char));
    if (getcwd(cwd, MAX_PATH_LEN) == NULL) {
        die("getcwd() failed");
    }
    return cwd;
}