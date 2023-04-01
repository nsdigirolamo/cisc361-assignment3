/*

Nicholas DiGirolamo
file_redirect.h

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

*/

#ifndef FILE_REDIRECT_H
#define FILE_REDIRECT_H

int parse_redirect (int argc, char *argv[]);
void restore_redirect ();
int swap_noclobber ();

#endif