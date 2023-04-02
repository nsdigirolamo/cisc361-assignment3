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
#include <sys/wait.h>
#include <unistd.h>

#include "cd.h"
#include "execute_external.h"
#include "file_redirect.h"
#include "kill.h"
#include "list.h"
#include "path.h"
#include "pipe.h"
#include "printenv.h"
#include "prompt.h"
#include "pwd.h"
#include "setenv.h"
#include "which.h"

const int MAX_BUFFER_SIZE = 128;
const int MAX_ARGS = 16;
glob_t glob_buffer;
bool is_piped = false;

void built_in_cmd_message (const char *name) {
    fprintf(stderr, "[myshell] Executing built-in '%s' command.\n", name);
}

void not_built_in_cmd_message (const char *name) {
    fprintf(stderr, "[myshell] Executing external '%s' command.\n", name);
}

void signal_handler (int signal) {
    fprintf(stdout, "\n");
    print_prompt();
}

void zombie_cleanup() {

    int status;
    pid_t wait_pid = waitpid((pid_t)-1, &status, WNOHANG);

    if (wait_pid == -1) {
        if (errno != ECHILD) { perror("[myshell] ZC - Wait Error"); }
        return;
    }

    if (wait_pid != 0) {
        if (WIFEXITED(status)) {
            fprintf(stderr, "[myshell] Child exited normally: PID=%d status=%d\n", wait_pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "[myshell] Child killed: PID=%d signal=%d\n", wait_pid, WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            fprintf(stderr, "[myshell] Child stopped: PID=%d signal=%d\n", wait_pid, WSTOPSIG(status));
        }
    }
}

void my_exit(int retval) {
    if (!is_piped) { built_in_cmd_message("exit"); }
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

        zombie_cleanup();

        if (print_prompt() != 0) {
            perror("[myshell] Error printing prompt");
            continue;
        }

        // Getting input
        if (fgets(input, MAX_BUFFER_SIZE, stdin) == NULL) {
            if (ferror(stdin)) {
                perror("[myshell] Input Error");
                return -1;
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

        // Parsing redirections
        int redirect = parse_redirect(arg_count, args);
        if (redirect < 0) {
            continue;
        } else if (redirect < arg_count) {
            for (int i = redirect; i < arg_count; i++) {
                args[i] = '\0';
            }
            arg_count = redirect;
        }

        // Parsing pipes
        int pipe = parse_pipe(arg_count, args);
        if (abs(pipe) == arg_count) {
            continue;
        } else if (abs(pipe) < arg_count && pipe != 0) {
            is_piped = true;
            if (pipe < 0) {
                pipe = -1 * pipe;
                for (int i = pipe; i < arg_count; i++) { args[i] = '\0'; }
                arg_count = pipe;
            } else {
                int count = 0;
                for (int i = pipe + 1; i < arg_count; i++) {
                    args[count] = args[i];
                    count++;
                }
                for (int i = count; i < arg_count; i++) { args[i] = '\0'; }
                arg_count = count;
            }
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

        } else if (strcmp(args[0], "noclobber") == 0) {

            built_in_cmd_message("noclobber");
            fprintf(stdout, "%d\n", swap_noclobber());

        } else if (args[0][0] == '.' || args[0][0] == '/') {

            if (access(args[0], X_OK) == 0) {
                not_built_in_cmd_message(args[0]);
                execute_external(arg_count, args);
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
                execute_external(arg_count, args);
                free_list(command_location);
            } else if (errno == ENOENT) {
                fprintf(stderr, "[myshell] '%s' is not a known command.\n", args[0]);
            }
        }

        globfree(&glob_buffer);
        restore_redirect();

        if (is_piped) { my_exit(0); }
    }
}