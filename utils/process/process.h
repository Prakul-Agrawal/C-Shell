#ifndef MP1_PROCESS_H
#define MP1_PROCESS_H

#include "../../headers.h"

struct ProcessInfo {
    pid_t pid;
    char cmd_name[32];
    char command[128];
    char status[32];
    bool isBackground;
    int index;
    bool exited;
    bool running;
    bool stopped;
};

int addProcess(pid_t pid, const char *cmd_name, const char *command, const char *status, bool isBackground);

void updateProcessStatus(pid_t pid, const char *status, bool exited, bool running, bool stopped);

void emptyExitedProcessArray();

void processUpdateHandler();

struct ProcessInfo *getSortedProcesses(int *process_count); 

#endif //MP1_PROCESS_H