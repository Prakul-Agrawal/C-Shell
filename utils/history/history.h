#ifndef MP1_HISTORY_H
#define MP1_HISTORY_H

#include "../../headers.h"
#include "../str_to_num/str_to_num.h"
#include "../delimiter/delimiter.h"

void set_history_path();

char (*get_history())[];

int get_history_count();

void update_history(const char*);

void purge_history();

#endif //MP1_HISTORY_H