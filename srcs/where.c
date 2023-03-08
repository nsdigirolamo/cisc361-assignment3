/*

Nicholas DiGirolamo
which.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A simple which command.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "path.h"
#include "where.h"

#define PATH_LENGTH 128

void where (char *args[], int arg_count) {

    if (arg_count < 2) {
        fprintf(stderr, "[which] Error: Not enough arguments provided.\n");
        return;
    }

    char *name = args[1];
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
            fprintf(stdout, "%s\n", command);
        }

        free(command);
        current =  current->next;
    }

    free_list(path);

    if (!found) {
        fprintf(stderr, "[which] Error: Command [%s] not found.\n", name);
    }
}