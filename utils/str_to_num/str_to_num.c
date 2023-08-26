#include "str_to_num.h"

long str_to_num(char *input) {
	char *end;
	long count = strtol(input, &end, 10);
	return count;
}

bool is_num(char *input) {
	size_t length = strlen(input);
	
	rep(i, 0, length) {
        if (input[i] < '0' || input[i] > '9') {
            return false;
        }
    }
    return true;
}