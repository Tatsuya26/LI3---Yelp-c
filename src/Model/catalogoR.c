#include "../../Includes/Model/catalogoR.h"

struct catalogo {
	GHashTable *reviews;
};

// Funçao de hash
static guint hashF (gconstpointer key) {
	guint hash = g_str_hash(key);
	return hash;
}

//Funcao de comparacao
static gboolean my_strcmp (gconstpointer a,gconstpointer b) {
	int r = strcmp ((char *) a,(char *) b);
	return !r;
}

static catReview initcatReview (){
	catReview new = malloc (sizeof(struct catalogo));
	new->reviews = g_hash_table_new_full ((GHashFunc)hashF,(GEqualFunc) my_strcmp ,(GDestroyNotify) free, (GDestroyNotify) destroy_review);
	return new;
}

catReview initcatReviewfromFile (char *file,catUser cu,B_NETWORK bn) {
	catReview c = initcatReview();
	FILE *f = fopen (file,"r");
	if (!f) {
		return NULL;
	}
	char *buffer = malloc (sizeof(char) * 50000);
	while (fgets (buffer,50000,f) != NULL) {
		REVIEW r = init_review_fromline (buffer,";",cu,bn);
		if (r) {
			insertcatReview (c,r);
		}
		destroy_review(r);
		free(buffer);
		buffer = malloc (sizeof(char) * 50000);
	}
	free(buffer);
	fclose(f);
	if (catSize (c) == 0) {destroycatReview(c); return NULL;}
	return c;
}

int catReviewremove (catReview c,char *id){
	return g_hash_table_remove(c->reviews,id);
}

int catContem (catReview c,char *id) {
	return g_hash_table_contains(c->reviews,id);
}

int catSize (catReview c) {
	return g_hash_table_size(c->reviews);
}




int insertcatReview(catReview c,REVIEW r) {
	REVIEW nr = clone_review(r);
	char *id = get_review_id(nr);
	int ret = g_hash_table_insert (c->reviews,strdup(id),nr);
	free(id);
	return ret;
}

void destroycatReview (catReview c) {
	if (c) { 
		g_hash_table_destroy (c->reviews);
		free(c);
	}
}

catIT initcatIT (catIT iter,catReview c) {
	g_hash_table_iter_init (&iter,c->reviews);
	return iter;
}

gboolean catIT_next (catIT *iter,REVIEW *r) {
	gpointer chave,valor;
	if (g_hash_table_iter_next(iter,&chave,&valor)){
		*r = clone_review ((REVIEW) valor);
		return TRUE;
	}
	else return FALSE;
}

RV get_RV_reviewed_from_catalog(catReview c, char *business_id) {
	catIT iter  = initcatIT (iter,c);
    REVIEW r     = NULL;
	char *b_id = NULL;
	RV  l_review = init_RV_empty();
	set_rv_id(l_review, business_id);
	while (catIT_next (&iter,&r) == TRUE) {
		b_id = get_review_business_id(r);
		if(strcmp(b_id,business_id) == 0) {
			inc_RV(l_review);
			float x = get_review_star(r);
			add_stars(l_review,x);
		}
		free(b_id);
		destroy_review(r);
	}
	return l_review;
}

void set_stars_for_businesses_list(catReview c, HASH_RV a) {
	catIT iter = initcatIT (iter,c);
	REVIEW r   = NULL;
	while (catIT_next (&iter,&r) == TRUE) {
		char* id = get_review_business_id(r);
		if(hash_RV_Contem(a,id) == TRUE) {
			float stars = get_review_star(r);
			RV new = init_RV("","",1,stars);
			RV aux = get_RV_from_Hash(a,id);
			complementRV(aux,new);
			Destroy_RV(new);
		}
		destroy_review(r);
		free(id);
	}
}

