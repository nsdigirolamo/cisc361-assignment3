/*

Nicholas DiGirolamo
myshell.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A very simple shell program.

*/

#include <errno.h>
#include <glob.h>
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
#include "prompt.h"
#include "pwd.h"
#include "setenv.h"
#include "which.h"

const int MAX_BUFFER_SIZE = 128;
const int MAX_ARGS = 16;
glob_t glob_buffer;

void built_in_cmd_message (const char *name) {
    fprintf(stdout, "[myshell] Executing built-in '%s' command.\n", name);
}

void not_built_in_cmd_message (const char *name) {
    fprintf(stdout, "[myshell] Executing external '%s' command.\n", name);
}

void signal_handler (int signal) {
    fprintf(stdout, "\n");
    print_prompt();
}

void my_exit(int retval) {
    built_in_cmd_message("exit");
    globfree(&glob_buffer);
    cd_cleanup();
    prompt_cleanup();
    exit(retval);
}

int main (int argc, char *argv[]) {

    char input[MAX_BUFFER_SIZE];
    char *args[MAX_ARGS];
    int arg_count = 0;

    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
    signal(SIGTERM, signal_handler);

    while (true) {

        if (print_prompt() != 0) {
            perror("[myshell] Error printing prompt");
            continue;
        }

        // Getting input
        if (fgets(input, MAX_BUFFER_SIZE, stdin) == NULL) {
            if (ferror(stdin)) {
                perror("[myshell] Input Error");
                continue;
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

        if (arg_count == 0) {
            continue;
        }

        // Parsing globs
        int flags = GLOB_NOCHECK | GLOB_BRACE | GLOB_TILDE;
        glob(args[0], flags, NULL, &glob_buffer);
        for (int i = 1; i < arg_count; i++) {
            glob(args[i], flags | GLOB_APPEND, NULL, &glob_buffer);
        }
        arg_count = glob_buffer.gl_pathc;
        args[arg_count];
        for (int i = 0; i < arg_count; i++) {
            args[i] = glob_buffer.gl_pathv[i];
        }

        if (strcmp(args[0], "exit") == 0) {

            my_exit(0);

        } else if (strcmp(args[0], "which") == 0) {

            built_in_cmd_message("which");
            list_element *path_locations = which(arg_count, args, false);

            if (path_locations != NULL) {
                print_list(path_locations);
                free_list(path_locations);
            } else if (errno == EINVAL) {
                printf("[which] Error: Not enough arguments.\n");
            } else if (errno == ENOENT) {
                printf("[which] Error: Argument(s) not found in the PATH.\n");
            }

        } else if (strcmp(args[0], "where") == 0) {

            built_in_cmd_message("where");
            list_element *path_locations = which(arg_count, args, true);

            if (path_locations != NULL) {
                print_list(path_locations);
                free_list(path_locations);
            } else if (errno == EINVAL) {
                printf("[where] Error: Not enough arguments.\n");
            } else if (errno == ENOENT) {
                printf("[where] Error: Argument(s) not found in the PATH.\n");
            }

        } else if (strcmp(args[0], "cd") == 0) {

            built_in_cmd_message("cd");

            if (cd(arg_count, args) != 0) { 
                perror("[cd] Error"); 
            }

        } else if (strcmp(args[0], "pwd") == 0) {

            built_in_cmd_message("pwd");
            pwd();

        } else if (strcmp(args[0], "list") == 0) {

            built_in_cmd_message("list");
            list(arg_count, args);

        } else if (strcmp(args[0], "pid") == 0) {

            built_in_cmd_message("pid");
            fprintf(stdout, "%d\n", getpid());

        } else if (strcmp(args[0], "kill") == 0) {

            built_in_cmd_message("kill");
            my_kill(args, arg_count);

        } else if (strcmp(args[0], "prompt") == 0) {

            built_in_cmd_message("prompt");

            if (set_prefix(arg_count, args) != 0) {
                perror("[myshell]: Error setting prompt");
            }

        } else if (strcmp(args[0], "printenv") == 0) {

            built_in_cmd_message("printenv");

            if (printenv(arg_count, args) != 0) {
                if (errno == EINVAL) {
                    fprintf(stderr, "[printenv] Error: %s could not be found.");
                } else if (errno == E2BIG) {
                    fprintf(stderr, "[printenv] Error: Too many arguments.");
                }
            } 

        } else if (strcmp(args[0], "setenv") == 0) {

            built_in_cmd_message("setenv");

            if (my_setenv(arg_count, args) != 0) {
                if (errno = E2BIG) {
                    fprintf(stderr, "[setenv] Error: Too many arguments.");
                }
            }

        } else if (args[0][0] == '.' || args[0][0] == '/') {

            if (access(args[0], X_OK) == 0) {
                not_built_in_cmd_message(args[0]);
                execute_external(args, arg_count);
            } else {
                fprintf(stderr, "[myshell] '%s' is not a known command.\n", args[0]);
            }

        } else {

            char *temp_argv[] = { NULL, args[0] };
            int temp_argc = 2;
            list_element *command_location = which(temp_argc, temp_argv, false);

            if (command_location != NULL) {
                args[0] = command_location->element;
                not_built_in_cmd_message(args[0]);
                execute_external(args, arg_count);
                free_list(command_location);
            } else if (errno == ENOENT) {
                fprintf(stderr, "[myshell] '%s' is not a known command.\n", args[0]);
            }
        }

        globfree(&glob_buffer);
    }
}