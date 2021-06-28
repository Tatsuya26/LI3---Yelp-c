#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../Includes/Model/AuxStructs.h"




StrArray initStrArray(){
	// Inicializa garray para ser utilizado com strings
	StrArray st = g_array_new(FALSE,FALSE, sizeof(char*));
	return st;
}

StrArray addToStrArray(StrArray st,  gpointer value){
	g_array_append_val(st,value);
	return st;
}


void destroyStrArray(StrArray st){
	for (int i = 0; i < getLength(st); ++i){
		free(getStrElement( st, i));
	}
	g_array_free (st,TRUE);
}

int getLength(StrArray st){
	return st->len;
}

char* getStrElement(StrArray st, int index){
	return strdup(g_array_index( st, char*, index ));

}


struct reviewedBUSINESS{
	char   *id;
	char   *name;
	int    nr_reviews;
	float  total_stars;
};

struct rvArray{
	RV *array;
	int size;
	int sp;
};

RV init_RV(char *id, char *name, int nr_r, float t_s){
	RV r            = malloc(sizeof(struct reviewedBUSINESS));
	r-> id          = strdup(id);
	r-> name        = strdup(name);
	r-> nr_reviews  = nr_r;
	r-> total_stars = t_s;
	return r;
}

RV init_RV_empty() {
	RV r           = malloc(sizeof(struct reviewedBUSINESS));
	r->id          = "";
	r->name        = "";
	r->nr_reviews  = 0;
	r->total_stars = 0;
	return r;
}

RVA init_empty_arrayRV(int top){
	RVA v     = malloc(sizeof(struct rvArray));
	v-> array = malloc(sizeof(RV) * top);
	v-> size  = top;
    v->sp     = 0;  
	for(int i = 0; i < top; i++)
		v-> array[i] = init_RV("", "", 0, 0);

	return v;
}


void inc_RVA_sp(RVA array) {
	array->sp++;
}



void set_rv_id(RV r, char *id){
	r-> id = strdup(id);
}

void set_rv_name(RV r, char *name){
	r-> name = strdup(name);
}

void set_rv_nr_reviews(RV r, int nr_r){
	r-> nr_reviews = nr_r;
}

void set_rv_total_stars(RV r, float t_s){
	r-> total_stars = t_s;
}

char** from_RV_to_line(RV r){
	char **line = malloc(sizeof(char *) * 3);
	line[0] = strdup(r-> id);
	line[1] = strdup(r-> name);
	line[2] = malloc(sizeof(char) * 16);
	float media = r-> total_stars / r-> nr_reviews;
	sprintf(line[2], "%.5f", media);
	return line;
}

char** RVAindex_to_line(RVA v, int index){
	char **line = NULL;
	
	if(index < v-> size)
		line = from_RV_to_line(v-> array[index]);
	
	return line;
}

int actual_size_RVA(RVA v){
	int r = 0;
	int size = v-> size;

	for(int i = 0; i < size; i++){
		if(v-> array[i]-> nr_reviews != 0) 
			r++;
	}

	return r;
}


RV to_reviewedBUSINESS(BUSINESS b, float stars){
	char *id   = get_id(b);
	char *name = get_nameb(b);
	RV r       = init_RV(id, name, 1, stars);
	free(id);
	free(name);
	return r;	
}

void complementRV(RV main, RV aux){
	int nr_r  = main-> nr_reviews + aux-> nr_reviews;
	float t_s = main-> total_stars + aux-> total_stars;
	set_rv_nr_reviews(main, nr_r);
	set_rv_total_stars(main, t_s);
}

float compareRV(RV rv1, RV rv2){
	if(rv1-> nr_reviews == 0)
		return -1;
	if(rv2-> nr_reviews == 0)
		return 1;
	if(!strcmp(rv1-> id, rv2-> id))
		return 0;

	float media1, media2;
	media1 = rv1-> total_stars / rv1-> nr_reviews;
	media2 = rv2-> total_stars / rv2-> nr_reviews;

	if(media1 == media2)
		return -1;

	return (media1 - media2);

}

gboolean addSort_RVA(gpointer key, gpointer value, gpointer data){
	RV rv = cloneRV((RV) value); 
	RVA v = (RVA) data;
	addSort_RVarray(v-> array, rv, v-> size);
	return FALSE;
}

