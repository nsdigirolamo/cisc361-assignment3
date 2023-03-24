/*

Nicholas DiGirolamo
which.h

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

*/

#ifndef WHICH_H
#define WHICH_H

#include "path.h"

list_element *which (int argc, char *argv[], bool show_all);

#endif