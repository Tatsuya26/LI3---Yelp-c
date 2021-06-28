#include "../../Includes/Model/sgr.h"
#include <time.h>


struct sgr {
    B_NETWORK business;
    catUser users;
    catReview reviews;
};

///////////////////////////////////////////////////////Funcoes AUXILIARES/////////////////////////////////////////////////////


//Funcao que percorre as reviews e adiciona a sua informaçao à tabela se o seu user_id for igual ao dado.
static TABLE buildTable_id_name (char *user_id,SGR sgr,TABLE t) {
	REVIEW r = NULL;
    catIT iter= initcatIT(iter,sgr->reviews);
	char **bus = malloc (sizeof(char *) * 2);
	set_header(t, "business_id;business_name", ";");
	while (catIT_next (&iter,&r) == TRUE) {
        if (strcmp (get_review_user_id(r),user_id) == 0) {
            bus[0] = get_review_business_id (r);
            bus[1] = get_business_name(sgr->business,bus[0]);
            t = add_line_to_TABLE(t,bus);
            free(bus[0]);
            free(bus[1]);
        }
        destroy_review(r);
    }
	free(bus);
	return t;
}

//Funcao que determina se o SGR é valido ou nao.
static int validSGR (SGR s) {
	if (s->business != NULL && s->reviews != NULL && s->users != NULL) return 1;
	else return 0;
}

// função que percorre uma hash table cujo valor é uma tree de RVs
// para fazer addSort numa hash table cujo valor é o array com os melhores negócios numa cidade
static GHashTable* sort_best_businesses_in_city(GHashTable *best, GHashTable *src){
        GHashTableIter it; gpointer key, value;
        g_hash_table_iter_init(&it, best);
        
	GTree *ptT;
        while(g_hash_table_iter_next(&it, &key, &value) == TRUE){
                char *city = (char *) key;
                if((ptT = g_hash_table_lookup(src, city)) != NULL)
                        g_tree_foreach(ptT, (GTraverseFunc) addSort_RVA, (RVA) value);
        }

        return best;
}

// função que preenche o argumento empty com um array de RVs vazios para cada cidade e
// enche outra hash table com uma árvore binária com todos os RVs correspondentes aos negócios numa cidade
static GHashTable* fill_HashTables_with_RV(SGR sgr, int top, GHashTable *toFill, GHashTable **empty){
	GTree *ptT; RVA ptRV; RV rv, tmp; REVIEW r; BUSINESS b = NULL;
	catIT iter = initcatIT(iter, sgr-> reviews);
	
	while(catIT_next(&iter, &r) == TRUE){
		char* business_id = get_review_business_id(r);
		b = get_business(sgr-> business, business_id);
		if(b != NULL){
			char* city = get_city(b);
			rv = to_reviewedBUSINESS(b, get_review_star(r));

			if((ptT = g_hash_table_lookup(toFill, city)) == NULL){
				GTree *T = g_tree_new_full((GCompareDataFunc) strcmp, NULL, (GDestroyNotify) free, (GDestroyNotify) Destroy_RV);
				g_tree_insert(T, strdup(business_id), cloneRV(rv));
				g_hash_table_insert(toFill, strdup(city), T);
			}
			else{
				if((tmp = g_tree_lookup(ptT, business_id)) == NULL)
					g_tree_insert(ptT, strdup(business_id), cloneRV(rv));
				else
					complementRV(tmp, rv);
			}

			if((ptRV = g_hash_table_lookup(*empty, city)) == NULL){
				RVA v = init_empty_arrayRV(top);
				g_hash_table_insert(*empty, strdup(city), v);
			}
			free(city);
			destroy_business(b);
			Destroy_RV(rv);
		}
		free(business_id); destroy_review(r);
	}

	return toFill;
}

static void free_LINE_RVA(char **line){
	for(int j = 0; j < 3; j++)
		free(line[j]);
	free(line);
}

