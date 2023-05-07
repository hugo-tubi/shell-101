#include "constants.h"
#include "execute.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void get_binary_path(const char*, char*);
void exec_program(char** args, int n_args) {
    char bin[MAX_TOKEN_LEN];
    get_binary_path(args[0], bin);

    char* env[] = {NULL};

    execve(bin, args, env);
}

void run_command(char** args, int n_args, int* terminal_given) {
    pid_t child_pid;
    pid_t parent_pid = getpid();
    int result;
    switch (child_pid = fork()) {
        case 0:
            setpgid(0, 0);

            // exec
            exec_program(args, n_args);

        case -1:
            printf("fork error");
            return;

        default:
            // wait child setpgid done before giving terminal
            while (getpgid(child_pid) != child_pid);
            result = give_terminal_to(child_pid);
            *terminal_given = result;

            waitpid(-1, 0, WUNTRACED | WCONTINUED);
            break;
    }
}

int give_terminal_to(int pid) {
    int given = 1;
    // Block SIGTSTP and other signals while we give the terminal to the child process
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGTSTP);
    sigaddset(&signal_set, SIGTTIN);
    sigaddset(&signal_set, SIGTTOU);
    sigaddset(&signal_set, SIGCHLD);

    pthread_sigmask(SIG_BLOCK, &signal_set, NULL);

    // Give the terminal to the child process
    pid_t child_pid = (pid_t) pid;
    if (tcsetpgrp(STDIN_FILENO, child_pid) == -1) {
        given = 0;
    }

    // Unblock the signals
    pthread_sigmask(SIG_UNBLOCK, &signal_set, NULL);
    return given;
}

void get_binary_path(const char* bin_name, char* absolute_path) {
    const char* path_env = getenv("PATH");
    char* path = strdup(path_env);  // Duplicate the string so we can modify it
    char* token = strtok(path, ":");

    while (token != NULL) {
        // Construct the path to the binary by concatenating the directory and filename
        sprintf(absolute_path, "%s/%s", token, bin_name);

        // Check if the binary exists at the current path
        if (access(absolute_path, X_OK) == 0) {
            free(path);  // Free the memory allocated by strdup()
            return;
        }

        token = strtok(NULL, ":");
    }

    // If we reach here, the binary wasn't found
    fprintf(stderr, "Error: Could not find %s in PATH\n", bin_name);
    exit(1);
}
