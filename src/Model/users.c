#include "../../Includes/Model/users.h"


struct user{
	char *user_id;
	char *name;
	char *friends;
};

USER init_user(char *user_id, char *name, char *friends){
	USER r = malloc(sizeof(struct user));
	r-> user_id = strdup(user_id);
	r-> name = strdup(name);
	r -> friends = strdup(friends);
	return r;
}

USER init_user_from_line(char *line, char *delim, int readFriends){
	USER u = malloc(sizeof(struct user));
	int notFullLine = 0;

	if (line){
		set_user_id(u, strsep(&line, delim));
		if(strlen(u-> user_id) != 22)
			notFullLine = 1;
	}
	else notFullLine = 1;

	if (line && strcmp(line, "\n") != 0 && !notFullLine) set_name(u, strsep(&line, delim));
	else notFullLine = 1;

	if (!readFriends) u-> friends = NULL;
	else if (line && strcmp(line, "\n") != 0 && !notFullLine)
		set_friends(u, strsep(&line, delim));
	else notFullLine = 1;
	
	if (notFullLine){
		destroy_user(u);
		return NULL;
	}

	return u;
}

char* get_user_id(USER u){
	return strdup(u-> user_id);
}

char* get_name(USER u){
	return strdup(u-> name);
}

char* get_friends(USER u){
	if(u-> friends != NULL)
		return strdup(u-> friends);
	else
		return NULL;
}

// função que transforma uma string numa lista de friends
// função para ser usada caso seja necessário algo concreto com os friends
GList* get_friends_from_line(char *line){
	char *friend = strsep(&line, ",");

	if(strcmp(friend, "None\n") == 0)
		return NULL;

	GList *list = g_list_alloc();

	if(strlen(friend) == 23){
		friend[22] = '\0';
		list = g_list_prepend(list, friend);
	}
	else{
		list = g_list_prepend(list, friend);
		while(line){
			friend = strsep(&line, ",");
			if(strlen(friend) == 23)
				friend[22] = '\0';
			list = g_list_prepend(list, friend);
		}
	}
	free(line);

	return list;
}

void set_user_id(USER u, char *user_id){
	u-> user_id = strdup(user_id);
}

void set_name(USER u, char *name){
	u-> name = strdup(name);
}

void set_friends(USER u, char *friends){
	u-> friends = strdup(friends);
}

USER clone_user(USER u){
	USER clone = malloc(sizeof(struct user));
	clone-> user_id = get_user_id(u);	
	clone-> name = get_name(u);
	clone-> friends = get_friends(u);
	return u;
}

void destroy_user(USER u){
	if(u){
		free(u-> user_id);
		free(u-> name);
		if(u-> friends != NULL)
			free(u-> friends);
		free(u);
	}
}
