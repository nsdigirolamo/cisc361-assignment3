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

void list (char *args[], int arg_count) {

    DIR *dir;
    struct dirent *entry;

    if (arg_count < 2) {

        dir = opendir("./");
        if (dir == NULL) {
            perror("[list] Error");
            return;
        }
        fprintf(stdout, "Contents of Current Working Directory:\n");

        entry = readdir(dir);
        while (entry != NULL) {
            fprintf(stdout, "%s\n", entry->d_name);
            entry = readdir(dir);
        }
        closedir(dir);

    } else {

        for (int i = 1; i < arg_count; i++) {
            
            dir = opendir(args[i]);
            if (dir == NULL) {
                fprintf(stderr, "[list] Error opening directory %s: ", args[i]);
                perror("");
                continue;
            }
            fprintf(stdout, "\nContents of %s:\n", args[i]);
            
            entry = readdir(dir);
            while (entry != NULL) {
                fprintf(stdout, "%s\n", entry->d_name);
                entry = readdir(dir);
            }
            closedir(dir);
        }
    }
}