// transforma os RVs não vazios de um array de RVs num array de strings para adicionar à TABLE
static TABLE RVAHashTable_to_TABLE(GHashTable *h, int n){
	TABLE t = alloc_TABLE(1, 3);
	
	GHashTableIter it; gpointer key, value;
	g_hash_table_iter_init(&it, h);
	RVA ptRV;
	t = set_header(t, "user_id;name;stars", ";");
	t = set_col_as_Num(t, 2);
	while(g_hash_table_iter_next(&it, &key, &value)){
		ptRV = (RVA) value;
		int size = actual_size_RVA(ptRV);
		int ini = n - size;
		for(int j = ini; j < n; j++){
			char **line = RVAindex_to_line(ptRV, j);
			t = add_line_to_TABLE(t, line);
			free_LINE_RVA(line);
		}
	}

	return t;
}


// int que sinaliza quando se deve destruir o conteúdo de uma lista
// para evitar que este se perca numa colisão das inserções numa hash table
static int free_list_bool = 0;

static void free_list(gpointer data){
	if(free_list_bool)
		g_list_free_full((GList *) data, (GDestroyNotify) free);
}

static int isUserInHash(GHashTable *h, char *user_id){
	return (g_hash_table_lookup_extended(h, user_id, NULL, NULL) == TRUE);
}

static TABLE add_user_to_TABLE(TABLE t, char *user){
	char **line = malloc(sizeof(char *));
	line[0] = strdup(user);
	t = add_line_to_TABLE(t, line);
	free(line[0]);
	free(line);
	return t;
}

// numa única travessia do catálogo de reviews, por cada user que faça reviews, armazena os
// estado onde as fez (numa lista) e verifica se deve ser acrescentado à TABLE
static TABLE get_international_users(TABLE t, SGR sgr){
	GHashTable *h = g_hash_table_new_full(g_str_hash, g_str_equal, (GDestroyNotify) free, (GDestroyNotify) free_list);
	GHashTable *iUsers = g_hash_table_new_full(g_str_hash, g_str_equal, (GDestroyNotify) free, NULL);
	free_list_bool = 0;
	GList *pt; REVIEW r;
	catIT iter = initcatIT(iter, sgr-> reviews);

	while(catIT_next(&iter, &r) == TRUE){
		char* user_id = get_review_user_id(r);
		char* business_id = get_review_business_id(r);
		BUSINESS b = get_business(sgr-> business, business_id);
		if(b != NULL){
			char* state = get_state(b);
			if((pt = g_hash_table_lookup(h, user_id)) != NULL){
				if(g_list_find_custom(pt, state, (GCompareFunc) strcmp) == NULL){
					pt = g_list_prepend(pt, strdup(state));
					g_hash_table_insert(h, strdup(user_id), pt);
				}
				if(g_list_length(pt) >= 2){
					if(!isUserInHash(iUsers, user_id)){
						g_hash_table_insert(iUsers, strdup(user_id), NULL);
						t = add_user_to_TABLE(t, user_id);
					}
				}
			}
			else{
				GList *b_list = g_list_prepend(NULL, strdup(state));
				g_hash_table_insert(h, strdup(user_id), b_list);
			}
			free(state);
			destroy_business(b);
		}
		free(user_id); free(business_id); 
		destroy_review(r); 
	}
	free_list_bool = 1;
	g_hash_table_destroy(h); g_hash_table_destroy(iUsers);

	return t;
}

//Funcao que cria e retorna um HASH_RV nos quais estao só os negócios de uma dada cidade.
static HASH_RV buildHashwithRVCity(SGR sgr,char *city) {
    HASH_RV hrv = init_Hash_RV();
    REVIEW r = NULL;
    catIT iter = initcatIT (iter,sgr->reviews);
    while (catIT_next (&iter,&r) == TRUE) {
        char *bus_id = get_review_business_id(r);
        BUSINESS b = get_business(sgr->business,bus_id);
        if (b) {
            char *bus_city = get_city(b);
            if (strcmp(bus_city,city) == 0) {
                RV aux = to_reviewedBUSINESS(b,get_review_star(r));
                if (hash_RV_Contem(hrv,bus_id) == TRUE) {
                    RV rv = get_RV_from_Hash (hrv,bus_id);
                    complementRV(rv,aux);
                }
                else insert_hash_RV(hrv,aux);
                Destroy_RV(aux);
            }
            free(bus_city);
        }
            destroy_business(b);
            destroy_review(r);
            free(bus_id);
    }
    return hrv;
}

