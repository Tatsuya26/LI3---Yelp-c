#ifndef USERS_H
#define USERS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "glibWarningAvoid.h"

#define MAX_BUFFER 500000

typedef struct user* USER;

USER init_user(char *user_id, char *name, char *friends);
USER init_user_from_line(char *line, char *delim, int readFriends);

char* get_user_id(USER u);
char* get_name(USER u);
char* get_friends(USER u);
GList* get_friends_from_line(char *line);

void set_user_id(USER u, char *user_id);
void set_name(USER u, char *name);
void set_friends(USER u, char *friends);


USER clone_user(USER u);
void destroy_user(USER u);


#endif
