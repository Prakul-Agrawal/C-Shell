#include "main.h"

char home_path[MAX_PATH_LEN + 1], pwd[MAX_PATH_LEN + 1];
bool is_pwd_set = false;
int last_command_time = 0;
char last_command[MAX_COMMAND_LEN + 1];

int main()
{
    char *temp_home_path = get_cwd();
    strcpy(home_path, temp_home_path);
    free(temp_home_path);

    set_history_path();

    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sa.sa_sigaction = processUpdateHandler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        die("sigaction() failed");
    }
    signal(SIGTTOU, SIG_IGN);

    while (1)
    {
        prompt();
        char input[MAX_COMMAND_LEN];
        fgets(input, MAX_COMMAND_LEN, stdin);
        execute(input);    
        update_history(input);
        emptyExitedProcessArray();
    }
}
