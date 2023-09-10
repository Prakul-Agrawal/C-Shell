#include "main.h"

char home_path[MAX_PATH_LEN + 1], pwd[MAX_PATH_LEN + 1];
bool is_pwd_set = false;

int main()
{
    char *temp_home_path = get_cwd();
    strcpy(home_path, temp_home_path);
    free(temp_home_path);

    set_history_path();

    // Keep accepting commands
    while (1)
    {
        prompt();
        char input[MAX_COMMAND_LEN];
        fgets(input, MAX_COMMAND_LEN, stdin);
        execute(input);    
        update_history(input);
    }
}
