#include "prompt.h"

#define MAX_PATH_LEN 1024

const size_t max_len = MAX_COMMAND_LEN; 

char user_name[MAX_COMMAND_LEN + 1], system_name[MAX_COMMAND_LEN + 1];

void get_user_name() {
    struct passwd *pw = getpwuid(getuid());
    strcpy(user_name, pw->pw_name);
}

void get_system_name() {
    if (gethostname(system_name, max_len) == -1) {
        die("gethostname() failed");
    }
}

void prompt() {
    get_user_name();
    get_system_name();
    char *cwd = get_cwd();

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

    if(last_command_time <= 2)
        printf("<%s@%s:%s> ", user_name, system_name, cwd);
    else {
        printf("<%s@%s:%s %s : %ds> ", user_name, system_name, cwd, last_command, last_command_time);
        strcpy(last_command, "");
        last_command_time = 0;
    }
    
    free(cwd);  
}
