#include "parser.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#define FILE_HISTORY "/tmp/dush.hist"

#define MAX_ARGS 128
#define MAX_CMDS 128
#define MAX_TOKEN_LEN 1024

void dush_add_history(char*);
void init_readline();
char commands[MAX_CMDS][MAX_TOKEN_LEN];

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

void dush_read_history() {
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE* fp = fopen(FILE_HISTORY, "r");
    if (fp == NULL)
        return;

    while ((read = getline(&line, &len, fp)) != -1) {
        add_history(line);
    }
    fclose(fp);
}

void init_readline() {
    // readline configs
    rl_bind_key('\t', rl_complete);
    using_history();
    dush_read_history();
}

void dush_add_history(char* input) {
    // Add to readline history.
    add_history(input);

    int fd = open(FILE_HISTORY, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    write(fd, input, strlen(input));
    write(fd, "\n", 1);
    close(fd);
}
