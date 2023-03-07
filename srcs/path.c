/*

Nicholas DiGirolamo
path.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

Parses the components of the PATH environment variable into a linked list.

*/

#include <stdio.h>

#include "path.h"

path_element *get_path () {

    char *ptr = getenv("PATH");
    char *path = malloc((strlen(ptr) + 1) * sizeof(char));
    strcpy(path, ptr);
    char *element = strtok(path, ":");

    path_element *head = malloc(sizeof(path_element));
    head->element = malloc((strlen(element) + 1) * sizeof(char));
    strcpy(head->element, element);
    head->next = NULL;

    path_element *current = head;
    element = strtok(NULL, ":");

    while (element != NULL) {
        current->next = malloc(sizeof(path_element));
        current = current->next;
        current->element = malloc((strlen(element) + 1) * sizeof(char));
        strcpy(current->element, element);
        current->next = NULL;
        element = strtok(NULL, ":");
    }

    free(path);
    return head;
}

void print_list(path_element *head) {
    while (head != NULL) {
        printf("%s\n", head->element);
        head = head->next;
    }
}

void free_list (path_element *head) {
    while (head) {
        path_element *temp = head;
        head = head->next;
        free(temp->element);
        free(temp);
    }
}