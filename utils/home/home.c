#include "home.h"

char *get_home_path() {
    // Asked ChatGPT how to find the path to the current executale

    char *home_path = SAFE_MALLOC((MAX_PATH_LEN + 1) * sizeof(char));

    ssize_t bytes = readlink("/proc/self/exe", home_path, MAX_PATH_LEN + 1);
    if (bytes >= 0) {
        home_path[bytes] = '\0';
    }
    else {
        die("readlink() failed");
    }

    size_t end = strlen(home_path) - 1;
    while (home_path[end] != '/') {
        end--;
    }
    home_path[end] = '\0';

    return home_path;
}