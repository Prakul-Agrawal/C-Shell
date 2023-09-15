#include "peek.h"

#define BLUE_COLOR "\033[1;34m"
#define GREEN_COLOR "\033[1;32m"
#define RESET_COLOR "\033[0m"

int string_cmp(const void *a, const void *b) {
    char *str1 = *(char **) a;
    char *str2 = *(char **) b;
    while (*str1 && *str2) {
        char c1 = tolower(*str1);
        char c2 = tolower(*str2);
        if (c1 != c2) {
            return c1 - c2;
        }
        str1++;
        str2++;
    }
    return strlen(str1) - strlen(str2);
}

char *getFileTypeAndPermissions(mode_t st_mode) {
    char *buf = SAFE_MALLOC(11);

    if (S_ISDIR(st_mode)) {
        buf[0] = 'd';
    } else if (S_ISLNK(st_mode)) {
        buf[0] = 'l';
    } else {
        buf[0] = '-';
    }

    buf[1] = (st_mode & S_IRUSR) ? 'r' : '-';
    buf[2] = (st_mode & S_IWUSR) ? 'w' : '-';
    buf[3] = (st_mode & S_IXUSR) ? 'x' : '-';
    buf[4] = (st_mode & S_IRGRP) ? 'r' : '-';
    buf[5] = (st_mode & S_IWGRP) ? 'w' : '-';
    buf[6] = (st_mode & S_IXGRP) ? 'x' : '-';
    buf[7] = (st_mode & S_IROTH) ? 'r' : '-';
    buf[8] = (st_mode & S_IWOTH) ? 'w' : '-';
    buf[9] = (st_mode & S_IXOTH) ? 'x' : '-';
    buf[10] = '\0';

    return buf;
}

void printFileDetails(char *filename, struct stat *statbuf) {
    char *fileTypeAndPermissions = getFileTypeAndPermissions(statbuf->st_mode);

    struct passwd *pwd = getpwuid(statbuf->st_uid);
    struct group *grp = getgrgid(statbuf->st_gid);

    struct tm *tm_info;
    char mod_time[20];
    tm_info = localtime(&(statbuf->st_mtime));
    strftime(mod_time, 20, "%b %d %H:%M", tm_info);

    printf("%s %5lu %12s %12s %12ld %s ",
        fileTypeAndPermissions,
        statbuf->st_nlink,
        pwd->pw_name,
        grp->gr_name,
        statbuf->st_size,
        mod_time);

    if (S_ISDIR(statbuf->st_mode)) {
        printf(BLUE_COLOR);
    } 
    else if (statbuf->st_mode & S_IXUSR || statbuf->st_mode & S_IXGRP || statbuf->st_mode & S_IXOTH) {
        printf(GREEN_COLOR);
    }

    printf("%s\n", filename);

    printf(RESET_COLOR);

    free(fileTypeAndPermissions);
}


char **listFilesAndDirs(const char *path, bool showHidden, int *count) {
    DIR *dir;
    struct dirent *entry;
    char **names = NULL;
    int nameCount = 0;

    dir = opendir(path);
    if (dir == NULL) {
        printf("Given path: %s\n", path);
        err("opendir failed");
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL) {
        char fullpath[MAX_PATH_LEN * 2];
        sprintf(fullpath, "%s/%s", path, entry->d_name);

        if (!showHidden && entry->d_name[0] == '.') {
            continue;
        }

        char **temp = (char **)realloc(names, (nameCount + 1) * sizeof(char *));
        if (temp == NULL) {
            die("realloc failed");
        }
        names = temp;
        names[nameCount] = strdup(entry->d_name);
        if (names[nameCount] == NULL) {
            die("strdup failed");
        }
        nameCount++;
    }

    closedir(dir);

    if (nameCount > 1) {
        qsort(names, nameCount, sizeof(char *), string_cmp);
    }

    *count = nameCount;
    return names;
}

