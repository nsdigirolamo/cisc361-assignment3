/*

Nicholas DiGirolamo
watchuser.h

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

*/

#ifndef WATCHUSER_H
#define WATCHUSER_H

bool matchuser (const char *name);
void *watchusers ();
void addwatcheduser(const char *name);
void removewatcheduser(const char *name);
void watchuser_cleanup ();

#endif