void addSort_RVarray(RV *v, RV rv, int n){
	int sorted = 0;

	for(int i = 0; !sorted && i < n; i++){
		float cmp = compareRV(rv, v[i]);
		if(cmp == 0){
			v[i] = rv;
		}
		else if(cmp < 0){
			sorted = 1;
		}
		else{
			RV tmp = v[i];
			v[i] = rv;
			if(i != 0)
				v[i - 1] = tmp;
			else
				Destroy_RV(tmp);
		}
	}	
}

int checkWordText (char *word,char *text) {
    int i,j = 0,found = 0,sizew = strlen(word);
    char *buf = malloc (sizeof(char) * sizew);
    char *palavra = malloc(sizeof(char) * sizew);
    for (i = 0;word[i] != '\0';i++) {
        palavra[i] = tolower(word[i]);
    }
    palavra[i] = '\0';    
    for (i = 0;text[i] != '\0' && !found;i++) {
        char c = text[i];
        if (ispunct(c) || isspace(c)) {
            free(buf);
            buf = malloc (sizeof(char) * sizew);
            j = 0;
        }
        else {
            while (!ispunct(c) && !isspace(c) && c != '\0' && j < sizew) {
                buf[j++] = tolower(c);
                i++;
                c = text[i];
            }
            if (!ispunct(c) && !isspace(c) && c != '\0') 
                while (!ispunct(c) && !isspace(c) && c != '\0') {
                    i++;
                    c = text[i];
                }
            else {
                buf[j] = '\0';
                if (strcmp (buf,palavra) == 0) found = 1;
            }
            i--;
        }
    }
    return found;
}

void inc_RV(RV r) {
	r->nr_reviews++;
}

void add_stars(RV r, int x) {
	r->total_stars += x;
}

char *get_RV_name(RV r) {
	return strdup(r->name);
}
float get_RV_stars(RV r) {
	return r->total_stars;
}

char *get_RV_id (RV r) {
	return strdup(r->id);
}

int get_RV_nreviews(RV r) {
	return r->nr_reviews;
}

char *get_RV_charnReviews(RV r) {
	int x = get_RV_nreviews(r);
	char str[25];
	sprintf(str,"%d",x);
	return strdup(str);
}

char* get_media_stars(RV r) {
	float x = get_RV_stars(r) / (float) get_RV_nreviews(r);
	char str[25];
	sprintf(str,"%f",x);
	return strdup(str);
}

float get_RV_media(RV r) {
	return get_RV_stars(r) / (float) get_RV_nreviews(r);
}

RV cloneRV (RV r) {
	char *id = get_RV_id(r);
	char *name = get_RV_name(r);
	RV new = init_RV(id, name, get_RV_nreviews(r), get_RV_stars(r));
	free(id);
	free(name);
	return new;
}

void Destroy_RV(RV r) {
	if(r != NULL){
		if(strcmp(r->id, "")){
			free(r->id);
		}
		if(strcmp(r->name, "")){
			free(r->name);
		}
		free(r);
	}
}

int get_RVA_size(RVA r) {
	return r->size;
}

RV get_RV_index(RVA rv, int index) {
	RV r = NULL;
	if(index < rv->size) r = cloneRV(rv->array[index]);
	return r;
}

void set_RVA_index(RVA rv, RV r, int index) {
	if(rv->array[index] != NULL) Destroy_RV(rv->array[index]);
	rv->array[index] = cloneRV(r);
}

BOOLEAN is_Empty_RVA(RVA r) {
	return (r != NULL);
}

void swap_RVA(RVA arr, int i, int j) {
	RV tmp = get_RV_index(arr,i);
	set_RVA_index(arr,get_RV_index(arr,j),i);
	set_RVA_index(arr,tmp,j);
}


void destroyRva(RVA r) {
	if(r != NULL){
		for(int i = 0; i < get_RVA_size(r); i++) {
			Destroy_RV(r-> array[i]);
		}
		free(r-> array);
		free(r);
	}
}

char** convertArrayCinMatrix(char* c) {
	char** aux = malloc(sizeof(char**));
	aux[0] = strdup(c);
	return aux;
}



