/*

Nicholas DiGirolamo
kill.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A simple kill command.

*/

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "kill.h"

void my_kill (char *args[], int arg_count) {

    if (arg_count < 2) {

        fprintf(stderr, "[kill] Error: Not enough arguments provided.\n");

    } else if (arg_count == 2) {

        errno = 0;
        char *endptr;
        long pid = strtol(args[1], &endptr, 10);

        if (errno != 0) {
            perror("[kill] Error parsing PID");
            return;
        }

        if (endptr == args[1]) {
            fprintf(stderr, "[kill] Error: Could not parse PID.\n");
            return;
        }

        if (kill(pid, SIGTERM) == -1) {
            perror("[kill] Error");
        } else {
            fprintf(stdout, "[kill] Killed process %ld with signal %d.\n", pid, SIGTERM);
        }

    } else {

        if (args[1][0] != '-') {
            fprintf(stderr, "[kill] Error: '%s' is an invalid argument.\n", args[1]);
            return;
        }

        errno = 0;
        char *endptr;
        long signal = strtol(args[1], &endptr, 10);

        if (errno != 0) {
            perror("[kill] Error parsing signal number");
            return;
        }

        if (endptr == args[1]) {
            fprintf(stderr, "[kill] Error: Could not parse signal number\n");
            return;
        }

        // Argument is going to be parsed as a negative number since it has a
        // - to the left of it, so we need to make it positive.
        signal = -1 * signal;

        errno = 0;
        long pid = strtol(args[2], &endptr, 10);

        if (errno != 0) {
            perror("[kill] Error parsing PID");
            return;
        }

        if (endptr == args[2]) {
            fprintf(stderr, "[kill] Error: Could not parse PID.\n");
            return;
        }

        if (kill(pid, signal) == -1) {
            perror("[kill] Error");
        } else {
            fprintf(stdout, "[kill] Killed process %ld with signal %ld.\n", pid, signal);
        }
    }

}