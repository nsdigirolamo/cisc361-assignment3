/*

Nicholas DiGirolamo
path.h

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

*/

#ifndef GET_PATH_H
#define GET_PATH_H

struct path_element {
    char *element;
    struct path_element *next;
} typedef path_element;

path_element *get_path ();
void print_list(path_element *head);
void free_list (path_element *head);

#endif