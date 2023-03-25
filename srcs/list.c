/*

Nicholas DiGirolamo
list.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

A simple list command.

*/

#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>

#include "list.h"

void list (int argc, char *argv[]) {

    DIR *dir;
    struct dirent *entry;

    if (argc <= 1) {
        argv[1] = "./";
        argc = 2;
    }

    for (int i = 1; i < argc; i++) {

        dir = opendir(argv[i]);

        fprintf(stdout, "\nContents of %s:\n", argv[i]);

        if (dir == NULL) {
            fprintf(stderr, "[list] Error opening directory %s: ", argv[i]);
            perror("");
            continue;
        }

        entry = readdir(dir);
        while (entry != NULL) {
            fprintf(stdout, "%s\n", entry->d_name);
            entry = readdir(dir);
        }
        closedir(dir);
    }

    fprintf(stdout, "\n");
}