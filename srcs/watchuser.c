/*

Nicholas DiGirolamo
watchuser.h

Created for University of Delaware CISC361 - Operating Systems
Assignment 3 - Simple Shell

*/

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utmpx.h>

#include "linkedlist.h"
#include "watchuser.h"

list_element *users = NULL;
pthread_mutex_t users_mutex = PTHREAD_MUTEX_INITIALIZER;

bool matchuser (const char *name) {
    pthread_mutex_lock(&users_mutex);
    list_element *current = users;
    bool is_match = false;
    while (current != NULL) {
        if (strcmp(current->element, name) == 0) {
            is_match = true;
            break;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&users_mutex);
    return is_match;
}

void *watchusers () {

    while (true) {

        struct utmpx *up;
        setutxent();

        while (up = getutxent()) {
            fprintf(stderr, "\n[DEBUG] Watching for users...\n");
            if (up->ut_type == USER_PROCESS && matchuser(up->ut_user)) { 
                fprintf(stdout, "%s has logged on %s from %s\n", up->ut_user, up->ut_line, up->ut_host);
                removewatcheduser(up->ut_user);
            }
            fprintf(stderr, "[DEBUG] Sleeping for 5 seconds...\n");
            sleep(5);
        }
    }
}

void addwatcheduser(const char *name) {
    pthread_mutex_lock(&users_mutex);
    list_element *new = malloc(sizeof(list_element));
    char *new_user = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(new_user, name);
    new->element = new_user;
    new->next = users;
    users = new;
    pthread_mutex_unlock(&users_mutex);
}

void removewatcheduser(const char *name) {
    pthread_mutex_lock(&users_mutex);
    list_element **list_ptr = &users;
    while (*list_ptr != NULL) {
        list_element *current = *list_ptr;
        if (strcmp(current->element, name) == 0) {
            *list_ptr = current->next;
            free(current->element);
            free(current);
        } else {
            *list_ptr = current->next;
        }
    }
    pthread_mutex_unlock(&users_mutex);
}

void watchuser_cleanup () {
    free_list(users);
}

