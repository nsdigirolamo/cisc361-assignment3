/*

Nicholas DiGirolamo
execute_external.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

Executes external commands or programs located at the given path. Does not
check if the path is actually has an executable located there, so the caller
has to check beforehand.

*/

#include "execute_external.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "myshell.h"

extern char **environ;

void execute_external (int argc, char *argv[]) {

    bool is_background = false;

    if (strcmp(argv[argc - 1], "&") == 0) {
        is_background = true;
        argv[argc - 1] = '\0';
        argc = argc - 1;
    }

    pid_t child_pid = fork();

    if (child_pid == -1) {

        perror("[myshell] Fork Error");

    } else if (child_pid == 0) {

        char** exec_args = calloc(argc + 1, sizeof(char*));

        for (int i = 0; i < argc; i++) {
            exec_args[i] = malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(exec_args[i], argv[i]);
        }

        exec_args[argc] = NULL;

        if (execve(argv[0], exec_args, environ) < 0) {
            perror("[myshell] Exec Error");
            for (int i = 0; i < argc; i++) {
                free(exec_args[i]);
            }
            free(exec_args);
            my_exit(errno);
        }

    } else if (!is_background) {

        int status;
        pid_t wait_pid = waitpid(child_pid, &status, 0);

        if (wait_pid == -1) {
            perror("[myshell] Wait Error");
        }

        if (WIFSIGNALED(status)) {
            fprintf(stderr, "[myshell] Child killed: PID=%d signal=%d\n", wait_pid, WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            fprintf(stderr, "[myshell] Child stopped: PID=%d signal=%d\n", wait_pid, WSTOPSIG(status));
        }
    }
}