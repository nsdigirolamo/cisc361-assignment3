/*

Nicholas DiGirolamo
file_redirect.c

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

Simple parsing for file redirection.

*/

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

bool noclobber = false;

bool changed_stdin = false;
bool changed_stdout = false;
bool changed_stderr = false;

/*
 * Parses a list of argument for redirection symbols. Then sets the appropriate
 * standard io streams to the file included in the arguments. On success,
 * returns the index of the symbol in the array, or the length of the array if
 * no symbol was found. On failure, returns -1.
 */
int parse_redirect (int argc, char *argv[]) {

    for (int i = 0; i < argc; i++) {

        bool in = strcmp(argv[i], "<") == 0;
        bool out = strcmp(argv[i], ">") == 0 || strcmp(argv[i], ">&") == 0 || strcmp(argv[i], ">>") == 0 || strcmp(argv[i], ">>&") == 0;
        bool err = strcmp(argv[i], ">&") == 0 || strcmp(argv[i], ">>&") == 0;
        bool append = strcmp(argv[i], ">>") == 0 || strcmp(argv[i], ">>&") == 0;

        if (!(in || out || err || append)) { continue; }

        if (argc <= i + 1) {
            fprintf(stderr, "[myshell] Error: No file for redirection.\n");
            return -1;
        }

        char *file = argv[i + 1];

        if (!in && noclobber) {
            if (!append && access(file, F_OK) == 0) {
                fprintf(stderr, "[myshell] '%s': File exists.\n", file);
                return -1;
            }
            if (append && access(file, F_OK) == -1) {
                fprintf(stderr, "[myshell] '%s': No such file or directory.\n", file);
                return -1;
            }
        }

        int flags = O_CREAT;
        int mode = S_IRUSR | S_IWUSR | S_IRGRP;

        if (in) {
            flags = flags | O_RDONLY;
        } else {
            flags = flags | O_WRONLY;
        }

        if (append) {
            flags = flags | O_APPEND;
        } else if (!in) {
            flags = flags | O_TRUNC;
        }

        int fid = open(file, flags, mode);

        if (in) {
            close(0);
            dup(fid);
            changed_stdin = true;
            fprintf(stderr, "\nflag1\n");
        } 
        
        if (out) {
            close(1);
            dup(fid);
            changed_stdout = true;
        }

        if (err) {
            close(2);
            dup(fid);
            changed_stderr = true;
        }

        close(fid);

        return i;
    }

    return argc;
}

void restore_redirect () {

    int fid = open("/dev/tty", O_RDWR);

    if (changed_stdin) {
        close(0);
        dup(fid);
        changed_stdin = false;
        fprintf(stderr, "\nflag2\n");
    }

    if (changed_stdout) {
        close(1);
        dup(fid);
        changed_stdout = false;
    }

    if (changed_stderr) {
        close(2);
        dup(fid);
        changed_stderr = false;
    }

    close(fid);
}

int swap_noclobber () {
    noclobber = !noclobber;
    return noclobber;
}