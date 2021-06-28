#ifndef BUSINESS_NETWORK_H
#define BUSINESS_NETWORK_H

#include "business.h"
#include "glibWarningAvoid.h"
#include "AuxStructs.h"


/*
* Estrutura de dados àrvore binária de procura que aramazena em cada nodo
* uma estrutura de business
*/
typedef struct business_network* B_NETWORK;

/*
* Função de comparação que serve como critério para ordenação da àrvore
* Utiliza a string compare como resultado para ordenar
*/
gint compare_business(gconstpointer a, gconstpointer b);

/*
*Inicia uma estrutura de dados àrvore com struct business
*/
B_NETWORK init_business_network();

/*
* Função que carrega para memória a informação de uma arvore de negócios
*/
B_NETWORK load_business_from_file(char* filename);

/*
*   Função que calcula a dimensão de uma àrvore de business
*/
int get_networkSize(B_NETWORK b);

/*
* Liberta a memória alocada por àrvore de business
*/
void destroyNetwork(B_NETWORK b);

/*
* Verifica se uma um negócio existe numa àrvore
*/
BOOLEAN exist_business(B_NETWORK b, BUSINESS new);

/*
*   Função que devolve uma cópia de um negócio que exisga num àrvore
*/
BUSINESS get_business(B_NETWORK b, char* business_id);

/*
*  Percorre a àrvore de negócios e acumula num array todos os negócios começados pela letra dada
*/
StrArray business_by_letter(B_NETWORK b , char letter);

/*
*   Devolve o nome do negócio
*/
char *get_business_name (B_NETWORK b,char *bus_id);

/*
*   Insere uma struct business numa àrvore de negócios
*/
void insert_business (B_NETWORK b,BUSINESS bus);

/*
*   Preenche um RVarray com todas as business que tem a categoria pretendida
*/
RVarray business_category(B_NETWORK b, char *category);

#endif