void peek(char *input) {
    const int cmd_len = 4;
    char arguments[MAX_COMMAND_LEN + 1];
    size_t input_len = strlen(input);
    strncpy(arguments, input + cmd_len, input_len);

    char **args = split_string(arguments, " \t\n");
    size_t arg_count = get_segment_count(arguments, " \t\n");

    bool showHidden = false;
    bool showDetails = false;
    char path[MAX_PATH_LEN + 1] = ".";
    bool pathSpecified = false;

    rep(i, 0, arg_count) {
        if (args[i][0] == '-') {
            int arg_len = strlen(args[i]);
            rep(j, 1, arg_len) {
                if (args[i][j] == 'a') {
                    showHidden = true;
                } else if (args[i][j] == 'l') {
                    showDetails = true;
                }
                else {
                    printf("Invalid option: %c\n", args[i][j]);
                    rep(i, 0, arg_count) {
                        free(args[i]);
                    }
                    free(args);
                    return;
                }
            }
        }
    }

    int path_count = 0;

    rep(i, 0, arg_count) {
        if (args[i][0] != '-') {
            if (path_count++ > 0) printf("\n");
            strcpy(path, args[i]);
            pathSpecified = true;

            if (args[i][0] == '~') {
                size_t home_path_len = strlen(home_path);
                char temp[MAX_PATH_LEN + 1];
                strcpy(temp, home_path);
                strcat(temp, args[i] + 1);
                strcpy(path, temp);
            }

            int count;
            char **names = listFilesAndDirs(path, showHidden, &count);

            if (names == NULL) {
                continue;
            }

            if (showDetails) {
                rep(i, 0, count) {
                    char fullpath[MAX_PATH_LEN * 2];
                    sprintf(fullpath, "%s/%s", path, names[i]);
                    struct stat statbuf;
                    if (lstat(fullpath, &statbuf) == -1) {
                        err("lstat failed");
                        continue;
                    }
                    printFileDetails(names[i], &statbuf);
                }
            }
            else {
                rep(i, 0, count) {
                    char fullpath[MAX_PATH_LEN * 2];
                    sprintf(fullpath, "%s/%s", path, names[i]);
                    struct stat statbuf;
                    if (lstat(fullpath, &statbuf) == -1) {
                        err("lstat failed");
                        continue;
                    }
                    if (S_ISDIR(statbuf.st_mode)) {
                        printf(BLUE_COLOR);
                    } 
                    else if (statbuf.st_mode & S_IXUSR || statbuf.st_mode & S_IXGRP || statbuf.st_mode & S_IXOTH) {
                        printf(GREEN_COLOR);
                    }
                    printf("%s\n", names[i]);
                    printf(RESET_COLOR);
                }
            }

            rep(i, 0, count) {
                free(names[i]);
            }
            free(names);
        }
    }

    if (!pathSpecified) {
        int count;
        char **names = listFilesAndDirs(path, showHidden, &count);

        if (names == NULL) {
            rep(i, 0, arg_count) {
                free(args[i]);
            }
            free(args);
            return;
        }

        if (showDetails) {
            rep(i, 0, count) {
                char fullpath[MAX_PATH_LEN * 2];
                sprintf(fullpath, "%s/%s", path, names[i]);
                struct stat statbuf;
                if (lstat(fullpath, &statbuf) == -1) {
                    err("lstat failed");
                    continue;
                }
                printFileDetails(names[i], &statbuf);
            }
        }
        else {
            rep(i, 0, count) {
                char fullpath[MAX_PATH_LEN * 2];
                sprintf(fullpath, "%s/%s", path, names[i]);
                struct stat statbuf;
                if (lstat(fullpath, &statbuf) == -1) {
                    err("lstat failed");
                    continue;
                }
                if (S_ISDIR(statbuf.st_mode)) {
                    printf(BLUE_COLOR);
                } 
                else if (statbuf.st_mode & S_IXUSR || statbuf.st_mode & S_IXGRP || statbuf.st_mode & S_IXOTH) {
                    printf(GREEN_COLOR);
                } 
                else {
                    printf(RESET_COLOR);
                }
                printf("%s\n", names[i]);
                printf(RESET_COLOR);
            }
        }

        rep(i, 0, count) {
            free(names[i]);
        }
        free(names);
    }

    rep(i, 0, arg_count) {
        free(args[i]);
    }
    free(args);
}
