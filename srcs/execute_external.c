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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "myshell.h"

extern char **environ;

void execute_external (char *args[], int arg_count) {

    int pid = fork();

    if (pid == 0) {

        char** exec_args = calloc(arg_count + 1, sizeof(char*));

        for (int i = 0; i < arg_count; i++) {
            exec_args[i] = malloc((strlen(args[i]) + 1) * sizeof(char));
            strcpy(exec_args[i], args[i]);
        }

        exec_args[arg_count] = NULL;

        if (execve(args[0], exec_args, environ) == -1) {
            perror("[myshell] Error executing command");
            fprintf(stderr, "[myshell] Command had error number %d", errno);
            for (int i = 0; i < arg_count; i++) {
                free(exec_args[i]);
            }
            free(exec_args);
            my_exit(errno);
        }

    } else {

        int wstatus;
        pid = waitpid(pid, &wstatus, 0);
        if (pid == -1) {
            perror("[myshell] Error making child");
        }

    }
}