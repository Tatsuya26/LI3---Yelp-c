#include "../../Includes/Model/catalogoU.h"
#include "../../Includes/Model/users.h"

struct catalogoUsers{
	GHashTable *users;
};

char *header_userFile = "user_id;name;friends\n";

catUser init_catUser(){
	catUser new = malloc(sizeof(struct catalogoUsers));
	new-> users = g_hash_table_new_full(g_str_hash, g_str_equal, (GDestroyNotify) free, (GDestroyNotify) destroy_user);
	return new;
}

catUser init_catUser_from_file(char *file, char *delim, int readFriends){
	FILE *f = fopen(file, "r");
	if (!f)
		return NULL;

	char *line = malloc(sizeof(char) * MAX_BUFFER);
	catUser us = init_catUser();
	if(fgets(line, MAX_BUFFER, f) > 0){
		if(!strcmp(line, header_userFile)){
			while(fgets(line, MAX_BUFFER, f)){
				USER p = init_user_from_line(line, delim, readFriends);
				if (p != NULL) 
					add_user_to_cat(us, p);
				free(line);
				line = malloc(sizeof(char) * MAX_BUFFER);
			}
		}
	}

	free(line);
	fclose(f);
	if (get_catUser_size(us) == 0){
		destroy_catUser(us);
		return NULL;
	}
	return us;
}

int get_catUser_size(catUser c){
	return g_hash_table_size(c-> users);
}

GHashTable* get_users(catUser c){
	GHashTable* copycat = g_hash_table_new_full(g_str_hash, g_str_equal, (GDestroyNotify) free, (GDestroyNotify) destroy_user);
	GHashTableIter iter;
	gpointer key, value;

	g_hash_table_iter_init(&iter, c-> users);
	while(g_hash_table_iter_next(&iter, &key, &value))
		g_hash_table_insert(copycat, strdup((char *) key), clone_user((USER) value));

	return copycat;
}

void set_users(catUser c, GHashTable *h){
	c-> users = h;
}

void add_user_to_cat(catUser c, USER u) {
	char *id = get_user_id(u);
	g_hash_table_insert(c-> users, strdup(id), u);
	free(id);
}

int user_exists(catUser c, char *id){
	return (g_hash_table_lookup(c-> users, id) != NULL);
}

void destroy_catUser(catUser c){
	if(c != NULL){
		g_hash_table_destroy(c-> users);
		free(c);
	}
}

catUser clone_catUser(catUser c){
	catUser clone = init_catUser();
	clone-> users = get_users(c);
	return clone;
}

