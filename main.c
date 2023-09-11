#include "main.h"

char home_path[MAX_PATH_LEN + 1], pwd[MAX_PATH_LEN + 1];
bool is_pwd_set = false;
int last_command_time = 0;
char last_command[MAX_COMMAND_LEN + 1];

// void bg_process_handler()
// {
//     int status = 0;
//     pid_t pid = -1;

//     printf("bg_process_handler called\n");

//     while ((pid = waitpid(-1, &status, WUNTRACED | WNOHANG)) > 0)
//     {
//         const char *p_command = get_process_command_by_pid(pid);
//         const char *p_exit_status;

//         if (p_command == NULL)
//         {
//             continue;
//         }

//         last_command_pid = pid;

//         if (WIFSIGNALED(status))
//         {
//             p_exit_status = "was killed";
//             last_command_status = WTERMSIG(status);
//         }
//         else if (WIFEXITED(status))
//         {
//             p_exit_status = "exited normally";
//             last_command_status = WEXITSTATUS(status);
//         }
//         else if (WIFSTOPPED(status))
//         {
//             p_exit_status = "suspended normally";
//             last_command_status = WSTOPSIG(status);
//         }
//         else
//         {
//             p_exit_status = "exited abnormally";
//             last_command_status = 0;
//         }

//         printf("\n%s with pid = %d %s\n", p_command, pid, p_exit_status);
//         set_process_status(pid, WIFSTOPPED(status) ? 1 : 0);
//         if (!WIFSTOPPED(status))
//         {
//             remove_process(pid);
//             number_of_bg_process_running--;
//         }
//     }
// }

int main()
{
    char *temp_home_path = get_cwd();
    strcpy(home_path, temp_home_path);
    free(temp_home_path);

    set_history_path();

    // struct sigaction st;
    // st.sa_sigaction = bg_process_handler;

    // //* restart syscalls if possible, man 2 sigaction flags section
    // st.sa_flags = SA_RESTART;

    // //* do not block signals of any other type
    // sigemptyset(&st.sa_mask);
    // if (sigaction(SIGCHLD, &st, NULL) == -1)
    // {
    //     die("sigaction failed");
    // }

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
