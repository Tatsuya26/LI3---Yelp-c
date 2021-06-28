#ifndef REVIEWS_H
#define REVIEWS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Includes/Model/data.h"
#include "catalogoU.h"
#include "business_network.h"

typedef struct review *REVIEW;

//Funçao que inicia uma review com os seus campos necessários.
REVIEW init_review (char *id,char *user,char *business,float stars,int useful,int funny,int cool,data data,char *text);

//Funcao que inicia uma review a partir de uma linha de texto.
REVIEW init_review_fromline (char *line,char *separator,catUser cu,B_NETWORK bn);

//Funcao que retorna o id da review.
char *get_review_id (REVIEW r);

//Funcao que retorna o user_id da review.
char *get_review_user_id (REVIEW r);

//Funcao que retorna o business_id da review.
char *get_review_business_id (REVIEW r);

//Funcao que retorna o numero de estrelas da review.
float get_review_star (REVIEW r);

//Funcao que retorna o campo cool da review.
int get_review_cool (REVIEW r);

//Funcao que retorna o campo useful da review.
int get_review_useful (REVIEW r);

//Funcao que retorna o campo funny da review.
int get_review_funny (REVIEW r);

//Funcao que retorna a data da review.
data get_review_data (REVIEW r) ;

//Funcao que retorna o texto da review.
char *get_review_text (REVIEW r);

//Funcao que retorna toda a informacao da review numa String.
char* get_review_info (REVIEW r);

//Funcao que altera o id da review.
int set_review_id (REVIEW r,char *id);

//Funcao que altera o user_id da review.
int set_review_user_id (REVIEW r, char *id);

//Funcao que altera o business_id da review.
int set_review_business_id (REVIEW r,char *id);

//Funcao que altera o numero de estrelas da review.
int set_review_stars (REVIEW r, float star);

//Funcao que altera o campo cool da review.
int set_review_cool (REVIEW r, int cool);

//Funcao que altera o campo funny da review.
int set_review_funny (REVIEW r, int funny);

//Funcao que altera o campo useful da review.
int set_review_useful (REVIEW r, int useful);

//Funcao que altera a data da review.
int set_review_data (REVIEW r,data d);

//Funcao que altera o texto da review.
int set_review_text (REVIEW r,char *text);

//Funcao que clona uma review.
REVIEW clone_review (REVIEW r);

//Funcao que destroi e dá free a uma review.
void destroy_review (REVIEW r);


#endif