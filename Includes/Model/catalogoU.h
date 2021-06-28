#ifndef CATALOGOU_H
#define CATALOGOU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmodule.h>

#include "users.h"

typedef struct catalogoUsers *catUser;

catUser init_catUser();
catUser init_catUser_from_file(char *file, char *delim, int readFriends);

int get_catUser_size(catUser c);
GHashTable* get_users(catUser c);
void set_users(catUser c, GHashTable *h);

void add_user_to_cat(catUser c, USER u);
int user_exists(catUser c, char *id);

void destroy_catUser(catUser c);
catUser clone_catUser(catUser c);

#endif
