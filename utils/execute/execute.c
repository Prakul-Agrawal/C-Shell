#include "execute.h"

char *command_names[] = {"exit", "warp", "proclore", "pastevents"};
void (*command_functions[])(char*) = {exit_program, warp, proclore, pastevents};

void exit_program(char* input) {
    exit(EXIT_SUCCESS);
}

void execute(char *input) {
    char temp[MAX_COMMAND_LEN + 1];
    char** split_at_amp = split_string(input, "&");
    size_t split_at_amp_count = get_segment_count(input, "&");

    rep(i, 0, split_at_amp_count){
        char** commands = split_string(split_at_amp[i], ";");
        size_t commands_count = get_segment_count(split_at_amp[i], ";");

        rep(j, 0, commands_count){
            strcpy(temp, commands[j]);
            char* cmd = strtok(temp, " \t\n");

            rep(k, 0, sizeof(command_names)/sizeof(command_names[0])){
                if (strcmp(cmd, command_names[k]) == 0)
                {
                    command_functions[k](left_strip(commands[j], " \t"));
                }
            }
        }

        rep(j, 0, commands_count){
            free(commands[j]);
        }
        free(commands);
    }

    rep(i, 0, split_at_amp_count){
        free(split_at_amp[i]);
    }
    free(split_at_amp);
}