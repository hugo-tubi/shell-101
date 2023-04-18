#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#define FILE_HISTORY "/tmp/dush.hist"

void dush_add_history(char*);
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

        printf("got: %s\n", input);

        // Free buffer that was allocated by readline
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
