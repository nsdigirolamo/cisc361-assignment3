/*

Nicholas DiGirolamo
myshell.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A very simple shell program.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cd.h"
#include "kill.h"
#include "list.h"
#include "path.h"
#include "printenv.h"
#include "pwd.h"
#include "setenv.h"
#include "where.h"
#include "which.h"

#define MAX_BUFFER_SIZE 128
#define MAX_ARGS 16

void display_execute_message (const char *name) {
    fprintf(stdout, "Executing built-in [%s]\n", name);
}

void display_prompt(char *prefix) {

    char *ptr = getcwd(NULL, 0);

    if (ptr != NULL && prefix != NULL) {
        printf("[%s] %s ", ptr, prefix);
    } else if (ptr != NULL) {
        printf("[%s] > ", ptr);
    } else {
        perror("[myshell] Error showing prompt");
    }

    free(ptr);
}

char **parse_args(char *args) {

}

int main (int argc, char *argv[]) {

    char input[MAX_BUFFER_SIZE];
    char *args[MAX_ARGS];
    int arg_count = 0;
    char *prefix = NULL;

    while (true) {

        display_prompt(prefix);

        if (fgets(input, MAX_BUFFER_SIZE, stdin) == NULL) {
            perror("[myshell] Error");
            continue;
        }

        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
            char *token = strtok(input, " ");
            arg_count = 0;
            while (token != NULL && arg_count < MAX_ARGS) {
                args[arg_count] = token;
                arg_count++;
                token = strtok(NULL, " ");
            }
        }

        for (int i = 0; i < arg_count; i++) {
            fprintf(stderr, "args[%d]: %s\n", i, args[i]);
        }

        if (arg_count == 0) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {

            display_execute_message("exit");
            free(prefix);
            exit(0);

        } else if (strcmp(args[0], "which") == 0) {

            display_execute_message("which");
            which(args, arg_count);

        } else if (strcmp(args[0], "where") == 0) {

            display_execute_message("where");
            where(args, arg_count);

        } else if (strcmp(args[0], "cd") == 0) {

            display_execute_message("cd");
            cd(args, arg_count);

        } else if (strcmp(args[0], "pwd") == 0) {

            display_execute_message("pwd");
            pwd();

        } else if (strcmp(args[0], "list") == 0) {

            display_execute_message("list");
            list(args, arg_count);

        } else if (strcmp(args[0], "pid") == 0) {

            display_execute_message("pid");
            fprintf(stdout, "%d\n", getpid());

        } else if (strcmp(args[0], "kill") == 0) {

            display_execute_message("kill");
            my_kill(args, arg_count);

        } else if (strcmp(args[0], "prompt") == 0) {

            display_execute_message("prompt");

            if (arg_count < 2) {

                fprintf(stdout, "Input new prompt prefix: ");

                if (fgets(input, MAX_BUFFER_SIZE, stdin) == NULL) {
                    perror("Input Error");
                } else if (input[strlen(input) - 1] == '\n') {
                    input[strlen(input) - 1] = '\0';
                    free(prefix);
                    prefix = malloc((strlen(input) + 1) * sizeof(char));
                    strcpy(prefix, input);
                }

            } else {

                free(prefix);
                prefix = malloc((strlen(args[1]) + 1) * sizeof(char));
                strcpy(prefix, args[1]);

            }

        } else if (strcmp(args[0], "printenv") == 0) {

            display_execute_message("printenv");
            printenv();

        } else if (strcmp(args[0], "setenv") == 0) {

            display_execute_message("setenv");
            my_setenv(args, arg_count);

        } else {
            // check if the user has given the absolute path to an executable
            // check if the user has given the name of an executable in their PATH
        }
    }
}