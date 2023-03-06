/*

Nicholas DiGirolamo
get_path.h

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

*/

#ifndef GET_PATH_H
#define GET_PATH_H

struct path_list_element {
    char *element;
    path_element *next;
} typedef path_element;

path_list_element *get_path ();

#endif