int checkNumericalString (char *text) {
    int i ,ret = 1,punct = 0;
    if (strlen(text) == 0) ret = 0;
    for(i = 0;text[i] != '\0'&& ret;i++) {
	if ((!isdigit(text[i]) && punct) || isalpha(text[i])) ret = 0;
        if (ispunct(text[i])) punct = 1;
    }
    return ret;
}

///////////////////////////////////////// GARRAY for RV ///////////////////////////////////////////////


RVarray init_RVarray() {
	RVarray r = g_array_new(FALSE,FALSE, sizeof(RV));
	return r;
}

//Apends the value to the array
RVarray add_RVarray(RVarray a, gpointer value) {
	g_array_append_val(a,value);
	return a;
}

void destroy_RVarray(RVarray a) {
	int dim = get_size(a);
	for(int i = 0; i < dim; i++) {
		free(get_RVarray_index(a,i));
	}
	g_array_free(a,TRUE);
}

int get_size(RVarray a) {
	return a->len;
}

RV get_RVarray_index(RVarray a ,int index) {
	return g_array_index(a,RV,index);
}

void set_RVarray_index(RVarray a, RV r, int index) {
	g_array_insert_val(a,index,r);
}

void swap_RVarray(RVarray a, int i, int j) {
	RV tmp  = cloneRV(get_RVarray_index(a,i));
	RV auxj = cloneRV(get_RVarray_index(a,j));
	g_array_remove_index (a,i);
	set_RVarray_index(a,auxj,i);
	g_array_remove_index (a,j);
	set_RVarray_index(a,tmp,j);
}

BOOLEAN contains(RVarray a, char* business_id) {
	RV aux 	  = NULL;
	BOOLEAN r = False;
	char* id  = "";
	for(int i = 0; i < get_size(a) && r == False; i++) {
		aux = get_RVarray_index(a,i);
		id  = get_RV_id(aux);
		if(strcmp(business_id,id) == 0) r = True;
		free(id);
	}
	return r;
}

RV get_RV_by_id(RVarray a, char* business_id) {
	int dim = get_size(a);
	BOOLEAN r = True;
	RV aux,new = NULL;
	char* id;
	for(int i = 0; i < dim && r == True; i++) {
		aux = get_RVarray_index(a,i);
		id  = get_RV_id(aux);
		if(strcmp(id,business_id) == 0) {
			new = cloneRV(aux);
			r = False;
		}
		free(id);
	}
	return new;
}

int partition (RVarray arr,int low, int high) {
    // pivot (Element to be placed at right position)
    RV pivot = get_RVarray_index(arr,high);
	float stars = get_RV_stars(pivot) / (float) (get_RV_nreviews(pivot) + 1);
    int i = (low - 1);
    for (int j = low; j <= high- 1; j++) {
        // If current element is greatter than the pivot
		RV aux  = get_RVarray_index(arr,j);
		float ms = get_RV_stars(aux) / (float) (get_RV_nreviews(aux) + 1);
        if (ms <= stars) {
            i++;    // increment index of smaller element
            swap_RVarray (arr,i,j);
        }
    }
	swap_RVarray(arr,i + 1,high);
    return (i + 1);
}

void quickSort(RVarray arr, int low, int high) {
    if (low < high) {
        /* pi is partitioning index, arr[pi] is now
           at right place */
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);  // Before pi
        quickSort(arr, pi + 1, high); // After pi
    }
}

RVarray drop(RVarray a, int n) {
	RVarray new = init_RVarray();
	int dim = get_size(a);
	int r = (dim < n) ? 0 : (dim - n - 1);
	for(int i = dim - 1; i > r; i--) {
		RV aux = get_RVarray_index(a,i);
		add_RVarray(new,cloneRV(aux));
	}
	return new;
}

////////////////////////////////////////////HASH RV///////////////////////////////////////////////////////
struct hashRV {
	GHashTable *reviewedBUSINESS;
};

static guint hashF (gconstpointer key) {
	guint hash = g_str_hash(key);
	return hash;
}


static gboolean my_strcmp (gconstpointer a,gconstpointer b) {
	int r = strcmp ((char *) a,(char *) b);
	return !r;
}

