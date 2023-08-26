#include "delimiter.h"

size_t get_segment_count(char* input, char* delimiter) {
    size_t count = 0;
    char *temp = SAFE_MALLOC((strlen(input) + 1) * sizeof(char));
    strcpy(temp, input);
    char *token = strtok(temp, delimiter);
    while (token != NULL) {
        count++;
        // printf("Token: %s\n", token);
        token = strtok(NULL, delimiter);
    }
    free(temp);
    return count;
}

char **split_string(char* input, char* delimiter) {
    size_t segment_count = get_segment_count(input, delimiter);
    char ** segments = SAFE_MALLOC(segment_count * sizeof(char*));

    char *temp = SAFE_MALLOC((strlen(input) + 1) * sizeof(char));
    strcpy(temp, input);
    char* token = strtok(temp, delimiter);
    size_t i = 0;
    while (token != NULL) {
        segments[i] = SAFE_MALLOC((strlen(token) + 1) * sizeof(char));
        strcpy(segments[i], token);
        token = strtok(NULL, delimiter);
        i++;
    }
    free(temp);
    return segments;
}