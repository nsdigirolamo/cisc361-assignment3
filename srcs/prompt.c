/*

Nicholas DiGirolamo
prompt.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A prompt for the terminal. Provides functions for printing the prompt and
getting its prefix.

*/

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int MAX_BUFFER_SIZE;
char *prefix = NULL;

int print_prompt () {

    char *cwd = getcwd(NULL, 0);
    bool failed = false;

    if (cwd != NULL) {
        if (prefix) {
            fprintf(stdout, "%s [%s] > ", prefix, cwd);
        } else {
            fprintf(stdout, "[%s] > ", cwd);
        }
        free(cwd);
    } else {
        fflush(stdout);
        return -1;
    }

    fflush(stdout);
    return 0;
}

int set_prefix (int argc, char *argv[]) {

    char input[MAX_BUFFER_SIZE];

    if (argc <= 1) {

        fprintf(stdout, "Input new prompt prefix: ");
        if (fgets(input, MAX_BUFFER_SIZE, stdin) == NULL) {
            return -1;
        } else if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
            if (prefix) { free(prefix); }
            prefix = malloc((strlen(input) + 1) * sizeof(char));
            strcpy(prefix, input);
        }

    } else {

        free(prefix);
        prefix = malloc((strlen(argv[1]) + 1) * sizeof(char));
        strcpy(prefix, argv[1]);

    }

    return 0;
}

void prompt_cleanup () {
    free(prefix);
}