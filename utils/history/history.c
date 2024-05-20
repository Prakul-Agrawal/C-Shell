#include "history.h"

char history[MAX_HISTORY_SIZE][MAX_COMMAND_LEN];
char history_path[MAX_PATH_LEN + 1];
int history_count;

void set_history_path()
{
    long bytes = readlink("/proc/self/exe", history_path, MAX_PATH_LEN + 1);
    if (bytes >= 0)
    {
        history_path[bytes] = '\0';
        char *last_slash = strrchr(history_path, '/');
        if (last_slash)
        {
            *(last_slash + 1) = '\0';
            strcat(history_path, "utils/history/history.txt");
        }
    }
    else
    {
        die("readlink() failed");
    }
}

void set_history() {
    FILE *fp = fopen(history_path, "a+");
    if (!fp)
    {
        err("fopen() failed");
        rep(i, 0, MAX_HISTORY_SIZE)
            strcpy(history[i], "");
        return;
    }

    char buffer[MAX_COMMAND_LEN + 1];
    history_count = 0;
    while (fgets(buffer, MAX_COMMAND_LEN, fp) != NULL)
    {
        strcpy(history[history_count], buffer);
        history_count++;
    }

    fclose(fp);
}

char (*get_history())[MAX_COMMAND_LEN]
{
    set_history();
    return history;
}

int get_history_count()
{
    set_history();
    return history_count;
}

void update_history(const char *command)
{
    set_history();

    char input[MAX_COMMAND_LEN + 1] = "";
    char output[MAX_COMMAND_LEN + 1] = "";
    char buffer[MAX_COMMAND_LEN + 1] = "";

    strcpy(input, command);
    char *inputPtr = input;
    char *outputPtr = output;
    char *bufferPtr, *pattern, *p;

    while (*inputPtr) {
        strcpy(buffer, "");
        bufferPtr = buffer;
        pattern = "pastevents";
        while (*inputPtr && isspace(*inputPtr)) {
            *outputPtr++ = *inputPtr++;
        }
        p = pattern;
        while (*p && *inputPtr == *p) {
            *bufferPtr++ = *inputPtr++;
            p++;
        }
        if (*p == '\0' && isspace(*inputPtr)) {
            pattern = "execute";
            while (*inputPtr && isspace(*inputPtr)) {
                *bufferPtr++ = *inputPtr++;
            }
            p = pattern;
            while (*p && *inputPtr == *p) {
                *bufferPtr++ = *inputPtr++;
                p++;
            }

            if (*p == '\0' && isspace(*inputPtr)) {
                while (*inputPtr && isspace(*inputPtr)) {
                    *bufferPtr++ = *inputPtr++;
                }
                char number[MAX_COMMAND_LEN + 1];
                char *numberPtr = number;
                if (isdigit(*inputPtr)) {
                    while (*inputPtr && !isspace(*inputPtr) && *inputPtr != ';' && *inputPtr != '&') {
                        *numberPtr++ = *inputPtr++;
                    }
                    *numberPtr = '\0';
                    if (!is_num(number) || str_to_num(number) > history_count || str_to_num(number) <= 0) {
                        return;
                    }
                    else {
                        strcpy(buffer, history[history_count - str_to_num(number)]);
                        bufferPtr = buffer + strlen(buffer);
                        *(bufferPtr - 1) = ' ';
                    }
                }
                else
                    return;
            }
            else 
                return;
        }

        *bufferPtr++ = *inputPtr++;
        *bufferPtr = '\0';
        *outputPtr = '\0';
        strcat(output, buffer);
        outputPtr = output + strlen(output);
    }
    
    *outputPtr = '\0';
    size_t len = strlen(output);
    int i = len - 1;
    while (i >= 0 && isspace(output[i])){
        output[i] = '\0';
        i--;
    }
    if (i >= 0 && output[i] != '\n'){
        output[i + 1] = '\n';
        output[i + 2] = '\0';
    }

    strcpy(input, output);

    if (history_count != 0 && strcmp(history[history_count - 1], input) == 0)
        return;

    FILE *fp = fopen(history_path, "w");
    if (!fp)
    {
        err("fopen() failed in update_history()");
        return;
    }

    if (history_count == MAX_HISTORY_SIZE)
    {
        rep(i, 0, MAX_HISTORY_SIZE - 1)
            strcpy(history[i], history[i + 1]);
        strcpy(history[MAX_HISTORY_SIZE - 1], input);
    }
    else
    {
        strcpy(history[history_count], input);
        history_count++;
    }

    rep(i, 0, history_count)
    {
        fprintf(fp, "%s", history[i]);
    }

    fclose(fp);
}

void purge_history()
{
    FILE *fp = fopen(history_path, "w");
    if (!fp)
    {
        err("fopen() failed");
        return;
    }

    rep(i, 0, MAX_HISTORY_SIZE)
        strcpy(history[i], "");

    history_count = 0;

    fclose(fp);
}