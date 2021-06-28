#ifndef AUX_STRUCTS_H
#define AUX_STRUCTS_H 

#include "glibWarningAvoid.h"
#include "business.h"

typedef enum {
    /**Para verificar quando algo é Falso*/
    False = 0, 
    /**Para verificar quando algo é Verdadeiro*/
    True = 1 
} BOOLEAN;

typedef GArray* StrArray;

StrArray initStrArray();

StrArray addToStrArray(StrArray st,  gpointer value );

void destroyStrArray(StrArray st);

int getLength(StrArray st);

char* getStrElement(StrArray st, int index);

char** strToChar(StrArray st);



typedef struct reviewedBUSINESS *RV;
typedef struct rvArray *RVA;

RV init_RV(char *id, char *name, int nr_r, float t_s);
RVA init_empty_arrayRV(int top);
void set_rv_id(RV r, char *id);
void set_rv_name(RV r, char *name);
void set_rv_nr_reviews(RV r, int nr_r);
void set_rv_total_stars(RV r, float t_s);
char** from_RV_to_line(RV r);
int actual_size_RVA(RVA v);
char** RVAindex_to_line(RVA v, int index);
RV to_reviewedBUSINESS(BUSINESS b, float stars);
void complementRV(RV main, RV aux);
float compareRV(RV rv1, RV rv2);
gboolean addSort_RVA(gpointer key, gpointer value, gpointer data);
void addSort_RVarray(RV *v, RV rv, int n);
RV init_RV_empty();
void inc_RV(RV r);
void add_stars(RV r, int x);
float get_RV_stars(RV r);
int get_RV_nreviews(RV r);
char* get_media_stars(RV r);
char *get_RV_charnReviews(RV r);
RV cloneRV (RV r);
char *get_RV_name (RV r) ;
char *get_RV_id (RV r) ;

void Destroy_RV(RV r);
void printRV(RV r);

int checkWordText (char *word,char *text);

char** convertArrayCinMatrix(char* c);

int checkNumericalString(char *text);

void addToRva(RVA array, RV review);

int get_RVA_size(RVA r);
void set_RVA_index(RVA rv, RV r, int index);
RVA take(RVA arr, int num);
void destroyRva(RVA r);
RV get_RV_index(RVA rv, int index);

typedef GArray* RVarray;

void quickSort(RVarray arr, int low, int high);
void sort_RVarray(RVarray a);
RV get_RVarray_index(RVarray a ,int index);
int get_size(RVarray a);
void destroy_RVarray(RVarray a);
RVarray add_RVarray(RVarray a, gpointer value);
RVarray init_RVarray();
gint compareReview (gconstpointer a, gconstpointer b);
void set_RVarray_index(RVarray a, RV r, int index);
BOOLEAN contains(RVarray a, char* business_id);
RV get_RV_by_id(RVarray a, char* business_id);
void swap_RVarray(RVarray a, int i, int j);
typedef struct hashRV *HASH_RV;

typedef GHashTableIter RV_IT;

HASH_RV init_Hash_RV ();
int hash_RV_remove (HASH_RV r,char *id);
int hash_RV_Contem (HASH_RV r,char *id);
int hash_RV_Size (HASH_RV r);
int insert_hash_RV (HASH_RV r,RV new);
void destroy_hash_RV (HASH_RV r);
RV_IT init_RV_IT(RV_IT iter, HASH_RV r);
gboolean RV_IT_next (RV_IT *iter,RV *r);
RV get_RV_from_Hash(HASH_RV r,char* id);
HASH_RV from_RVarray_to_Hash(RVarray a);
RVarray from_hash_to_RVarray(HASH_RV r);
RVarray drop(RVarray a, int n);


typedef struct Tree_RV* TREE_RV;

TREE_RV init_Tree_rv();
void insert_RV (TREE_RV r,RV a);
void destroy_Tree_RV(TREE_RV r);
TREE_RV from_Hash_to_Tree(HASH_RV r);
RVarray from_Tree_to_Array(TREE_RV r);
#endif
