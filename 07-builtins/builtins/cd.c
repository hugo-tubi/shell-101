#include "../constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int run_cd(char** args) {
    char dir_to[MAX_TOKEN_LEN];
    if (args[1] == NULL) {
        char* home = getenv("HOME");
        int len = strlen(home);
        strncpy(dir_to, home, len);
        dir_to[len] = '\0';
    } else {
        int len = strlen(args[1]);
        strncpy(dir_to, args[1], len);
        dir_to[len] = '\0';
    }

    int result = chdir(dir_to);
    if (result == -1) {
        perror("chdir");
        return -1;
    }

    return 0;
}
