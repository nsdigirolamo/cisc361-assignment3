/*

Nicholas DiGirolamo
setenv.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A simple setenv command.

*/

#include <stdio.h>
#include <stdlib.h>

#include "printenv.h"
#include "setenv.h"

void my_setenv (char *args[], int arg_count) {

    if (arg_count < 2) {

        printenv();

    } else if (arg_count == 2) {

        setenv(args[1], "", 1);

    } else if (arg_count == 3) {

        setenv(args[1], args[2], 1);

    } else {

        fprintf(stderr, "[setenv] Too many arguments.");

    }
}