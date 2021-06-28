#ifndef CATALOGOR_H
#define CATALOGOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmodule.h>
#include "../../Includes/Model/reviews.h"
#include "../../Includes/Model/AuxStructs.h"
#include "../../Includes/Model/glibWarningAvoid.h"
#include "../../Includes/Model/data.h"
#include "../../Includes/Model/AuxStructs.h"
#include "business_network.h"
#include "catalogoU.h"

//Apontador para a estrutura do catalogo das reviews.
typedef struct catalogo *catReview;

//Iterador do catalogo das reviews.
typedef GHashTableIter catIT;

//Inicializa o catalogo das reviews.
catIT initcatIT (catIT iter,catReview c);

//Coloca na review uma cópia da proxima review do catalogo e incrementa o iterador.
gboolean catIT_next (catIT *iter,REVIEW *r);

//Funcao que inicializa o catalogo a partir de um ficheiro.
catReview initcatReviewfromFile (char *file,catUser cu,B_NETWORK bn);

//Funcao que insere uma review no catalogo.
int insertcatReview(catReview c,REVIEW r);

//Funcao que retorna o tamanho do catalogo.
int catSize (catReview c);

//Funcao que verifica se o id é uma chave do catalogo e se a review existe.
int catContem (catReview c,char *id);

//Funcao que remove um valor do catalogo.
int catReviewremove (catReview c,char *id);

//Funcao que dá free ao catalogo e a todas as suas reviews.
void destroycatReview (catReview c);

//Funcao que, para o business_id recebido, calcula o número de estrelas e o número de ocorrencias do negocio no catalogo.
RV get_RV_reviewed_from_catalog(catReview c, char *business_id);

//Funcao que completa o HASH_RV com todos os negocios do catalogo.
void set_stars_for_businesses_list(catReview c, HASH_RV a);


#endif