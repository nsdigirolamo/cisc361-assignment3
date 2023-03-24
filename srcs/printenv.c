/*

Nicholas DiGirolamo
printenv.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A simple printenv command. Prints a list of environment variables, or just a
single environment variable given as an argument. On success, returns 0. On
failure, returns -1 and sets errno accordingly.

EINVAL - Argument could not be found.
E2BIG - Too many arguments.

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "printenv.h"

extern char **environ;

int printenv (int argc, char *argv[]) {

    if (argc < 2) {

        for (int i = 0; environ[i] != NULL; i++) {
            fprintf(stdout, "%s\n", environ[i]);
        }
        return 0;

    } else if (argc == 2) {

        char *env_ptr = getenv(argv[1]);
        if (env_ptr) {
            fprintf(stdout, "%s\n", env_ptr);
            return 0;
        } else {
            errno = EINVAL;
            return -1;
        }

    } else {

        errno = E2BIG;
        return -1;

    }
}