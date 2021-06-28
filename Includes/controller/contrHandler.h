#ifndef CONTRHANDLER_H
#define CONTRHANDLER_H 

#include "../Model/sgr.h"
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../view/table.h"
#include"../view/interface.h"

typedef struct varTable *TabVar;



//Funcao que inicializa uma TabVar vazia.
TabVar initTabVar();

//Funcao que insere um valor na tabela.
int insertTabVar (TabVar tv,char *key,TABLE t);

//Funcao que verifica se existe o valor na tabela.
int contemTabVar (TabVar tv, char *key) ;

//Funcao que remove um valor da tabela.
int removeTabVar (TabVar tv,char *key);

//Funcao que destroi uma TabVar.
void freeTabVar (TabVar tv);

//Funcao que retorna a tabela corresponde à variavel.
TABLE getTable (TabVar tv,char *key);

//Funcao que limpa os espaços iniciais e finais duma string.
void cleanSpaces (char *querie);

//Funcao que limpa as quotes iniciais e finais.
void cleanQuotes (char *word);

//Funcao que dado um delimitador retorna uma string equivalente à do argumento ate ao delimitador.
char *getString (char **querie,char *delim,int quotes) ;

//Funcao auxiliar para dar load ao SGR
SGR loadSGR (char **buffer,int estado);

//Funcao auxiliar para correr a querie 2
void runQ2 (char *variavel,char *buffer,TabVar tv,SGR s);

//Funcao auxiliar para correr a querie 3
void runQ3 (TabVar tv,char *buffer,char *variavel,SGR s);

//Funcao auxiliar para correr a querie 4
void runQ4 (TabVar tv,char *buffer,char *variavel,SGR s);

//Funcao auxiliar para correr a querie 5
void runQ5 (TabVar tv,char *buffer,char *variavel,SGR s);

//Funcao auxiliar para correr a querie 6
void runQ6 (TabVar tv,char *buffer,char *variavel,SGR s);

//Funcao auxiliar para correr a querie 7
void runQ7 (TabVar tv,char *buffer,char *variavel,SGR s);

//Funcao auxiliar para correr a querie 8
void runQ8 (TabVar tv,char *buffer,char *variavel,SGR s) ;

//Funcao auxiliar para correr a querie 9
void runQ9 (TabVar tv,char *buffer,char *variavel,SGR s) ;

//Funcao auxiliar para correr o comando toCSV
void runToCSV (TabVar tv,char * buffer) ;

//Funcao auxiliar para correr o comando fromCSV
void runFromCSV (char *buffer,char *variavel,TabVar tv);

//Funcao auxiliar para correr o comando filter
void runFilter(char *buffer,TabVar tv,char *variavel);

//Funcao auxiliar para correr o comando proj
void runProj (char *buffer,TabVar tv,char * variavel);

//Funcao auxiliar para correr o comando var[][]
void runGetValue (TabVar tv,char *buffer,char *variavel,char *q);

//Funcao auxiliar para correr o comando show
void runShow(TabVar tv,char *buffer) ;

//Funcao auxiliar para remover um valor da tabela
void runRemove(TabVar tv,char *buffer);

//Funcao auxiliar que corre o manual.
void runManual();

#endif