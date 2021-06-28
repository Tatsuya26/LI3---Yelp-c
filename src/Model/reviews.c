#include "../../Includes/Model/reviews.h"
#include "../../Includes/Model/data.h"

struct review {
	char *id;
	char *user_id;
	char *business_id;
	float stars;
	int cool;
	int useful;
	int funny;
	data data;
	char *text;	
};

static REVIEW create_review (){
	REVIEW new = malloc (sizeof(struct review));
	return new;
}

//Funcao que valida uma string.
static int validS (char *s) {
	if (s)
		return strlen(s) > 0;
	else return 0;
}

//Funcao que valida o numero de estrelas.
static int validStars(float stars) {
	return stars <= 5 && stars >= 0; 
}

//Funcao que valida se um user pertence ao catalogo dos users.
static int validUser(char *user,catUser cu) {
	if (user_exists(cu,user)) return 1;
	else return 0;
}

//Funcao que valida se um negocio pertence ao catalogo dos negocios.
static int validBusiness (char *business,B_NETWORK bn) {
	int r = 1;
	BUSINESS b = get_business(bn,business);
	if (exist_business(bn,b)) r = 1;
	else r = 0;
	destroy_business(b);
	return r;
}

REVIEW init_review (char *id,char *user,char *business,float stars,int useful,int funny,int cool,data data,char *text) {
	REVIEW new = create_review();
	set_review_id (new,id);
	set_review_user_id (new,user);
	set_review_business_id(new,business);
	set_review_stars(new,stars);
	set_review_cool (new,cool);
	set_review_funny (new,funny);
	set_review_useful(new,useful);
	set_review_data (new,data);
	set_review_text (new,text);
	return new;
}

REVIEW init_review_fromline (char *line,char *separator,catUser cu,B_NETWORK bn){
	char *id = NULL,*user = NULL,*business = NULL,*text = NULL;
	float stars = 0;
	data data = NULL;
	REVIEW r = NULL;
	int cool = 0,useful = 0,funny = 0;
	id = strdup(strsep (&line,separator));
	if (line) user = strdup(strsep (&line,separator));
	if (line) business = strdup(strsep (&line,separator));
	if (line) stars = atof (strsep (&line,separator));
	if (line) useful = atoi (strsep (&line,separator));
	if (line) funny = atoi (strsep (&line,separator));
	if (line) cool = atoi (strsep (&line,separator));
	if (line) data = init_data_fromtext(strsep(&line,separator));
	if (line) text = strdup(strsep (&line,separator));
	if (validS(id) && validUser(user,cu) && validBusiness(business,bn) && cool >= 0 && funny >= 0 && useful >= 0 && validS(text) && validStars(stars) && data) 
				r = init_review (id,user,business,stars,useful,funny,cool,data,text);
	if (id) free(id);
	if (user) free(user);
	if (business) free(business);
	if (text) free(text);
	destroy_data(data);
	return r;
}


char *get_review_id (REVIEW r) {
	return strdup(r->id);
}

char *get_review_user_id (REVIEW r) {
	return strdup(r->user_id);
}

char *get_review_business_id (REVIEW r) {
	return strdup(r->business_id);
}

float get_review_star (REVIEW r) {
	return r->stars;
}

int get_review_cool (REVIEW r) {
	return r->cool;
} 

int get_review_useful (REVIEW r) {
	return r->useful;
}

int get_review_funny (REVIEW r) {
	return r->funny;
}

data get_review_data (REVIEW r) {
	return clone_data(r->data);
}

char *get_review_text (REVIEW r) {
	return strdup(r->text);
}

char* get_review_info (REVIEW r) {
	char info[10000];
	sprintf(info,"Id: %s | User:%s |Business:%s |Stars:%1.f;Cool:%d;Useful:%d;Funny:%d |Data:%d-%d-%d %d:%d:%d |Text:%s ", r->id,r->user_id,r->business_id,r->stars,r->cool,r->useful,r->funny,get_ano(r->data),get_mes(r->data),get_dia(r->data),get_hora(r->data),get_min(r->data),get_seg(r->data),r->text);
	return strdup(info);
}

int set_review_id (REVIEW r,char *id) {
	if (id && r) {
		r->id = strdup (id);
		return 1;
	}
	else return 0;
}

int set_review_user_id (REVIEW r, char *id){
	if (id && r) {
		r->user_id = strdup (id);
		return 1;
	}
	else return 0;
}

int set_review_business_id (REVIEW r,char *id){
	if (id && r) {
		r->business_id = strdup (id);
		return 1;
	}
	else return 0;
}

int set_review_stars (REVIEW r, float star){
	if (r) {
		r->stars = star;
		return 1;
	}
	return 0;
}

int set_review_cool (REVIEW r, int cool){
	if (r) {
		r->cool = cool;
		return 1;
	}
	return 0;
}

int set_review_funny (REVIEW r, int funny){
	if (r) {
		r->funny = funny;
		return 1;
	}
	return 0;
}

int set_review_useful (REVIEW r, int useful){
	if (r) {
		r->useful = useful;
		return 1;
	}
	return 0;
}

int set_review_data (REVIEW r,data d) {
	if (d && r) {
	data new = clone_data (d);
	r->data = new;
	return 1;
	}
	return 0;
}

int set_review_text (REVIEW r,char *text) {
	if (r && text) {
		r->text = strdup (text);
		return 1;
	}
	return 0;
}

REVIEW clone_review (REVIEW r) {
	REVIEW new = malloc (sizeof(struct review));
	new->id = strdup(r->id);
	new->user_id = strdup(r->user_id);
	new->business_id = strdup(r->business_id);
	new->stars = r->stars;
	new->funny = r->funny;
	new->cool = r->cool;
	new->useful = r->useful;
	new->data = clone_data (r->data);
	new->text = strdup (r->text);
	return new;
}

void destroy_review (REVIEW r) {
	if (r) {
		free(r->id);
		free(r->user_id);
		free(r->business_id);
		free(r->text);
		destroy_data(r->data);
		free(r);
	}
}
