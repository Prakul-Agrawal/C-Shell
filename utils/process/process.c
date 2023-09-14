#include "process.h"

#define MAX_PROCESSES 1000

struct ProcessInfo {
    pid_t pid;
    char command[32];
    char status[32];
    bool isBackground;
    int index;
    bool exited;
};

struct ProcessInfo processArray[MAX_PROCESSES], exitedProcessArray[MAX_PROCESSES];
int numProcesses = 0, numExitedProcesses = 0;

int addProcess(pid_t pid, const char *command, const char *status, bool isBackground) {
    if (numProcesses < MAX_PROCESSES) {
        processArray[numProcesses].pid = pid;
        strcpy(processArray[numProcesses].command, command);
        processArray[numProcesses].command[strlen(command)] = '\0';
        strcpy(processArray[numProcesses].status, status);
        processArray[numProcesses].command[strlen(status)] = '\0';
        processArray[numProcesses].isBackground = isBackground;
        processArray[numProcesses].index = numProcesses + 1;
        processArray[numProcesses].exited = false;
        numProcesses++;
    } else {
        die("Maximum number of processes exceeded");
    }
    return numProcesses;
}

void updateProcessStatus(pid_t pid, const char *status, bool exited) {
    rep(i, 0, numProcesses) {
        if (processArray[i].pid == pid) {
            strcpy(processArray[i].status, status);
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
            printf("\n[%d] %d %32s %32s", exitedProcessArray[i].index, exitedProcessArray[i].pid, exitedProcessArray[i].status, exitedProcessArray[i].command);
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
            updateProcessStatus(pid, "Killed", true);
        }
        else if (WIFEXITED(status))
        {
            updateProcessStatus(pid, "Exited normally", true);
        }
        else if (WIFSTOPPED(status))
        {
            updateProcessStatus(pid, "Suspended normally", false);
        }
        else
        {
            updateProcessStatus(pid, "Exited abnormally", true);
        }
    }
}