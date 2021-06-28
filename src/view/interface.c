#include <stdio.h>
#include "../../Includes/view/interface.h"

void showMessage(char *s) {
	printf("%s",s);
}

void showError (char *s) {
	printf("\033[0;31m");
	printf("%s",s);
	printf("\033[0m");
}


void printEmptyLine() {
    printf("|                                                                            |\n");
}

void manload() {
    printf("------------------------------------------------------------------------------\n");
    printf("| Query 1:load_sgr(char *FPU,char *FPB,char *FPR)                            |\n");
	printf("| Inicializa o SGR com o conteudo dos ficheiros                              |\n");
    printf("------------------------------------------------------------------------------\n");
}

void manQ2() {
    printf("------------------------------------------------------------------------------\n");
    printf("| Query 2:businesses_started_by_letter(SGR sgr,char letter)                  |\n");
	printf("| Coloca numa tabela todos os negocios cujo nome comece por dada letra       |\n");
    printf("------------------------------------------------------------------------------\n");
}

void manQ3() {
    printf("------------------------------------------------------------------------------\n");
	printf("| Query 3:business_info(SGR , bus_id)                                        |\n");
	printf("| Coloca numa tabela a informacao sobre o negocio                            |\n");
    printf("------------------------------------------------------------------------------\n");
}

void manQ4() {
    printf("------------------------------------------------------------------------------\n");
    printf("| Query 4:businesses_reviewed(SGR, user_id)                                  |\n");
	printf("| Coloca numa tabela todos os negocios avaliados pelo user                   |\n");
    printf("------------------------------------------------------------------------------\n");
}

void manQ5() {
    printf("------------------------------------------------------------------------------\n");
    printf("| Query 5:businesses_with_stars_and_city(SGR,stars,city)                     |\n");
	printf("| Coloca numa tabela os negocios da cidade com uma media superior a stars    |\n");
    printf("------------------------------------------------------------------------------\n");
}

void manQ6() {
    printf("------------------------------------------------------------------------------\n");
    printf("| Query 6:top_businesses_by_city(SGR,top)                                    |\n");
	printf("| Coloca numa tabela os top negocios de todas as cidades                     |\n");
    printf("------------------------------------------------------------------------------\n");
}

void manQ7() {
    printf("------------------------------------------------------------------------------\n");
    printf("| Query 7: international_users(SGR)                                          |\n");
    printf("| Coloca numa tabela os users que fizeram uma review em mais que um estado   |\n");
    printf("------------------------------------------------------------------------------\n");
}

void manQ8() {
    printf("------------------------------------------------------------------------------\n");
    printf("| Query 8: top_businesses_with_category(SGR,top,category)                    |\n");
	printf("| Coloca numa tabela os top negocios com dada categoria                      |\n");
    printf("------------------------------------------------------------------------------\n");
}

void manQ9() {
    printf("------------------------------------------------------------------------------\n");
    printf("| Query 9: reviews_with_word(SGR,top,word)                                   |\n");
    printf("| Coloca numa tabela todas as reviews com a palavra indicada no seu texto    |\n");
    printf("------------------------------------------------------------------------------\n");
}

void menu() {
    printf("-----------------------------------------------------------------------------|\n");
    printf("|             Bem vindo ao Sistema de Gestão de Recomendações SGR            |\n");
    printf("-----------------------------------------------------------------------------|\n");
    printf("|Instruções:                                                                 |\n");
    printEmptyLine();
    printf("|Escrever a funcionalidade desejada no interpretador->ex:load_sgr(files)     |\n");
    printEmptyLine();
    printf("|Pode carregar os friends nos users utilizando o comando loadFriends()       |\n");
    printEmptyLine();
    printf("|Digite 'man' no interpretador para ver todas as funcionalidades do programa |\n");
    printEmptyLine();
    printf("|Para sair do programa digite 'quit' no interpretador.                       |\n");
    printEmptyLine();
    printf("|Have fun :)!                                                                |\n");
    printEmptyLine();
    printf("------------------------------------------------------------------------------\n");
}


void manuel() {
	clear();
    printf("------------------------------------------------------------------------------\n");
    printf("|       Bem vindo ao Manuel do Sistema de Gestão de Recomendações SGR        |\n");
    printf("------------------------------------------------------------------------------\n");
    printf("| Funcionalidades:                                                           |\n");
    printEmptyLine();
    printf("| Query 1:load_sgr(char *FPU,char *FPB,char *FPR)                            |\n");
    printEmptyLine();
    printf("| Query 2:businesses_started_by_letter(SGR sgr,char letter)                  |\n");
    printEmptyLine();
	printf("| Query 3:business_info(SGR )                                                |\n");
    printEmptyLine();
    printf("| Query 4:businesses_reviewed(SGR, user_id)                                  |\n");
    printEmptyLine();
    printf("| Query 5:businesses_with_stars_and_city(SGR,stars,city)                     |\n");
    printEmptyLine();
    printf("| Query 6:top_businesses_by_city(SGR,top)                                    |\n");
    printEmptyLine();
    printf("| Query 7: international_users(SGR)                                          |\n");
    printEmptyLine();
    printf("| Query 8: top_businesses_with_category(SGR,top,category)                    |\n");
    printEmptyLine();
    printf("| Query 9: reviews_with_word(SGR,top,word)                                   |\n");
    printEmptyLine();
    printf("-----------------------------------------------------------------------------\n\n");
    printEmptyLine();
}

void clear() {
    printf("\e[1;1H\e[2J");
}
