/*

Nicholas DiGirolamo
prompt.h

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

*/

#ifndef PROMPT_H
#define PROMPT_H

int print_prompt ();
int set_prefix (int argc, char *argv[]);
void prompt_cleanup ();

#endif