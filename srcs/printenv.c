/*

Nicholas DiGirolamo
printenv.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A simple printenv command.

*/

#include <stdio.h>

#include "printenv.h"

extern char **environ;

void printenv () {
    for (int i = 0; environ[i] != NULL; i++) {
        fprintf(stdout, "%s\n", environ[i]);
    }
}