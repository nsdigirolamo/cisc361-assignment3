/*

Nicholas DiGirolamo
get_path.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

Returns a linked list of the components of the PATH environment variable.

*/

path_list_element *get_path () {

    char *ptr = getenv("PATH");
    char *path = malloc(strlen(ptr) * sizeof(char));
    strcpy(path, ptr);

}