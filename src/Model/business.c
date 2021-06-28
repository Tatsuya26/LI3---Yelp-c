#include "../../Includes/Model/business.h"

struct business {
    char* business_id;
    char* name;
    char* city;
    char* state;
    char* categories;
};

static int validS (char *s) {
	if (s)
		return strlen(s) > 0;
	else return 0;
}

static int validState(char *s) {
    if (s) {
        int i = 0,valido = 1;
        while (s[i] != '\0' && valido) {
            if (s[i] < 'A'|| s[i] >'Z' ) valido = 0;
            i++;
        }
        return valido;
    }
    return 0;
}

/*
Initiate a new business struct
*/
BUSINESS init_business(char* id,char* name, char* city, char* state,char* categories) {
    BUSINESS b       = malloc(sizeof(struct business));
    if(validS(id)) set_id(b,id);
    else	   b-> business_id = NULL;
    if(validS(name)) set_nameb(b,name);
    else	   b-> name = NULL;
    if(validS(city)) set_city(b,city);
    else	   b-> city = NULL;
    if(validState(state)) set_state(b,state);
    else	   b-> state = NULL;
    if(validS(categories) || strcmp(categories,"") == 0) set_categories(b,categories);
    else	   b-> categories = NULL;
    return b;
} 

BUSINESS init_business_fromline(char* line, char* separator) {
    char* name = NULL, *city= NULL, *state= NULL, *categories= NULL, *id= NULL;
    BUSINESS b = NULL;
    id            = strdup(strsep(&line,separator));
    if (line) name          = strdup(strsep(&line,separator));
    if (line) city          = strdup(strsep(&line,separator));
    if (line) state         = strdup(strsep(&line,separator));
    if (line) {categories    = strdup(strsep(&line,separator));}

    if (validS(name) && validS(city) && validState(state) && (validS(categories) || strcmp(categories,"") == 0) && validS(id) && (!line || strlen(line) == 0)) b = init_business(id,name,city,state,categories);
    if (id) free(id);
    if(name) free(name);
    if(state) free(state);
    if(city) free(city);
    if(categories) free(categories);
    return b;
}


/*
Getters to acess the struct
*/
char* get_id(BUSINESS b) {
    return strdup(b->business_id);
}

char* get_nameb(BUSINESS b) {
    return strdup(b->name);
}

char* get_city(BUSINESS b) {
    return strdup(b->city);
}

char* get_state(BUSINESS b) {
    return strdup(b->state);
}

char* get_categorie(BUSINESS b) {
    return strdup(b->categories);
}

char* get_business_info(BUSINESS b) {
    char info[2048];
    sprintf(info,"Business id: %s| Name: %s | City: %s | State: %s | Categories: %s",b->business_id, b->name, b->city,b->state,b->categories);
    return strdup(info);
}


/*
Setters to change elements in struct
*/
void set_id(BUSINESS b, char* id) {
    b->business_id = strdup(id);
}

void set_nameb(BUSINESS b, char* name) {
    b->name = strdup(name);
}

void set_city(BUSINESS b, char* city) {
    b->city = strdup(city);
}

void set_state(BUSINESS b, char* state) {
    b->state = strdup(state);
}

void set_categories(BUSINESS b, char* categories) {
    b->categories = strdup(categories);
}


/*
Usefull functions
*/

BUSINESS clone_business(BUSINESS b) {
    BUSINESS new        = malloc(sizeof(struct business));
    new->business_id    = strdup(b->business_id);
    new->city           = strdup(b->city);
    new->name           = strdup(b->name);
    new->state          = strdup(b->state);
    new->categories     = strdup(b->categories);
    return new;
}

void destroy_business(BUSINESS b) {
    if (b != NULL) {
	if(b-> business_id != NULL && strcmp(b->business_id, "")) 
        free(b->business_id);
        if(b-> name != NULL && strcmp(b->name, "") != 0)
	    free(b->name);
        if(b-> city != NULL && strcmp(b->city, "") != 0)
		free(b->city);
        if(b-> state != NULL && strcmp(b->state, "") != 0)
		free(b->state);
        if(b-> categories != NULL && strcmp(b->categories, "") != 0)
		free(b->categories);
        free(b);
    }
}
