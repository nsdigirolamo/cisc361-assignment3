/*

Nicholas DiGirolamo
myshell.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A very simple shell program.

*/

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cd.h"
#include "execute_external.h"
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

char *prefix = NULL;

void built_in_cmd_message (const char *name) {
    fprintf(stdout, "[myshell] Executing built-in '%s'.\n", name);
}

void not_built_in_cmd_message (const char *name) {
    fprintf(stdout, "[myshell] Executing external '%s'.\n", name);
}

void display_prompt() {

    char *ptr = getcwd(NULL, 0);

    // If the prefix is NULL, the default prompt uses '>' as the prefix.
    if (ptr != NULL && prefix != NULL) {
        printf("[%s] %s ", ptr, prefix);
    } else if (ptr != NULL) {
        printf("[%s] > ", ptr);
    } else {
        perror("[myshell] Error showing prompt");
    }
    
    fflush(stdout);

    free(ptr);
}

void signal_handler (int signal) {
    fprintf(stdout, "\n");
    display_prompt();
}

int main (int argc, char *argv[]) {

    /*
    // Debugging argv
    for (int i = 0; i < argc; i++) {
        fprintf(stderr, "argv[%d]: %s\n", i, argv[i]);
    }
    */

    char input[MAX_BUFFER_SIZE];
    char *args[MAX_ARGS];
    int arg_count = 0;

    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
    signal(SIGTERM, signal_handler);

    while (true) {

        display_prompt();

        // Getting input
        if (fgets(input, MAX_BUFFER_SIZE, stdin) == NULL) {
            if (ferror(stdin)) {
                perror("[myshell] Input Error");
                exit(-1);
            } else if (feof(stdin)) {
                clearerr(stdin);
                fprintf(stdout, "[myshell] Use 'exit' to leave.\n");
                continue;
            }
        }

        // Parsing input
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

        /*
        // Debugging args
        for (int i = 0; i < arg_count; i++) {
            fprintf(stderr, "args[%d]: %s\n", i, args[i]);
        }
        */

        if (arg_count == 0) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {

            built_in_cmd_message("exit");
            free(prefix);
            exit(0);

        } else if (strcmp(args[0], "which") == 0) {

            built_in_cmd_message("which");
            which(args, arg_count);

        } else if (strcmp(args[0], "where") == 0) {

            built_in_cmd_message("where");
            where(args, arg_count);

        } else if (strcmp(args[0], "cd") == 0) {

            built_in_cmd_message("cd");
            cd(args, arg_count);

        } else if (strcmp(args[0], "pwd") == 0) {

            built_in_cmd_message("pwd");
            pwd();

        } else if (strcmp(args[0], "list") == 0) {

            built_in_cmd_message("list");
            list(args, arg_count);

        } else if (strcmp(args[0], "pid") == 0) {

            built_in_cmd_message("pid");
            fprintf(stdout, "%d\n", getpid());

        } else if (strcmp(args[0], "kill") == 0) {

            built_in_cmd_message("kill");
            my_kill(args, arg_count);

        } else if (strcmp(args[0], "prompt") == 0) {

            built_in_cmd_message("prompt");

            if (arg_count < 2) {

                fprintf(stdout, "Input new prompt prefix: ");

                if (fgets(input, MAX_BUFFER_SIZE, stdin) == NULL) {
                    perror("[myshell] Input Error");
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

            built_in_cmd_message("printenv");
            printenv();

        } else if (strcmp(args[0], "setenv") == 0) {

            built_in_cmd_message("setenv");
            my_setenv(args, arg_count);

        } else if (args[0][0] == '.' || args[0][0] == '/') {

            if (access(args[0], X_OK) == 0) {
                not_built_in_cmd_message(args[0]);
                execute_external(args, arg_count);
            } 

        } else {

            char *name = args[0];

            path_element *path = get_path();
            path_element *current = path;
            bool found = false;

            while (current != NULL) {
                // Final string will be "current->element" + '/' + "name" + '\0'
                // So the length has to be the strings' lengths + 2
                int length = strlen(current->element) + strlen(name) + 2;
                char *command = malloc(length * sizeof(char));
                strcpy(command, current->element);
                strcat(command, "/");
                strcat(command, name);
                if (access(command, X_OK) == 0) {
                    found = true;
                    args[0] = command;
                    not_built_in_cmd_message(args[0]);
                    execute_external(args, arg_count);
                }
                free(command);
                current = current->next;
            }

            free_list(path);
            if (!found) {
                fprintf(stderr, "[myshell] '%s' is not a known command.\n", args[0]);
            }
        }
    }
}