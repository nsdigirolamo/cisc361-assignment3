/*

Nicholas DiGirolamo
path.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

Parses the components of the PATH environment variable into a linked list.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "path.h"

list_element *get_path () {

    char *env_ptr = getenv("PATH");
    char *path = malloc((strlen(env_ptr) + 1) * sizeof(char));
    strcpy(path, env_ptr);

    list_element *head = malloc(sizeof(list_element));
    list_element *current = head;

    char *element = strtok(path, ":");

    while (element) {
        current->element = malloc((strlen(element) + 1) * sizeof(char));
        strcpy(current->element, element);
        element = strtok(NULL, ":");
        if (element) {
            current->next = malloc(sizeof(list_element));
            current = current->next;
        } else {
            current->next = NULL;
        }
    }

    free(path);
    return head;
}