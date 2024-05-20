#include "pastevents.h"

void pastevents(char *input) {
    const int cmd_len = 10;
    char arguments[MAX_COMMAND_LEN + 1];
    size_t input_len = strlen(input);
    strncpy(arguments, input + cmd_len, input_len);

    char **args = split_string(arguments, " \t\n");
    size_t arg_count = get_segment_count(arguments, " \t\n");

    if (arg_count > 2) {
        printf("Invalid number of arguments for pastevents\n");
        rep(i, 0, arg_count) {
            free(args[i]);
        }
        free(args);
        return;
    }

    if (arg_count == 0) {
        int history_count = get_history_count();        
        char (*history)[MAX_COMMAND_LEN] = get_history();
        rep(i, 0, history_count) {
            printf("%s", history[i]);
        }        
    }
    else if (arg_count == 1) {
        if (strcmp(args[0], "purge") == 0) {
            purge_history();
        }
        else {
            printf("Invalid argument %s\n", args[0]);
        }
    }
    else {
        if (strcmp(args[0], "execute") == 0) {
            if (!is_num(args[1])) {
                printf("Invalid argument %s\n", args[1]);
                rep(i, 0, arg_count) {
                    free(args[i]);
                }
                free(args);
                return;
            }
            else {
                int index = str_to_num(args[1]);
                int history_count = get_history_count();
                char (*history)[MAX_COMMAND_LEN] = get_history();
                if (index <= 0 || index > history_count) {
                    printf("Invalid index %d\n", index);
                    rep(i, 0, arg_count) {
                        free(args[i]);
                    }
                    free(args);
                    return;
                }
                else {
                    execute(history[history_count - index]);
                }
            }
        }
        else {
            printf("Invalid argument %s\n", args[0]);
        }
    }

    rep(i, 0, arg_count) {
        free(args[i]);
    }
    free(args);
}