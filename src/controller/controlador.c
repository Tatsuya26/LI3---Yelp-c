#include "../../Includes/controller/controlador.h"


char *rl_completer_word_break_characters =", \t\n\"\\'`@$><=;|&{(";


void controlador () {
    int exitF = 0,carregaFriends = 0;
    using_history();
    SGR s = NULL;
    clear();
    menu();
    TabVar tv = initTabVar();
    while (!exitF) {
        //Mostrar algum tipo de menu ou informaçao
        char *buffer = malloc(sizeof(char) *200);
        buffer = readline("Interpretador>");
        if (buffer && *buffer) add_history(buffer);
        while (buffer && strlen(buffer) > 0 && !exitF) {
        //char *buffer = getString(&buffer,";",0);
        //char *buffer = buffer;
        char *q ,*args,*variavel;
        char *tmp = malloc(sizeof(char) *100);
        int     vari = 0,i = 0;
        while (buffer[i] != '=' && buffer[i] != '\0' && buffer[i] != ';') {
            tmp[vari++] = buffer[i++];
        }
        
        if (buffer[i] == '=') {i++;tmp[vari] = '\0';variavel = getString(&tmp,"=",0);}
        else {variavel = NULL;i = 0;} 
        free(tmp);tmp = malloc(sizeof(char) *100);
        int qi = 0;
        while (buffer[i] != '(' && buffer[i] != '[' && buffer[i] != '\0' && buffer[i] != ';') {
                tmp[qi++] = buffer[i++]; 
        }

        if (buffer[i] == '(') {tmp[qi] = '\0';i++;q = getString(&tmp,"(",0);}
        else if (buffer[i] == '[') {tmp[qi] = '\0';i++;q = getString(&tmp,"[",0);}
        else {tmp[qi] = '\0';q = getString(&tmp,"(",0);}
        int argi = 0,index = 0;
        free(tmp);tmp = malloc(sizeof(char) *100);
        while (buffer[i] != ')' && index != 2 && buffer[i] != '\0') {
                if (buffer[i] == ']') index++;
                tmp[argi++] = buffer[i++]; 
        }
        if (buffer[i] == ')') {tmp[argi++] = ')';tmp[argi] = '\0';args = getString(&tmp,"\0",0);}
        else if (buffer[--i] == ']') {tmp[argi] = '\0';args = getString(&tmp,"\0",0);}
        else args = NULL;
        while (buffer[i] != '\0' && buffer[i] != ';') i++;
        if (buffer[i] == ';') {buffer = &(buffer[++i]);}
        else {buffer = NULL;}
        free(tmp);
        if (strcmp(q,"quit") == 0) exitF = 1;
        else if (strcmp(q,"load_sgr") == 0) {
            if (s != NULL) free_sgr(s);
            s = loadSGR(&args,carregaFriends);
            if (s) showMessage("SGR carregado com sucesso!\n");
        }

        else if (strcmp(q,"businesses_started_by_letter") == 0) {
            if (!s) showError("SGR ainda nao foi inicializado\n");
            else runQ2(variavel,args,tv,s);
        }

        else if (strcmp(q,"business_info") == 0) {
            if (!s) showError("SGR ainda nao foi inicializado\n");
            else runQ3(tv,args,variavel,s);
        }

        else if (strcmp(q,"businesses_reviewed") == 0) {
            if (!s) showError ("SGR ainda nao inicializado!\n");
            else runQ4(tv,args,variavel,s);
        }

        else if (strcmp(q,"businesses_with_stars_and_city") == 0) {
            if (!s) showError ("SGR ainda nao inicializado!\n");
            else runQ5(tv,args,variavel,s);
        }

        else if (strcmp(q,"top_businesses_by_city") == 0) {
            if (!s) showError ("SGR ainda nao inicializado!\n");
            else runQ6(tv,args,variavel,s);
        }

        else if (strcmp(q,"international_users") == 0) {
            if (!s) showError ("SGR ainda nao inicializado!\n");
            else runQ7(tv,args,variavel,s);
        }

        else if (strcmp(q,"top_businesses_with_category") == 0) {
            if (!s) showError ("SGR ainda nao inicializado!\n");
            else runQ8(tv,args,variavel,s);
        }

        else if (strcmp(q,"reviews_with_word") == 0) {
            if (!s) showError ("SGR ainda nao inicializado!\n");
            else runQ9(tv,args,variavel,s);
        }

        else if (strcmp (q,"toCSV") == 0) {
            runToCSV (tv,args);
        }

        else if (strcmp(q,"fromCSV") == 0) {
            runFromCSV(args,variavel,tv);
        }

        else if (strcmp(q,"filter") == 0) {
            runFilter(args,tv,variavel);
        }

        else if (strcmp(q,"proj") == 0) {
            runProj(args,tv,variavel);
        }


        else if (strcmp(q,"show") == 0) {
            runShow(tv,args);
        }

        else if (strcmp(q,"remove") == 0) {
            runRemove(tv,args);
        }

        else if (contemTabVar(tv,q) && args != NULL) {
            runGetValue (tv,args,variavel,q);
        }

        else if (strcmp(q,"man") == 0) {
            runManual();
        }

        else if (strcmp(q,"loadFriends") == 0) {
            if (strcmp(args,")") == 0) {
                if (carregaFriends) carregaFriends = 0;
                else carregaFriends = 1;

                if (carregaFriends) showMessage("SGR vai carregar os friends!\n");
                else showMessage ("SGR vai deixar de carregar os friends!\n");
            }
            else showError("Argumentos invalidos!\n");
        }


        else showError("buffer não reconhecido. Help para ver os buffers disponiveis!\n");
        }
    }
    freeTabVar(tv);
    rl_clear_history();
    if (s) free_sgr(s);
}
