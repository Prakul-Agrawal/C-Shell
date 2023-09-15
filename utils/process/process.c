#include "process.h"

#define MAX_PROCESSES 1000

struct ProcessInfo processArray[MAX_PROCESSES], exitedProcessArray[MAX_PROCESSES];
int numProcesses = 0, numExitedProcesses = 0;

int addProcess(pid_t pid, const char *cmd_name, const char *command, const char *status, bool isBackground) {
    if (numProcesses < MAX_PROCESSES) {
        processArray[numProcesses].pid = pid;
        strcpy(processArray[numProcesses].cmd_name, cmd_name);
        processArray[numProcesses].cmd_name[strlen(cmd_name)] = '\0';
        strcpy(processArray[numProcesses].command, command);
        processArray[numProcesses].command[strlen(command)] = '\0';
        strcpy(processArray[numProcesses].status, status);
        processArray[numProcesses].status[strlen(status)] = '\0';
        processArray[numProcesses].isBackground = isBackground;
        processArray[numProcesses].index = numProcesses + 1;
        processArray[numProcesses].exited = false;
        processArray[numProcesses].running = true;
        processArray[numProcesses].stopped = false;
        numProcesses++;
    } else {
        die("Maximum number of processes exceeded");
    }
    return numProcesses;
}

void updateProcessStatus(pid_t pid, const char *status, bool exited, bool running, bool stopped) {
    rep(i, 0, numProcesses) {
        if (processArray[i].pid == pid) {
            strcpy(processArray[i].status, status);
            processArray[i].exited = exited;
            processArray[i].running = running;
            processArray[i].stopped = stopped;
            if (exited) {
                exitedProcessArray[numExitedProcesses++] = processArray[i];
                rep(j, i, numProcesses - 1) {
                    processArray[j] = processArray[j + 1];
                }
                numProcesses--;
            }
        }
    }
}

void emptyExitedProcessArray() {
    bool flag = false;
    rep(i, 0, numExitedProcesses) {
        if (exitedProcessArray[i].isBackground) {
            printf("\n[%d] %d %32s %32s", exitedProcessArray[i].index, exitedProcessArray[i].pid, exitedProcessArray[i].status, exitedProcessArray[i].cmd_name);
            flag = true;
        }
    }
    if (flag) printf("\n");
    numExitedProcesses = 0;
}

void processUpdateHandler()
{
    int status = 0;
    pid_t pid = -1;

    while ((pid = waitpid(-1, &status, WUNTRACED | WNOHANG)) > 0)
    {
        if (WIFSIGNALED(status))
        {
            updateProcessStatus(pid, "Killed", true, false, false);
        }
        else if (WIFEXITED(status))
        {
            updateProcessStatus(pid, "Exited normally", true, false, false);
        }
        else if (WIFSTOPPED(status))
        {
            updateProcessStatus(pid, "Suspended normally", false, false, true);
        }
        else
        {
            updateProcessStatus(pid, "Exited abnormally", true, false, false);
        }
    }
}

int compareByPid(const void *a, const void *b) {
    const struct ProcessInfo *processA = (const struct ProcessInfo *)a;
    const struct ProcessInfo *processB = (const struct ProcessInfo *)b;

    return processA->pid - processB->pid;
}

struct ProcessInfo *getSortedProcesses(int *process_count) {

    *process_count = numProcesses;

    struct ProcessInfo *processes = SAFE_MALLOC(numProcesses * sizeof(struct ProcessInfo));
    if (processes == NULL) {
        die("malloc() failed");
    }

    memcpy(processes, processArray, numProcesses * sizeof(struct ProcessInfo));

    qsort(processes, numProcesses, sizeof(struct ProcessInfo), compareByPid);

    return processes;
}