#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct data *data;

//Funcao que inicializa uma data com os 6 campos necessarios.
data init_data (int ano,int mes,int dia,int hora,int min,int seg);

//Funcao que inicializa uma data a partir de uma linha de texto.
data init_data_fromtext (char *line);

//Funcao que retorna o ano.
int get_ano (data d);

//Funcao que retorna o mes.
int get_mes (data d);

//Funcao que retorna o dia.
int get_dia (data d);

//Funcao que retorna a hora.
int get_hora (data d);

//Funcao que retorna o minuto.
int get_min (data d);

//Funcao que retorna o segundo.
int get_seg (data d);

//Funcao que retorna a data como string.
char *get_data (data d);

//Funcao que altera o ano.
int set_ano (data d,int ano);

//Funcao que altera o mes.
int set_mes (data d,int mes);

//Funcao que altera o dia.
int set_dia (data d,int dia);

//Funcao que altera a hora.
int set_hora (data d,int hora);

//Funcao que altera o minuto.
int set_min (data d,int min);

//Funcao que altera o segundo.
int set_seg (data d,int seg);

//Funcao que duplica a data.
data clone_data (data d);

//Funcao que destroi e dá free a data.
void destroy_data (data d);

#endif
