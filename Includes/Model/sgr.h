#ifndef SGR_H
#define SGR_H

#include "business_network.h"
#include "catalogoR.h"
#include "catalogoU.h"
#include "../view/table.h"
#include "AuxStructs.h"


typedef struct sgr * SGR;

//Inicializa o SGR vazio;
SGR init_sgr();

//Funcao que altera se carregamos os friends ou nao.
void setEstadoCarrega (int estado);

//Funcao que retorna se estamos a carregar os friends ou nao.
int getEstadoCarrega ();

//Funcao que destroi o SGR.
void free_sgr(SGR sgr);

/* query 1 -> Carrega o SGR a partir dos ficheiros*/
SGR load_sgr(char*users,char*businesses,char*reviews);

/* query 2 */
TABLE businesses_started_by_letter(SGR sgr,char letter);

/* query 3 */
TABLE business_info(SGR sgr,char*business_id);

/* query 4 -> Coloca numa tabela todos os negocios avaliados pelo user*/
TABLE businesses_reviewed (SGR sgr, char *user_id);

/* query 5 -> Coloca numa tabela todos os negocios da cidade com uma média de estrelas superior a dada*/
TABLE businesses_with_stars_and_city(SGR sgr,float stars,char *city);

/* query 6 */
TABLE top_businesses_by_city(SGR sgr, int top);

/* query 7 */
TABLE international_users(SGR sgr);

/* query 8 */
TABLE top_businesses_with_category(SGR sgr,int top,char* category);

/* query 9 -> Coloca numa tabela todos as reviews com a palavra no seu campo texto*/
TABLE reviews_with_word(SGR sgr,char* word);

#endif
