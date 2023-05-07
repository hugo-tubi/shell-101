#include "constants.h"
#include "history.h"
#include "parser.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

        dush_add_history(input);

        int n_cmds = 0;
        char* commands[MAX_CMDS];
        for (int i = 0; i < MAX_CMDS; i++) {
            commands[i] = malloc(MAX_TOKEN_LEN * sizeof(char));
        }
        split_commands(input, commands, &n_cmds);

        for (int j = 0; j < n_cmds; j++) {
            printf("command %d: ", j);

            int n_args = 0;
            char* args[MAX_ARGS];
            for (int i = 0; i < MAX_ARGS; i++) {
                args[i] = malloc(MAX_TOKEN_LEN * sizeof(char));
            }
            split_args(commands[j], args, &n_args);

            printf("[");
            for (int i = 0; i < n_args; i++)
                printf("\'%s\', ", args[i]);
            printf("]\n");

            // free args
            for (int i = 0; i < MAX_ARGS; i++) {
                free(args[i]);
            }
        }

        // free resources
        for (int i = 0; i < MAX_CMDS; i++) {
            free(commands[i]);
        }
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