HASH_RV init_Hash_RV (){
	HASH_RV new = malloc (sizeof(struct hashRV));
	new->reviewedBUSINESS = g_hash_table_new_full ((GHashFunc)hashF,(GEqualFunc) my_strcmp ,(GDestroyNotify) free, (GDestroyNotify) Destroy_RV);
	return new;
}



int hash_RV_remove (HASH_RV r,char *id){
	return g_hash_table_remove(r->reviewedBUSINESS,id);
}

int hash_RV_Contem (HASH_RV r,char *id) {
	return g_hash_table_contains(r->reviewedBUSINESS,id);
}

int hash_RV_Size (HASH_RV r) {
	return g_hash_table_size(r->reviewedBUSINESS);
}

int insert_hash_RV (HASH_RV r,RV new) {
	RV aux   = cloneRV(new);
	char *id = get_RV_id(aux);
	int res  = g_hash_table_insert (r->reviewedBUSINESS,strdup(id),aux);
	free(id);
	return res;
}

void destroy_hash_RV (HASH_RV r) {
	g_hash_table_destroy (r->reviewedBUSINESS);
	free(r);
}

RV_IT init_RV_IT(RV_IT iter, HASH_RV r) {
	g_hash_table_iter_init (&iter,r->reviewedBUSINESS);
	return iter;
}

gboolean RV_IT_next (RV_IT *iter,RV *r) {
	gpointer chave,valor;
	if (g_hash_table_iter_next(iter,&chave,&valor)){
		*r = cloneRV ((RV) valor);
		return TRUE;
	}
	else return FALSE;
}

RV get_RV_from_Hash(HASH_RV r,char* id) {
	RV rv = (RV) g_hash_table_lookup (r->reviewedBUSINESS,id);
	return rv;
}

HASH_RV from_RVarray_to_Hash(RVarray a) {
	HASH_RV r = init_Hash_RV();
	for(int i = 0; i < get_size(a); i++) {
		RV new = get_RVarray_index(a,i);
		insert_hash_RV(r,new);
	}
	return r;
}

RVarray from_hash_to_RVarray(HASH_RV r) {
	RVarray a  = init_RVarray();
	RV_IT iter = init_RV_IT(iter,r);
	RV aux;
	while (RV_IT_next (&iter,&aux) == TRUE) {
		add_RVarray(a,cloneRV(aux));
		Destroy_RV(aux);
	}
	return a;
}


//////////////////////////////////////////////////////////// TREE_RV ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
struct Tree_RV {
    GTree *rv;
};

gint compare_review(gconstpointer a, gconstpointer b) {
	RV rv1 = (RV) a;
	RV rv2 = (RV) b;
	int r;
	if(rv1-> nr_reviews == 0) return -1;
	if(rv2-> nr_reviews == 0) return 1;
	if(!strcmp(rv1-> id, rv2-> id)) return 0;
	float media1, media2;
	media1 = rv1-> total_stars / rv1-> nr_reviews;
	media2 = rv2-> total_stars / rv2-> nr_reviews;
	if(media1 == media2) r = -1;
	else if(media1 > media2) r = 1;
	else r = -1;
	return r;
}

TREE_RV init_Tree_rv(){
	TREE_RV new = malloc(sizeof(struct Tree_RV));
	new->rv     =  g_tree_new_full((GCompareDataFunc)compare_review,NULL,NULL,(GDestroyNotify) Destroy_RV);
	return new;	
}

void insert_RV (TREE_RV r,RV a) {
    g_tree_insert(r->rv,cloneRV(a),cloneRV(a));
}

void destroy_Tree_RV(TREE_RV r) {
    g_tree_destroy(r->rv);
    free(r);
}

TREE_RV from_Hash_to_Tree(HASH_RV r) {
	TREE_RV new = init_Tree_rv();
	RV_IT iter = init_RV_IT(iter,r);
	RV aux;
	while (RV_IT_next (&iter,&aux) == TRUE) {
		insert_RV(new,aux);
		Destroy_RV(aux);
	}
	return new;
}

gboolean fill_RV_Array(gpointer key, gpointer value, gpointer data) {
	RV aux  = ((RV) value);
	add_RVarray(data,cloneRV(aux));
	return FALSE; 
}

RVarray from_Tree_to_Array(TREE_RV r) {
	RVarray new = init_RVarray();
	g_tree_foreach(r->rv,fill_RV_Array,new);
	return new;
}
