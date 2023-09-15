#include "execute.h"

char *command_names[] = {"exit", "warp", "proclore", "pastevents", "peek", "iMan", "neonate", "activities"};
void (*command_functions[])(char *) = {exit_program, warp, proclore, pastevents, peek, iMan, neonate, activities};

void exit_program(char *input)
{
    exit(EXIT_SUCCESS);
}

bool is_empty(char *s)
{
    while (*s != '\0')
    {
        if (!isspace(*s))
        {
            return false;
        }
        s++;
    }
    return true;
}

void run_cmd(char *cmd, bool isBackground, bool isPipe, int* infile, int outfile, int process_pipe[2])
{
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    char temp[MAX_COMMAND_LEN + 1];
    strcpy(temp, cmd);
    char *cmd_name = strtok(temp, " \t\n");
    bool isCustom = false;

    char input_file[MAX_PATH_LEN + 1] = "", output_file[MAX_PATH_LEN + 1] = "";
    char modified_cmd[MAX_COMMAND_LEN + 1] = "", *cmdPtr = cmd;

    bool input_set = false, output_set = false, is_append = false;
    int old_stdin = dup(STDIN_FILENO), old_stdout = dup(STDOUT_FILENO);

    while(*cmdPtr != '\0') {
        if (*cmdPtr == '<') {
            input_set = true;

            cmdPtr++;

            while(*cmdPtr != '\n' && isspace(*cmdPtr) && *cmdPtr != '\0') cmdPtr++;
            if (*cmdPtr == '\n') {
                err("No input file specified");
                return;
            }

            while(!isspace(*cmdPtr) && *cmdPtr != '\0') strncat(input_file, cmdPtr++, 1);
        } 
        else if (*cmdPtr == '>') {
            output_set = true;

            cmdPtr++;

            if (*cmdPtr == '>') {
                is_append = true;
                cmdPtr++;
            }

            while(*cmdPtr != '\n' && isspace(*cmdPtr) && *cmdPtr != '\0') cmdPtr++;
            if (*cmdPtr == '\n') {
                err("No output file specified");
                return;
            }

            while(!isspace(*cmdPtr) && *cmdPtr != '\0') strncat(output_file, cmdPtr++, 1);
        }
        else {
            strncat(modified_cmd, cmdPtr++, 1);
        }
    }

    if (input_set) {
        int fd = open(input_file, O_RDONLY);
        if (fd == -1) {
            err("open() failed");
            printf("No such input file found!\n");
            return;
        }

        if (dup2(fd, STDIN_FILENO) == -1) {
            err("dup2() failed");
            return;
        }

        if (close(fd) == -1) {
            err("close() failed");
            return;
        }
    }

    if (output_set) {
        int fd;
        if (is_append) {
            fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else {
            fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }

        if (fd == -1) {
            err("open() failed");
            return;
        }

        if (dup2(fd, STDOUT_FILENO) == -1) {
            err("dup2() failed");
            return;
        }

        if (close(fd) == -1) {
            err("close() failed");
            return;
        }
    }

    strcpy(cmd, modified_cmd);

    rep(i, 0, sizeof(command_names) / sizeof(command_names[0]))
    {
        if (strcmp(cmd_name, command_names[i]) == 0)
        {
            isCustom = true;
            command_functions[i](left_strip(cmd, " \t"));
            clock_gettime(CLOCK_MONOTONIC, &end_time);
            last_command_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
            strcpy(last_command, cmd_name);
        }
    }

    if (!isCustom)
    {
        char **args = split_string(cmd, " \t\n");
        size_t arg_count = get_segment_count(cmd, " \t\n");

        pid_t pid = fork();
        if (pid == -1)
        {
            err("fork() failed");
        }
        else if (pid == 0)
        {
            pid_t pgid = getpid();
            if (setpgid(pid, pgid) == -1)
            {
                die("setpgid() failed");
            }

            if (isPipe) {
                close(process_pipe[0]);
            }

            if (!input_set) {
                dup2(*infile, STDIN_FILENO);
            }

            if (!output_set) {
                dup2(outfile, STDOUT_FILENO);
            }

            char **temp_args = realloc(args, sizeof(char *) * (arg_count + 1));
            if (temp_args == NULL)
            {
                die("realloc() failed");
            }
            args = temp_args;
            args[arg_count] = NULL;

            if (execvp(args[0], args) == -1)
            {
                die("execvp() failed");
            }
        }
        else
        {
            int process_index = addProcess(pid, args[0], left_strip(cmd, " \t"), "Running", isBackground);
            if (!isBackground)
            {
                waitpid(pid, NULL, WUNTRACED);
                clock_gettime(CLOCK_MONOTONIC, &end_time);
                last_command_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
                strcpy(last_command, args[0]);
                updateProcessStatus(pid, "Excited normally", true, false, false);
            }
            else
            {
                printf("[%d] %d\n", process_index, pid);
            }
        }

        rep(i, 0, arg_count)
        {
            free(args[i]);
        }
        free(args);
    }

    if (*infile != STDIN_FILENO) {
        close(*infile);
    }
    if (outfile != STDOUT_FILENO) {
        close(outfile);
    }

    if (isPipe) {
        *infile = process_pipe[0];
    }

    if (input_set) {
        if (dup2(old_stdin, STDIN_FILENO) == -1) {
            err("dup2() failed");
            return;
        }

        if (close(old_stdin) == -1) {
            err("close() failed");
            return;
        }
    }

    if (output_set) {
        if (dup2(old_stdout, STDOUT_FILENO) == -1) {
            err("dup2() failed");
            return;
        }

        if (close(old_stdout) == -1) {
            err("close() failed");
            return;
        }
    }
}

void handle_pipes(char *cmd, bool isBackground) {
    char **jobs = split_string(cmd, "|");
    size_t job_count = get_segment_count(cmd, "|");

    bool isPipe = false;

    if (job_count > 1) {
        isPipe = true;
    }

    if (*cmd == '|' || cmd[strlen(cmd) - 1] == '|') {
        printf("Invalid use of pipe\n");
        rep(i, 0, job_count) {
            free(jobs[i]);
        }
        free(jobs);
        return;
    }

    if (isPipe) {
        rep(i, 0, job_count) {
            if (is_empty(jobs[i])) {
                printf("Invalid use of pipe\n");
                rep(j, 0, job_count) {
                    free(jobs[j]);
                }
                free(jobs);
                return;
            }
        }
    }

    if (!isPipe) {
        int infile = STDIN_FILENO, outfile = STDOUT_FILENO;
        run_cmd(cmd, isBackground, false, &infile, outfile, NULL);
    }
    else {
        int infile, outfile, process_pipe[2];
        infile = STDIN_FILENO;

        rep(i, 0, job_count) {
            if (i == job_count - 1) {
                outfile = STDOUT_FILENO;
                run_cmd(jobs[i], isBackground, false, &infile, outfile, process_pipe);
            }
            else {
                if (pipe(process_pipe) < 0) {
                    err("Pipe creation failed");
                    rep(j, 0, job_count) {
                        free(jobs[j]);
                    }
                    free(jobs);
                    return;
                }
                outfile = process_pipe[1];
                run_cmd(jobs[i], isBackground, true, &infile, outfile, process_pipe);
            }
        }
    }

    rep(i, 0, job_count) {
        free(jobs[i]);
    }
    free(jobs);
}

void execute(char *input)
{
    char temp[MAX_COMMAND_LEN + 1] = "";
    char *inputPtr = input;

    while (*inputPtr != '\0')
    {
        if (*inputPtr == '&')
        {
            if (is_empty(temp))
            {
                printf("Invalid empty command\n");
            }
            else
            {
                handle_pipes(temp, true);
            }
            strcpy(temp, "");
        }
        else if (*inputPtr == ';')
        {
            if (is_empty(temp))
            {
                printf("Invalid empty command\n");
            }
            else
            {
                handle_pipes(temp, false);
            }
            strcpy(temp, "");
        }
        else if (*inputPtr == '\n')
        {
            if (!is_empty(temp))
            {
                handle_pipes(temp, false);
            }
            strcpy(temp, "");
        }
        else
        {
            strncat(temp, inputPtr, 1);
        }
        inputPtr++;
    }
}