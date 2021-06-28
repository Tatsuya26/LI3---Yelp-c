#include "../../Includes/Model/business_network.h"

struct business_network {
    GTree *business;
};

gint compare_business(gconstpointer a, gconstpointer b) {
    char* id1 = get_id((BUSINESS) a);
    char* id2 = get_id((BUSINESS) b);
    int r = strcmp(id1,id2);
    free(id1);
    free(id2);
    return r;
}

B_NETWORK init_business_network() {
    B_NETWORK new = malloc(sizeof(struct business_network));
    new->business = g_tree_new_full((GCompareDataFunc)compare_business,NULL,(GDestroyNotify) destroy_business,(GDestroyNotify) destroy_business);
    return new;
}

B_NETWORK load_business_from_file(char* filename) {
    char buffer[15000];
    B_NETWORK b = init_business_network();
    FILE *f = fopen(filename,"r");
    if(f == NULL) return NULL;
    while(fgets(buffer,15000,f) != NULL) {
        char * BLine = (strtok((buffer), "\r\n"));
        BUSINESS business = init_business_fromline(BLine,";");
        if (business) insert_business(b,business);
        destroy_business(business);
        }
    fclose(f);
    if (get_networkSize(b) == 0 ) {destroyNetwork(b); return NULL;}
    return b;
}

void insert_business (B_NETWORK b,BUSINESS bus) {
    g_tree_insert(b->business,clone_business(bus),clone_business (bus));
}

BOOLEAN exist_business(B_NETWORK b, BUSINESS new) {
    char* key = g_tree_lookup(b->business,new);
    BOOLEAN r = (key != NULL) ? True : False;
    return r;
}

int get_networkSize(B_NETWORK b) {
    return g_tree_nnodes(b->business);
}


void destroyNetwork(B_NETWORK n) {
    if (n) {
        g_tree_destroy(n->business);
        free(n);
    }
}


BUSINESS get_business(B_NETWORK b, char* business_id) {
    BUSINESS new = init_business(business_id,"","","","");
    gpointer key = g_tree_lookup(b->business, new);
    destroy_business(new);
    if(key == NULL)
	    return NULL;
    return clone_business((BUSINESS) key);
}

gboolean fillBusinessArray(gpointer key, gpointer value, gpointer data){
    BUSINESS  b    = ((BUSINESS)  value);
    char *name     = get_nameb(b);
    char *letter   = getStrElement(data,0);
    if(tolower(name[0]) == tolower(letter[0])) addToStrArray(data, get_nameb(b));
    free(name);
    free(letter);
    return FALSE;
}

StrArray business_by_letter(B_NETWORK b , char letter) {
    StrArray st = initStrArray();
    char letra[2] = {letter,'\0'};
    addToStrArray(st,letra);
    g_tree_foreach(b->business, fillBusinessArray, st);
    g_array_remove_index_fast (st,0);
    return st;
}

char *get_business_name (B_NETWORK b,char *bus_id) {
    BUSINESS b2 = get_business(b,bus_id);
    char *bus_nome = get_nameb(b2);
    destroy_business(b2);
    return bus_nome;
}


void insert_businessCity(B_NETWORK bnet,BUSINESS b,char *city) {
    char *bus_city = get_city(b);
    if (strcmp(city,bus_city) == 0) {
        insert_business(bnet,b);
    }
    free(bus_city);
}

gboolean fillBusinessArrayCategory(gpointer key, gpointer value, gpointer data) {
    BUSINESS  b    = ((BUSINESS)  value);
    RVarray   r    = ((RVarray)    data);
    RV  acc        = get_RVarray_index(r,0);
    char* category = get_RV_name(acc), *name = get_categorie(b);
    if(checkWordText(category,name)) {
        char* bname = get_nameb(b), *id = get_id(b);
        RV new      = init_RV(id,bname,0,0);
        add_RVarray(data, new);
        free(bname);
        free(id);
    }
    free(category);
    free(name);
    return FALSE;
}


RVarray business_category(B_NETWORK b, char *category) {
    RVarray r  = init_RVarray();
    RV acc     = init_RV(category,category,1,1);
    add_RVarray(r,acc);
    g_tree_foreach(b->business, fillBusinessArrayCategory, r);
    g_array_remove_index_fast (r,0);
    return r;
}
