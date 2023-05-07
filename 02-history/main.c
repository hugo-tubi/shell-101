#include "history.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>

void init_readline();

int main()
{
    init_readline();

    while (1) {
        char* input = readline("\033[32mpro\033[0mmpt> ");

        // Check for EOF.
        if (!input) {
            printf("\n");
            break;
        }

        printf("got: %s\n", input);
        dush_add_history(input);

        // Free buffer that was allocated by readline
        free(input);
    }
    return 0;
}

void init_readline() {
    // readline configs
    rl_bind_key('\t', rl_complete);
    using_history();
    dush_read_history();
}
