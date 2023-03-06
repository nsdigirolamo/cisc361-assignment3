/*

Nicholas DiGirolamo
myshell.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A very simple shell program.

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 128
#define MAX_ARGS 16

void display_execute_message (const char *name) {
    fprintf(stdout, "Executing built-in [%s]\n", name);
}

int main (int argc, char *argv[]) {

    char input[MAX_BUFFER_SIZE];
    char *args[MAX_ARGS];

    fprintf(stdout, "> ");

    while (fgets(input, MAX_BUFFER_SIZE, stdin) != NULL) {
        if (input[strlen(input) - 1] == '\n') {

            input[strlen(input) - 1] = '\0';

            char *token = strtok(input, " ");
            int arg_count = 0;
            while (token != NULL && arg_count < MAX_ARGS) {
                args[arg_count] = token;
                arg_count++;
                token = strtok(NULL, " ");
            }

            if (strcmp(args[0], "exit") == 0) {

                display_execute_message("exit");
                exit(0);

            } else if (strcmp(args[0], "which") == 0) {

                display_execute_message("which");
                // todo

            } else if (strcmp(args[0], "where") == 0) {

                display_execute_message("where");
                // todo

            } else if (strcmp(args[0], "cd") == 0) {

                display_execute_message("cd");
                char *path = args[1];
                if (chdir(path) == -1) {
                    perror("[cd] Error");
                }

            } else if (strcmp(args[0], "pwd") == 0) {

                display_execute_message("pwd");
                char *ptr = getcwd(NULL, 0);
                if (prt != NULL) {
                    printf("%s\n", ptr);
                } else {
                    perror("[pwd] Error");
                }
                free(ptr);

            } else if (strcmp(args[0], "list") == 0) {

                display_execute_message("list");
                // todo

            } else if (strcmp(args[0], "pid") == 0) {

                display_execute_message("pid");
                // todo

            } else if (strcmp(args[0], "kill") == 0) {

                display_execute_message("kill");
                // todo

            } else if (strcmp(args[0], "prompt") == 0) {

                display_execute_message("prompt");
                // todo

            } else if (strcmp(args[0], "printenv") == 0) {

                display_execute_message("printenv");
                // todo

            } else if (strcmp(args[0], "setenv") == 0) {

                display_execute_message("setenv");
                // todo

            } else {
                // check if the user has given the absolute path to an executable
                // check if the user has given the name of an executable in their PATH
            }
        }

        fprintf(stdout, "> ");
    }
}