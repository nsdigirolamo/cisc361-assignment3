/*

Nicholas DiGirolamo
which.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

The which command. Takes an array of executables as arguments, and tries to
find the location of each executable in the PATH. On success, returns a linked
list of paths to each executable it found. On failure returns NULL and sets
errno based on the below reasons.

EINVAL - Not enough arguments.
ENOENT - None of the commands were found on the PATH.
*/

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "path.h"
#include "which.h"

list_element *which (int argc, char *argv[], bool show_all) {

    if (argc <= 1) {
        errno = EINVAL;
        return NULL;
    }

    list_element *env_path = get_path();

    list_element *head = malloc(sizeof(list_element));
    list_element *current = head;
    list_element *prev = NULL;

    for (int i = 1; i < argc; i++) {

        char *name = argv[i];
        list_element *current_env_path = env_path;

        while (current_env_path) {

            int length = strlen(current_env_path->element) + strlen(name) + 2;
            char *command = malloc(length * sizeof(char));
            strcpy(command, current_env_path->element);
            strcat(command, "/");
            strcat(command, name);

            if (access(command, X_OK) == 0) {
                current->element = command;
                current->next = malloc(sizeof(list_element));
                prev = current;
                current = current->next;
                if (!show_all) { break; }
            } else {
                free(command);
            }

            current_env_path = current_env_path->next;
        }
    }

    free_list(env_path);

    if (prev) {
        // Free current because its space was allocated but never initialized.
        free(current);
        prev->next = NULL;
        return head;
    } else {
        // Free head because its space was allocated but enever initialized.
        free(head);
        errno = ENOENT;
        return NULL;
    }
}