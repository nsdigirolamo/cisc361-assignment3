/*

Nicholas DiGirolamo
cd.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A simple cd command. Changes to the directory given as an argument. Providing no
arguments will change to the home directory, Providing '-' as an argument will 
change to the last visited directory. On success returns 0. On failure returns
-1 and sets errno to the appropriate value.

*/

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *previous_dir = NULL;

int cd (int argc, char *argv[]) {

    char *cwd = getcwd(NULL, 0);
    char *destination = NULL;

    bool dest_needs_free = false;

    if (argc <= 1) {

        char *env_ptr = getenv("HOME");
        destination = malloc((strlen(env_ptr) + 1) * sizeof(char));
        strcpy(destination, env_ptr);
        dest_needs_free = true;

    } else if (argc == 2 && strcmp(argv[1], "-") == 0) {

        if (previous_dir) {
            destination = previous_dir;
        } else {
            errno = ENOENT;
            free(cwd);
            return -1;
        }

    } else {

        destination = argv[1];

    }

    bool success = chdir(destination) == 0;
    if (dest_needs_free) { free(destination); }

    if (success) {
        free(previous_dir);
        previous_dir = cwd;
        return 0;
    } else {
        free(cwd);
        return -1;
    }
}

void cd_cleanup () {
    free(previous_dir);
}