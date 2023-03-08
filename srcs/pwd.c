/*

Nicholas DiGirolamo
pwd.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A simple pwd command.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pwd.h"

void pwd () {

    char *ptr = getcwd(NULL, 0);
    if (ptr != NULL) {
        printf("%s\n", ptr);
    } else {
        perror("[pwd] Error");
    }
    free(ptr);

}

