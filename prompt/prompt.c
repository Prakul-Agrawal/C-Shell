#include "prompt.h"

#define MAX_PATH_LEN 1024

const size_t max_len = MAX_COMMAND_LEN; 

char user_name[MAX_COMMAND_LEN + 1], system_name[MAX_COMMAND_LEN + 1], 
cwd[MAX_COMMAND_LEN + 1], home_path[MAX_PATH_LEN + 1];

void get_user_name() {
    struct passwd *pw = getpwuid(getuid());
    strcpy(user_name, pw->pw_name);
}

void get_system_name() {
    if (gethostname(system_name, max_len) == -1) {
        crash("gethostname() failed");
    }
}

void get_cwd() {
    if (getcwd(cwd, max_len) == NULL) {
        crash("getcwd() failed");
    }
}

void get_home_path() {
    // Asked ChatGPT how to find the path to the current executale
    ssize_t bytes = readlink("/proc/self/exe", home_path, MAX_PATH_LEN + 1);
    if (bytes >= 0) {
        home_path[bytes] = '\0';
    }
    else {
        crash("readlink() failed");
    }

    size_t end = strlen(home_path) - 1;
    while (home_path[end] != '/') {
        end--;
    }
    home_path[end] = '\0';
}

void prompt() {
    get_user_name();
    get_system_name();
    get_cwd();
    get_home_path();

    size_t home_path_len = strlen(home_path);

    bool flag;
    if (strlen(cwd) >= home_path_len) {
        flag = true;
        rep(i, 0, home_path_len) {
            if (cwd[i] != home_path[i]) {
                flag = false;
                break;
            }
        }
    }
    else {
        flag = false;
    }
    if (flag) {
        char temp[MAX_PATH_LEN + 1];
        temp[0] = '~';
        rep(i, home_path_len, strlen(cwd)) {
            temp[i - home_path_len + 1] = cwd[i];
        }
        temp[strlen(cwd) - home_path_len + 1] = '\0';
        strcpy(cwd, temp);
    }

    printf("<%s@%s:%s> ", user_name, system_name, cwd);  
}
