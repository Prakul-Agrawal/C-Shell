#include "execute.h"

char *command_names[] = {"exit", "warp", "proclore", "pastevents", "peek", "iMan"};
void (*command_functions[])(char*) = {exit_program, warp, proclore, pastevents, peek, iMan};

void exit_program(char* input) {
    exit(EXIT_SUCCESS);
}

void run_cmd(char* cmd, bool isBackground) {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    char temp[MAX_COMMAND_LEN + 1];
    strcpy(temp, cmd);
    char* cmd_name = strtok(temp, " \t\n");
    bool isCustom = false;

    rep(i, 0, sizeof(command_names)/sizeof(command_names[0])){
        if (strcmp(cmd_name, command_names[i]) == 0)
        {
            isCustom = true;
            command_functions[i](left_strip(cmd, " \t"));
            clock_gettime(CLOCK_MONOTONIC, &end_time);
            last_command_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
            strcpy(last_command, cmd_name);
        }
    }

    if (!isCustom) {
        char** args = split_string(cmd, " \t\n");
        size_t arg_count = get_segment_count(cmd, " \t\n");

        pid_t pid = fork();
        if (pid == -1) {
            err("fork() failed");
        }
        else if (pid == 0) {
            if (setpgid(0, 0) == -1) {
                die("setpgid() failed");
            }

            char** temp_args = realloc(args, sizeof(char*) * (arg_count + 1));
            if (temp_args == NULL) {
                die("realloc() failed");
            }
            args = temp_args;
            args[arg_count] = NULL;

            if (execvp(args[0], args) == -1) {
                die("execvp() failed");
            }
        }
        else {
            if (!isBackground) {
                waitpid(pid, NULL, 0);
                clock_gettime(CLOCK_MONOTONIC, &end_time);
                last_command_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
                strcpy(last_command, args[0]);
            }
            else {
                printf("%d\n", pid);
            }
        }

        rep(i, 0, arg_count){
            free(args[i]);
        }
        free(args);
    }
}

bool is_empty(char *s) {
    while (*s != '\0') {
        if (!isspace(*s)) {
            return false;
        }
        s++;
    }
    return true;
}

void execute(char *input) {
    char temp[MAX_COMMAND_LEN + 1] = "";
    char *inputPtr = input;

    while(*inputPtr != '\0') {
        if (*inputPtr == '&') {
            if (is_empty(temp)) {
                printf("Invalid empty command\n");
            }
            else {
                run_cmd(temp, true);
            }
            strcpy(temp, "");
        }
        else if (*inputPtr == ';') {
            if (is_empty(temp)) {
                printf("Invalid empty command\n");
            }
            else {
                run_cmd(temp, false);
            }
            strcpy(temp, "");
        }
        else if (*inputPtr == '\n') {
            if (!is_empty(temp)) {
                run_cmd(temp, false);
            }
            strcpy(temp, "");
        }
        else {
            strncat(temp, inputPtr, 1);
        }
        inputPtr++;
    }
}