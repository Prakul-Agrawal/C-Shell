#include "delimiter.h"

size_t get_segment_count(char* input, char* delimiter) {
    size_t count = 0;
    char *temp = SAFE_MALLOC((strlen(input) + 1) * sizeof(char));
    strcpy(temp, input);
    char *token = strtok(temp, delimiter);
    while (token != NULL) {
        count++;
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

char *left_strip(char* input, char* delimiters)  {
    size_t input_len = strlen(input);
    size_t delimiters_len = strlen(delimiters);
    size_t i = 0;
    while (i < input_len) {
        size_t j = 0;
        while (j < delimiters_len) {
            if (input[i] == delimiters[j]) {
                i++;
                break;
            }
            j++;
        }
        if (j == delimiters_len) {
            break;
        }
    }
    return input + i;
}