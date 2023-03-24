/*

Nicholas DiGirolamo
setenv.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A simple setenv command. Without arguments it prints a list of the environment
variables. With arguments it changes the value of the environment variable
passed as an argument. On success returns 0. On failure returns -1 and sets
errno to the appropriate value.

E2BIG - Too many arguments.

*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"
#include "path.h"
#include "printenv.h"
#include "setenv.h"

int my_setenv (int argc, char *argv[]) {

    if (argc < 2) {

        char *empty_args[] = { NULL };
        printenv(1, empty_args);
        return 0;

    } else if (argc == 2) {

        setenv(argv[1], "", 1);
        return 0;

    } else if (argc == 3) {

        setenv(argv[1], argv[2], 1);
        return 0;

    } else {

        errno = E2BIG;
        return -1;

    }
}