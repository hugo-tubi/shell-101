#include "cd.h"
#include "index.h"

#include <string.h>

int is_builtin(char* name) {
    char *names[] = {"cd", "source"};

    int found = 0;
    for (int i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
        if (strcmp(names[i], name) == 0) {
            found = 1;
            break;
        }
    }

    return found;
}

int run_builtin(char** args) {
    if (strcmp(args[0], "cd") == 0) {
        run_cd(args);
    }
    return 0;
}
