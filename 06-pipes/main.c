#include "constants.h"
#include "execute.h"
#include "expansion.h"
#include "history.h"
#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <readline/readline.h>

void init_readline();

int main()
{
    init_readline();
    int terminal_given = 0;

    while (1) {
        if (terminal_given) {
            pid_t gid = getpgid(0);
            give_terminal_to(gid);
        }
        char* input = readline("\033[32mdu\033[0msh> ");

        // Check for EOF.
        if (!input) {
            printf("\n");
            break;
        }

        dush_add_history(input);

        int n_cmds = 0;
        char* commands[MAX_CMDS];
        int children[MAX_CMDS];
        char command_expanded[MAX_TOKEN_LEN];
        for (int i = 0; i < MAX_CMDS; i++) {
            commands[i] = malloc(MAX_TOKEN_LEN * sizeof(char));
        }
        split_commands(input, commands, &n_cmds);

        int pipes[2];
        if (pipe(pipes) == -1)
            printf("pipe error\n");

        for (int j = 0; j < n_cmds; j++) {
            // expand envs
            expand_env_vars(commands[j], command_expanded);

            int n_args = 0;
            int gid = 0;
            char* args[MAX_ARGS];
            for (int i = 0; i < MAX_ARGS; i++) {
                args[i] = malloc(MAX_TOKEN_LEN * sizeof(char));
            }

            split_args(command_expanded, args, &n_args);
            int pid = run_command(args, j, n_cmds, pipes, &gid, &terminal_given);
            if (pid > 0) {
                children[j] = pid;
            }

            // free args
            for (int i = 0; i < MAX_ARGS; i++) {
                free(args[i]);
            }
        }

        close(pipes[0]);
        close(pipes[1]);

        for (int j = n_cmds - 1; j >= 0; j--) {
            waitpid(-1, 0, WUNTRACED | WCONTINUED);
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
