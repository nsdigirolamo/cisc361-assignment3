/*

Nicholas DiGirolamo
linkedlist.h

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct linked_list_element {
    char *element;
    struct linked_list_element *next;
} typedef list_element;

void print_list (list_element *head);
void free_list (list_element *head);

#endif