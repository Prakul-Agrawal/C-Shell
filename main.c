#include "headers.h"
#include "prompt/prompt.h"
#include "commands/warp/warp.h"

char home_path[MAX_PATH_LEN + 1], pwd[MAX_PATH_LEN + 1];
bool is_pwd_set = false;

int main()
{
    char *temp_home_path = get_cwd();
    strcpy(home_path, temp_home_path);
    free(temp_home_path);

    // Keep accepting commands
    while (1)
    {
        prompt();
        char input[MAX_COMMAND_LEN], temp[MAX_COMMAND_LEN];
        fgets(input, MAX_COMMAND_LEN, stdin);

        char** split_at_amp = split_string(input, "&");
        size_t split_at_amp_count = get_segment_count(input, "&");

        rep(i, 0, split_at_amp_count){
            char** commands = split_string(split_at_amp[i], ";");
            size_t commands_count = get_segment_count(split_at_amp[i], ";");

            rep(j, 0, commands_count){
                strcpy(temp, commands[j]);
                char* cmd = strtok(temp, " \t\n");

                if (strcmp(cmd, "exit") == 0)
                {
                    exit(EXIT_SUCCESS);
                }
                if (strcmp(cmd, "warp") == 0)
                {
                    warp(left_strip(commands[j], " \t"));
                }
                else
                {
                    printf("Command not found\n");
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
}
