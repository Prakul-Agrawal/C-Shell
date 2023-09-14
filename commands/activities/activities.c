#include "activities.h"

void activities(char *input) {
    const int cmd_len = 10;
    char arguments[MAX_COMMAND_LEN + 1];
    size_t input_len = strlen(input);
    strncpy(arguments, input + cmd_len, input_len);

    char **args = split_string(arguments, " \t\n");
    size_t arg_count = get_segment_count(arguments, " \t\n");

    if (arg_count != 0) {
        printf("Invalid arguments\n");
        rep(i,0,arg_count) {
            free(args[i]);
        }
        free(args);
    }

    int process_count = 0;
    struct ProcessInfo *processes = getSortedProcesses(&process_count);

    rep(i, 0, process_count) {
        char *state = processes[i].running ? "Running" : (processes[i].stopped ? "Stopped" : "Exited");
        printf("%d : %s - %s\n", processes[i].pid, processes[i].command, state);
    }

    free(processes);

    rep(i, 0, arg_count) {
        free(args[i]);
    }
    free(args);
}