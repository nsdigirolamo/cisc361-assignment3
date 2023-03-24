/*

Nicholas DiGirolamo
linkedlist.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

Utilities for a linked list.

*/

#include "linkedlist.h"

#include <stdio.h>
#include <stdlib.h>

void print_list (list_element *head) {
    while (head) {
        printf("%s\n", head->element);
        head = head->next;
    }
}

void free_list (list_element *head) {
    while (head) {
        list_element *temp = head;
        head = head->next;
        free(temp->element);
        free(temp);
    }
}