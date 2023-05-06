// Made by OpenAI GPT 3.5
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void split_args(const char* input, char** args, int* n_args) {
    int i = 0, j = 0, in_quotes = 0;
    while (input[i] != '\0') {
        if (input[i] == '\'' || input[i] == '\"') {
            in_quotes = !in_quotes;
            i++;
            continue;
        }
        if (input[i] == ' ' && !in_quotes) {
            args[*n_args][j] = '\0';
            (*n_args)++;
            j = 0;
            i++;
            continue;
        }
        args[*n_args][j] = input[i];
        j++;
        i++;
    }
    args[*n_args][j] = '\0';
    (*n_args)++;
}

void split_commands(const char *input, char **commands, int *n_cmds) {
    char* token = NULL;
    int cmd_idx = 0;
    char* input_dup = strdup(input);

    token = strtok(input_dup, "|");
    while (token != NULL) {
        // remove leading/trailing whitespace from command
        while (*token == ' ') {
            token++;
        }
        int len = strlen(token);
        while (len > 0 && token[len-1] == ' ') {
            len--;
        }
        token[len] = '\0';

        strcpy(commands[cmd_idx], token);
        cmd_idx++;
        token = strtok(NULL, "|");
    }

    *n_cmds = cmd_idx;
    free(input_dup);
}
