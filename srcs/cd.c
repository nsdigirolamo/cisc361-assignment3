/*

Nicholas DiGirolamo
cd.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A simple cd command.

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "cd.h"

void cd (char *args[], int arg_count) {

    if (arg_count < 2) {
        char *ptr = getenv("HOME");
        char *path = malloc((strlen(ptr) + 1) * sizeof(char));
        strcpy(path, ptr);
        if (chdir(path) == -1) {
            perror("[cd] Error");
        }
        free(path);
    } else {
        char *path = args[1];
        if (chdir(path) == -1) {
            perror("[cd] Error");
        }
    }

}