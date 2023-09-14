#include "neonate.h"

void neonate(char *input) {
    const int cmd_len = 7;
    char arguments[MAX_COMMAND_LEN + 1];
    size_t input_len = strlen(input);
    strncpy(arguments, input + cmd_len, input_len);

    char **args = split_string(arguments, " \t\n");
    size_t arg_count = get_segment_count(arguments, " \t\n");

    if (arg_count != 2) {
        printf("Invalid number of arguments\n");
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }

    if (strcmp(args[0], "-n") != 0) {
        printf("Invalid option: %s\n", args[0]);
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }

    if (!is_num(args[1])) {
        printf("Invalid argument: %s\n", args[1]);
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }

    int time_arg = str_to_num(args[1]);

    pid_t pid = fork();
    if (pid == -1) {
        err("fork() failed");
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }
    else if (pid == 0) {
        while(1) {
            FILE *fp = fopen("/proc/loadavg", "r");

            if (!fp) {
                err("fopen() failed");
                rep(i, 0, arg_count) {
                    free(args[i]);
                }
                free(args);
                return;
            }

            char buffer[33], recent_pid[33];

            {
                int pt = 0;
                while (fscanf(fp, "%s", buffer) != EOF) {
                    if (pt == 4) strcpy(recent_pid, buffer);
                    pt++;
                }
            }

            fclose(fp);
            
            printf("%s\n", recent_pid);
            sleep(time_arg);
        }
    }
    else {
        enableRawMode();
        while(1) {
            char c = getchar();
            if (c == 'x') {
                kill(pid, SIGKILL);
                break;
            }
        }
        disableRawMode();
    }

    rep(i, 0, arg_count) {
        free(args[i]);
    }
    free(args);
}