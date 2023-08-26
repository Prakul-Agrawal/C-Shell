#include "warp.h"

void warp(char* input) {
    const int cmd_len = 4;
    char arguments[MAX_COMMAND_LEN + 1];
    size_t input_len = strlen(input);
    strncpy(arguments, input + cmd_len, input_len);

    char **args = split_string(arguments, " \t\n");
    size_t arg_count = get_segment_count(arguments, " \t\n");

    if (arg_count == 0) {
        char *temp_pwd = get_cwd();

        if (chdir(home_path) == -1) {
            printf("%s\n", home_path);
            err("chdir() failed");
            free(temp_pwd);
        }
        else {
            strcpy(pwd, temp_pwd);
            is_pwd_set = true;

            free(temp_pwd);
        
            char *cwd = get_cwd();
            printf("%s\n", cwd);
            free(cwd);
        }
    }

    rep(i, 0, arg_count) {
        char *path = args[i];

        char *temp_pwd = get_cwd();

        if (strlen(path) == 1 && path[0] == '-'){
            if (is_pwd_set == false){
                printf("Previous directory is not set\n");
                free(temp_pwd);
                continue;
            }
            else {
                path = pwd;
            }
        }
        else if (path[0] == '~') {
            size_t home_path_len = strlen(home_path);
            char temp[MAX_PATH_LEN + 1];
            strcpy(temp, home_path);
            strcat(temp, path + 1);
            strcpy(path, temp);
        }

        if (chdir(path) == -1) {
            printf("%s\n", path);
            err("chdir() failed");
            free(temp_pwd);
            continue;
        }
        else {
            strcpy(pwd, temp_pwd);
            is_pwd_set = true;
        }

        free(temp_pwd);

        char *cwd = get_cwd();
        printf("%s\n", cwd);
        free(cwd);
    }

    rep(i, 0, arg_count) {
        free(args[i]);
    }
    free(args);
}