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

char *previous_dir = NULL;

void cd (char *args[], int arg_count) {

    char *temp = getcwd(NULL, 0);

    if (arg_count < 2) {
        char *ptr = getenv("HOME");
        char *path = malloc((strlen(ptr) + 1) * sizeof(char));
        strcpy(path, ptr);
        if (chdir(path) == -1) {
            free(temp);
            perror("[cd] Error");
        } else {
            free(previous_dir);
            previous_dir = temp;
        }
        free(path);
    } else if (arg_count == 2 && strcmp(args[1], "-") == 0) {
        if (chdir(previous_dir) == -1) {
            free(temp);
            perror("[cd] Error");
        } else {
            free(previous_dir);
            previous_dir = temp;
        }
    } else {
        char *path = args[1];
        if (chdir(path) == -1) {
            free(temp);
            perror("[cd] Error");
        } else {
            free(previous_dir);
            previous_dir = temp;
        }
    }
}