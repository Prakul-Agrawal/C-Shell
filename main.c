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
        char input[4096], temp[4096];
        fgets(input, 4096, stdin);
        strcpy(temp, input);
        char* cmd = strtok(temp, " \t\n");
        if (strcmp(cmd, "exit") == 0)
        {
            exit(EXIT_SUCCESS);
        }
        if (strcmp(cmd, "warp") == 0)
        {
            warp(input);
        }
        else
        {
            printf("Command not found\n");
        }
    }
}
