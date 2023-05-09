#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <readline/history.h>

#define FILE_HISTORY "/tmp/dush.hist"

void dush_add_history(char* input) {
    if (strlen(input) == 0) {
        return;
    }

    // Add to readline history.
    add_history(input);

    int fd = open(FILE_HISTORY, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    write(fd, input, strlen(input));
    write(fd, "\n", 1);
    close(fd);
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