//Funcao que determina se as reviews contêm a palavra no seu texto e ,se sim, adiciona a sua informaçao à tabela.
static TABLE addReviewsWithWord (TABLE t,SGR sgr,char *word) {
	char **bus = malloc(sizeof(char *) * 1);
    REVIEW r = NULL;
    catIT iter = initcatIT (iter,sgr->reviews);
	set_header(t, "Review_id", ";");
    while (catIT_next (&iter,&r) == TRUE) {
        char *r_text = get_review_text (r);
        if (checkWordText(word,r_text)) {
            bus[0] = get_review_id (r);
            t = add_line_to_TABLE(t,bus);
            free(bus[0]);
        }
        destroy_review(r);
        free(r_text);
    }
    free(bus);
	return t;
}

//Funcao que percorre o HASH_RV e coloca na tabela a informaçao dos negocios com uma media de estrelas superior à dada.
static TABLE buildTABLEWithRV (HASH_RV hrv,SGR sgr,float stars,TABLE t) {
    RV_IT hiter = init_RV_IT (hiter,hrv);
    RV aux = NULL;
    char **bus = malloc(sizeof(char *) * 2);
    while (RV_IT_next(&hiter,&aux) == TRUE) {
        float media = get_RV_stars(aux) / (float) get_RV_nreviews (aux);
        if (media >= stars) {
            bus[0] = get_RV_id(aux);
            BUSINESS b = get_business(sgr->business,bus[0]);
            bus[1] = get_nameb(b);
            t = add_line_to_TABLE(t,bus);
            free(bus[0]);
            free(bus[1]);
            destroy_business(b);
        }
        Destroy_RV(aux);
    }
    return t;
}

///////////////////////////////////////////////////////Funcoes do Modulo SGR/////////////////////////////////////////////////////
//Valor que indica se carregamos os friends ou não.
static int carregaAmigos = 0;

void setEstadoCarrega (int estado) {
    carregaAmigos = estado;
}

int getEstadoCarrega () {
    return carregaAmigos;
}

SGR init_sgr() {
    SGR new = malloc(sizeof(struct sgr));
    return new;
}

void free_sgr(SGR sgr) {
    destroyNetwork  (sgr->business);
    destroy_catUser (sgr->users);
    destroycatReview(sgr->reviews);
    free(sgr);
}

SGR load_sgr(char* users,char* businesses,char* reviews) {
    SGR new = init_sgr();
    //create business catalog
    new->business = load_business_from_file(businesses);
    //create users catalog
    new->users = init_catUser_from_file(users, ";", carregaAmigos);
    //create reviews catalog
    new->reviews = initcatReviewfromFile(reviews,new->users,new->business);
    //Valida SGR
	if (validSGR (new)) 
		return new;
	else{
        free(new);
		return NULL;
	}
}

//QUERIE 2
TABLE businesses_started_by_letter(SGR sgr,char letter) {
    StrArray b   = business_by_letter(sgr->business,letter);
    int length   = getLength(b);
    TABLE t     = alloc_TABLE(length,1);
    t = set_header (t,"Business_name;",";");
    for(int i = 0; i < length; i++)  {
        char  *aux  = getStrElement(b,i);
        char **aux2 = convertArrayCinMatrix(aux);
        add_line_to_TABLE(t,aux2);
        free(aux);
    }
    destroyStrArray(b);
    return t;
}

