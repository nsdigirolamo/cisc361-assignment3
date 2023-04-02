/*

Nicholas DiGirolamo
pipe.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

Simple parsing for pipes.

*/

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <myshell.h>

extern char **environ;

 /*
  * Parses a list of arguments for pipe symbols. Then uses pipe() to pipe the
  * appropriate io streams between the given commands. Has multiple return
  * conditions:
  *
  * On Failure, returns the length of the array
  * If the function found no pipe symbols, returns 0
  * If the right side of the pipe needs to be ran, returns the index of the pipe
    symbol as a positive number.
  * If the left side of the pipe needs to be ran, returns the index of the pipe
    symbol as a negative number.
  * If both sides of the pipe have been ran, returns the length of the array
  * as a negative number
  */
int parse_pipe (int argc, char *argv[]) {

    int pipe_index = argc;

    bool out;
    bool err;

    for (int i = 0; i < argc; i++) {

        out = strcmp(argv[i], "|") == 0 || strcmp(argv[i], "|&") == 0;
        err = strcmp(argv[i], "|&") == 0;

        if (!(out || err)) { continue; }

        if (argc <= i + 1) {
            fprintf(stderr, "[myshell] Error: No destination for pipe.\n");
            return argc;
        }
        
        if (i == 0) {
            fprintf(stderr, "[myshell] Error: No source for pipe.\n");
            return argc;
        }

        pipe_index = i;
        break;
    }

    if (pipe_index == argc) {
        return 0; 
    }

    int pipefd[2];

    if (pipe(pipefd) == argc) {
        perror("[myshell] Pipe Error");
        return argc;
    }

    pid_t right_pid = fork();

    if (right_pid == argc) {

        perror("[myshell] Right Pipe Fork Error");

    } else if (right_pid == 0) {

        close(0);
        dup(pipefd[0]);

        close(pipefd[0]);
        close(pipefd[1]);

        return pipe_index;
    }

    pid_t left_pid = fork();

    if (left_pid == argc) {

        perror("[myshell] Left Pipe Fork Error");

    } else if (left_pid == 0) {

        if (out) {
            close(1);
            dup(pipefd[1]);
        }

        if (err) {
            close(2);
            dup(pipefd[1]);
        }

        close(pipefd[0]);
        close(pipefd[1]);

        return -1 * pipe_index;
    }

    close(pipefd[0]);
    close(pipefd[1]);

    int status;
    pid_t wait_pid = waitpid(left_pid, &status, 0);

    while (wait(NULL) > 0);

    return -1 * argc;
}