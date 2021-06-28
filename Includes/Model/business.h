#ifndef BUSINESS_H
#define BUSINESS_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

typedef struct business* BUSINESS;

/*
* Função que inicia uma struct business
*/
BUSINESS init_business(char* id,char* name, char* city, char* state,char* categories);

/*
* Função que inicia uma struct business apartir de uma string line em que os parametros estão separados pelo separator
*/
BUSINESS init_business_fromline(char* line, char* separator);


 ////////////////////////////////////////Funções de get ///////////////////////////////
/*
*   Retorna o business_id de uma struct business
*/
char* get_id(BUSINESS b);

/*
*   Retorna o business name de uma struct business
*/
char* get_nameb(BUSINESS b);

/*
*   Retorna cidade de uma struct business
*/
char* get_city(BUSINESS b);

/*
*   Retorne o estado de uma struct business
*/
char* get_state(BUSINESS b);

/*
*   Retorne as categorias de uma struct business
*/
char* get_categorie(BUSINESS b);

/*
*   Retorne uma string com toda a informação de uma struct business b
*/
char* get_business_info(BUSINESS b);


///////////////////////////////////////////////Funções de set//////////////////////////////////////////
/*
*   Coloca numa struct business b um business_id id
*/
void set_id(BUSINESS b, char* id);

/*
*   Coloca numa struct business b um business name name
*/
void set_nameb(BUSINESS b, char* name);

/*
*   Coloca numa struct business b uma cidade city
*/
void set_city(BUSINESS b, char* city);

/*
*   Coloca numa struct business b um estado state
*/
void set_state(BUSINESS b, char* state);

/*
*   Coloca numa struct business b um conjunto de categorias categories
*/
void set_categories(BUSINESS b, char* categories);


/////////////////////////////////////////////////////// Usefull functions ////////////////////////////////////////////////////////////////////
/*
* Devolve a copia de uma struct business b
*/
BUSINESS clone_business(BUSINESS b);

/*
* Liberta a memória alocada por uma struct business b
*/
void destroy_business(BUSINESS b);

#endif