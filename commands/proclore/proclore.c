#include "proclore.h"

void proclore(char *input) {
	const int cmd_len = 8;
    char arguments[MAX_COMMAND_LEN + 1];
    size_t input_len = strlen(input);
    strncpy(arguments, input + cmd_len, input_len);

    char **args = split_string(arguments, " \t\n");
    size_t arg_count = get_segment_count(arguments, " \t\n");
	
	pid_t pid;
	
	if (arg_count > 1) {
		printf("Invalid number of arguments for proclore\n");
		rep(i, 0, arg_count) {
			free(args[i]);
		}
		free(args);
		return;
	}
	
	if (arg_count == 0) {
		pid = getpid();
	} 
	else {
		if (!is_num(args[0])) {
			printf("Invalid argument %s\n", args[0]);
			rep(i, 0, arg_count) {
				free(args[i]);
			}
			free(args);
			return;
		}
		else
			pid = str_to_num(args[0]);
	}
	
	char buffer[MAX_PATH_LEN + 1], proc_stat_path[MAX_PATH_LEN + 1], proc_exe_path[MAX_PATH_LEN + 1];

	sprintf(proc_stat_path, "/proc/%d/stat", pid);
	sprintf(proc_exe_path, "/proc/%d/exe", pid);
	
	FILE *fp = fopen(proc_stat_path, "r");
	if (!fp) {
		err("fopen() failed");
		rep(i, 0, arg_count) {
			free(args[i]);
		}
		free(args);
		return;
	}
	
	char status[3], pgrp[100], tpgid[100], memory[100];
	
	{
		int pt = 0;
		while (fscanf(fp, "%s", buffer) != EOF && pt < 23) {
			if (pt == 2) strcpy(status, buffer);
			else if (pt == 4) strcpy(pgrp, buffer);
			else if (pt == 7) strcpy(tpgid, buffer);
			else if (pt == 22) strcpy(memory, buffer);
			pt++;
		}
	}
	
	fclose(fp);

	long bytes = readlink(proc_exe_path, buffer, MAX_PATH_LEN);
	
	if (bytes == -1) {
		err("readlink() failed");
		rep(i, 0, arg_count) {
			free(args[i]);
		}
		free(args);
		return;
	}
	
	buffer[bytes] = '\0';

	size_t home_path_len = strlen(home_path);

    bool flag;
    if (strlen(buffer) >= home_path_len) {
        flag = true;
        rep(i, 0, home_path_len) {
            if (buffer[i] != home_path[i]) {
                flag = false;
                break;
            }
        }
    }
    else {
        flag = false;
    }

    if (flag) {
        char temp[MAX_PATH_LEN + 1];
        temp[0] = '~';
        rep(i, home_path_len, strlen(buffer)) {
            temp[i - home_path_len + 1] = buffer[i];
        }
        temp[strlen(buffer) - home_path_len + 1] = '\0';
        strcpy(buffer, temp);
    }

	printf("pid : %d\n", pid);
	if (strcmp(pgrp, tpgid) == 0) {
		printf("process status : %s+\n", status);
	}
	else {
		printf("process status : %s\n", status);
	}
	printf("process group : %s\n", pgrp);
	printf("virtual memory : %s\n", memory);
	printf("executable path : %s\n", buffer);

	rep(i, 0, arg_count) {
		free(args[i]);
	}
	free(args);
}