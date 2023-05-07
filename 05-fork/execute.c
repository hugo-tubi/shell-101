#include "execute.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int run_command(char** args, int n_args, int* terminal_given) {
    pid_t child_pid;
    pid_t parent_pid = getpid();
    int result;
    switch (child_pid = fork()) {
        case 0:
            setpgid(0, 0);
            printf("child is saying\n");
            exit(0);

        case -1:
            printf("fork error");
            return 2;

        default:
            while (getpgid(child_pid) != child_pid);
            result = give_terminal_to(child_pid);
            *terminal_given = result;

            waitpid(-1, 0, WNOHANG);
            break;
    }

    printf("both child and parent\n");
    return 0;
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
