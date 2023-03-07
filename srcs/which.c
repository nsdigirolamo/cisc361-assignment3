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

char *which (char *name, path_element *list) {

    char command[PATH_LENGTH];
    bool found = false;

    while (list != NULL) {
        int size = strlen(list->element) + strlen(name);
        if (PATH_LENGTH < size + 1) {
            fprintf(stderr, "[which] Error: Max path length (128) exceeded.\n");
            return (char *) NULL;
        } else {
            sprintf(command, "%s/%s", list->element, name);
            if (access(command, X_OK) == 0) {
                found = true;
                break;
            }
            list = list->next;
        }
    }

    if (found) {
        char *cmd = malloc((strlen(command) + 1) * sizeof(char));
        strcpy(cmd, command);
        return cmd;
    }

    fprintf(stderr, "[which] Error: Command [%s] not found.\n", name);
    return (char *) NULL;
}