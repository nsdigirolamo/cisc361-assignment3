/*

Nicholas DiGirolamo
which.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A simple which command. Returns the path to a command given its name as a char
pointer. Returns NULL if the path is too long or the command was not found. The
returned value must be freed to avoid memory leakage.

*/

#include <stdio.h>
#include <unistd.h>

#include <stdbool.h>

#include "which.h"

#define PATH_LENGTH 128

void which (char *args[], int arg_count) {

    if (arg_count < 2) {
        fprintf(stderr, "[which] Error: Not enough arguments provided.\n");
        return;
    }

    char *name = args[1];
    path_element *list = get_path();
    path_element *current = list;

    char command[PATH_LENGTH];
    bool found = false;

    while (current != NULL) {
        int size = strlen(current->element) + strlen(name) + 1;
        if (PATH_LENGTH < size) {
            fprintf(stderr, "[which] Error: Max path length (128) exceeded.\n");
            free_list(list);
            return;
        } else {
            sprintf(command, "%s/%s", current->element, name);
            if (access(command, X_OK) == 0) {
                found = true;
                break;
            }
            current = current->next;
        }
    }

    free_list(list);

    if (found) {
        fprintf(stdout, "%s\n", command);
    } else {
        fprintf(stderr, "[which] Error: Command [%s] not found.\n", name);
    }
}