//QUERIE 3
TABLE business_info(SGR sgr,char* business_id) {
    char *name = NULL, *city = NULL, *state = NULL, *stars = NULL, *reviews = NULL;
    BUSINESS new = get_business(sgr->business,business_id);
    TABLE t      = alloc_TABLE(1,5);
    set_header(t,"Business_name;City;State;Stars;Reviews;",";");
    t = set_col_as_Num(t, 3);
    t = set_col_as_Num(t, 4);
    if (new) {
        RV stars_num = get_RV_reviewed_from_catalog(sgr->reviews,business_id);
        name         = get_nameb(new);
        city         = get_city (new);
        stars        = get_media_stars(stars_num);
        state        = get_state(new);
        reviews      = get_RV_charnReviews(stars_num);
        char **bus   = malloc(sizeof(char *) * 5);
        bus[0]       = name;
        bus[1]       = city;
        bus[2]       = state;
        bus[3]       = stars;
        bus[4]       = reviews;
        add_line_to_TABLE(t,bus);
        free(bus);
        free(name);
        free(city);
        free(state);
        free(stars);
        free(reviews);
    }
    return t;
}

//QUERIE 4
TABLE businesses_reviewed (SGR sgr, char *user_id) {
    TABLE t = alloc_TABLE (2,2);

    //Percorrer catalogo das reviews e inserir na tabela reviews com o mesmo user_id do argumento.
    t = buildTable_id_name (user_id,sgr,t);

    return t;
}

//QUERIE 5
TABLE businesses_with_stars_and_city(SGR sgr,float stars,char *city){
    TABLE t = alloc_TABLE (1,2);
    set_header(t, "business_id;business_name", ";");

    //Criar HASH_RV com todos os negocios da cidade
    HASH_RV hrv = buildHashwithRVCity(sgr,city);

    //Construir TABLE a partir de HASH_RV
    t = buildTABLEWithRV(hrv,sgr,stars,t);
    destroy_hash_RV(hrv);
    return t;
}

//QUERIE 6
TABLE top_businesses_by_city(SGR sgr, int top){
	GHashTable *all = g_hash_table_new_full(g_str_hash, g_str_equal, (GDestroyNotify) free, (GDestroyNotify) g_tree_destroy);
	GHashTable *best = g_hash_table_new_full(g_str_hash, g_str_equal, (GDestroyNotify) free, (GDestroyNotify) destroyRva);

	all = fill_HashTables_with_RV(sgr, top, all, &best);
	best = sort_best_businesses_in_city(best, all);

	TABLE t = RVAHashTable_to_TABLE(best, top);
	g_hash_table_remove_all(all);	
	g_hash_table_destroy(all);
	g_hash_table_destroy(best);
	return t;
}

//QUERIE 7 
TABLE international_users(SGR sgr){ 
	TABLE t = alloc_TABLE(1, 1);
	t = set_header(t, "user_id", ";");
	t = get_international_users(t, sgr);
	return t;
}


//QUERIE 8
TABLE top_businesses_with_category(SGR sgr,int top,char*category) {
    RVarray b_category = business_category(sgr->business,category),aux2 ,final;
    TABLE t  = alloc_TABLE(get_size(b_category),3);
    set_header(t, "business_id;business_name;stars", ";");
    t = set_col_as_Num(t, 2);
    if(get_size(b_category) != 0) {
        HASH_RV acc        = from_RVarray_to_Hash(b_category);
        set_stars_for_businesses_list(sgr->reviews,acc);
        TREE_RV aux        = from_Hash_to_Tree(acc);
        aux2 = from_Tree_to_Array(aux);
        final = drop(aux2,top);
        for(int i = 0; i < top; i++) {
            RV r       = get_RVarray_index(final,i);
            char** tab = from_RV_to_line(r);
            add_line_to_TABLE(t,tab);
        }
        destroy_RVarray(b_category);
        destroy_hash_RV(acc);
        destroy_Tree_RV(aux);
        destroy_RVarray(final);
    }
    return t;
}

//QUERIE 9
TABLE reviews_with_word(SGR sgr,char* word){
    TABLE t = alloc_TABLE(2,1);

    //Percorrer catalogo das reviews e ver se existe a palavra no texto. Se sim,adicionar o id na tabela.
    t = addReviewsWithWord (t,sgr,word);
    
    return t;
}

