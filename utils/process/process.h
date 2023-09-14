#ifndef MP1_PROCESS_H
#define MP1_PROCESS_H

#include "../../headers.h"

int addProcess(pid_t pid, const char *command, const char *status, bool isBackground);

void updateProcessStatus(pid_t pid, const char *status, bool exited);

void emptyExitedProcessArray();

void processUpdateHandler();

#endif //MP1_PROCESS_H