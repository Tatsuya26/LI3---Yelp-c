#include "../../Includes/controller/contrHandler.h"

//Estrutura que armazena as tabelas com o nome da variavel
struct varTable
{
    GHashTable *tabelas;
};

//Funcao de hash
static guint hashF (gconstpointer key) {
	guint hash = g_str_hash(key);
	return hash;
}

//Funcao de comparacao
static gboolean my_strcmp (gconstpointer a,gconstpointer b) {
	int r = strcmp ((char *) a,(char *) b);
	return !r;
}

TabVar initTabVar() {
    TabVar new = malloc(sizeof(struct varTable));
    new->tabelas = g_hash_table_new_full ((GHashFunc)hashF,(GEqualFunc) my_strcmp ,(GDestroyNotify) free, (GDestroyNotify) destroy_TABLE);
    return new;
}

int insertTabVar (TabVar tv,char *key,TABLE t) {
    return g_hash_table_insert (tv->tabelas,strdup(key),t);
}

int contemTabVar (TabVar tv, char *key) {
    return g_hash_table_contains(tv->tabelas,key);
}

int removeTabVar (TabVar tv,char *key) {
    return g_hash_table_remove(tv->tabelas,key);
}

void freeTabVar (TabVar tv) {
    g_hash_table_destroy (tv->tabelas);
    free(tv);
}

TABLE getTable (TabVar tv,char *key) {
    TABLE t = (TABLE) g_hash_table_lookup(tv->tabelas,key);
    return t;
}

void cleanSpaces (char *querie) {
    int c ,j;
    for (c = 0;querie[c] == ' ';) { 
        j = 0;
        while (querie[j] != '\0')  {querie[j] = querie [j+1];j++;}
    }
    c = strlen(querie) -1;
    while (querie[c] == ' ') { 
        j = c;
        while (querie[j] != '\0')  {querie[j] = querie [j+1];j++;}
        c--;
    } 
}   

void cleanQuotes (char *word) {
    int c = 0,j;
    if (word[c] == '"') {
        j = c;
        while (word[j] != '\0')  {word[j] = word [j+1];j++;}
    }
    c = strlen(word) -1;
    if (word[c] == '"') {
        word[c] = '\0';
    }
}

char *getString (char **querie,char *delim,int quotes) {
    char *ret = strsep(querie,delim);
    cleanSpaces(ret);
    if (quotes) cleanQuotes(ret);
    return strdup(ret);
}

SGR loadSGR (char **buffer,int estado) {
    SGR s = init_sgr();
    char *arg1 = NULL,*arg2 = NULL ,*arg3 = NULL ;
    if (*buffer) arg1 = getString(buffer,",",1);
    if (*buffer) arg2 = getString(buffer,",",1);
    if (*buffer) {
        arg3 = getString(buffer,")",1);
        if (*buffer) {
            setEstadoCarrega(estado);
            s = load_sgr(arg1,arg2,arg3);
            if (!s) showError("Erro na criacao do SGR!\n");
        }
        else showError("Erros nos argumentos\n");
    }
    else showError ("Argumentos insuficientes ou mal separados\n");
    free(arg1);free(arg2);free(arg3);
    return s;
}

void runQ2 (char *variavel,char *buffer,TabVar tv,SGR s) {
    if (!variavel) showError("Sem variavel para atribuir o valor\n");
            else {
                char *arg1 = NULL,*arg2 = NULL;
                if (buffer) arg1 = getString(&buffer,",",0);
                if (buffer) {
                    arg2 = getString(&buffer,")",1);
                    if (buffer) {
                        if (strcmp(arg1,"sgr") != 0) showError ("SGR nao atribuido\n");
                        else if (strlen(arg2) > 1 || !isalpha(*arg2)) showError ("Segundo argumento é somente uma letra!\n");
                        else {
                            TABLE t = businesses_started_by_letter(s,*arg2);
                            insertTabVar(tv,variavel,t);
                            showMessage("Querie 2 relizada com sucesso!\n");
                        }
                    }
                    else showError("Erro nos argumentos!\n");
                }
                else showError("Erro nos argumentos!\n");
                free(arg1);free(arg2);
            }
}

void runQ3 (TabVar tv,char *buffer,char *variavel,SGR s) {
    if (!variavel) showError("Sem variavel para atribuir o valor\n");
    else {
        char *arg1 = NULL,*arg2 = NULL;
        if (buffer) arg1 = getString(&buffer,",",0);
        if (buffer) {
            arg2 = getString(&buffer,")",1);
            if (buffer) {
                if (strcmp(arg1,"sgr") != 0) showError ("SGR nao atribuido\n");
                else if (strlen(arg2) == 0) showError("Business ID vazio!");
                else {
                    TABLE t = business_info(s,arg2);
                    insertTabVar(tv,variavel,t);
                    showMessage("Querie 3 relizada com sucesso!\n");
                }
            }
            else showError("Erro nos argumentos!\n");
        }
        else showError("Erro nos argumentos!\n");
        free(arg1);free(arg2);
    }
}

void runQ4 (TabVar tv,char *buffer,char *variavel,SGR s) {
    if (!variavel) showError("Sem variavel para atribuir o valor\n");
    else {
        char *arg1 = NULL,*arg2 = NULL;
        if (buffer) arg1 = getString(&buffer,",",0);
        if (buffer) {
            arg2 = getString(&buffer,")",1);
            if (buffer) {
                if (strcmp(arg1,"sgr") != 0) showError ("SGR nao atribuido\n");
                else if (strlen(arg2) == 0) showError("USER ID vazio!");
                else {
                    TABLE t = businesses_reviewed(s,arg2);
                    insertTabVar(tv,variavel,t);
                    showMessage("Querie 4 relizada com sucesso!\n");
                }
            }
            else showError("Erro nos argumentos\n");
        }
        else showError("Erro nos argumentos!\n");
        free(arg1);free(arg2);
    }
}

void runQ5 (TabVar tv,char *args,char *variavel,SGR s) {
    if (!variavel) showError("Sem variavel para atribuir o valor\n");
    else {
        char *arg1 = NULL,*arg2 = NULL,*arg3 = NULL;
        if (args) arg1 = getString(&args,",",0);
        if (args) arg2 = getString(&args,",",0);
        if (args) {
            float stars = atof(arg2);
            arg3 = getString(&args,")",1);
            if (args) {
                if (strcmp(arg1,"sgr") != 0) showError ("SGR nao atribuido\n");
                else if (!checkNumericalString(arg2) || stars < 0 || stars > 5) showError("Numero de stars invalido!\n");
                else if (strlen(arg3) < 1) showError("Cidade invalida!\n"); 
                else {
                    TABLE t = businesses_with_stars_and_city(s,stars,arg3);
                    insertTabVar(tv,variavel,t);
                    showMessage("Querie 5 relizada com sucesso!\n");
                }
            }
            else showError("Erro nos argumentos!\n");
        }
        else showError("Erro nos argumentos!\n");
        free(arg1);free(arg2);free(arg3);
    }
}

void runQ6 (TabVar tv,char *buffer,char *variavel,SGR s) {
    if (!variavel) showError("Sem variavel para atribuir o valor\n");
    else {
        char *arg1 = NULL,*arg2 = NULL;
        if (buffer) arg1 = getString(&buffer,",",0);
        if (buffer) {
            arg2 = getString(&buffer,")",0);
            if (buffer) {
                int top = atoi (arg2);
                if (strcmp(arg1,"sgr") != 0) showError ("SGR nao atribuido\n");
                else if (!checkNumericalString(arg2) || top <= 0) showError("Inteiro invalido!\n");
                else {
                    TABLE t = top_businesses_by_city(s,top);
                    insertTabVar(tv,variavel,t);
                    showMessage("Querie 6 relizada com sucesso!\n");
            }
        }
            else showError("Erro nos argumentos!\n");
        }
        else showError("Erro nos argumentos!\n");
        free(arg1);free(arg2);
    }
}

void runQ7 (TabVar tv,char *buffer,char *variavel,SGR s) {
    if (!variavel) showError("Sem variavel para atribuir o valor\n");
    else {
        char *arg1 = NULL;
        if (buffer){ 
            arg1 = getString(&buffer,")",0);
            if (buffer) {
                if (strcmp(arg1,"sgr") != 0) showError ("SGR nao atribuido\n");
                else {
                    TABLE t = international_users(s);
                    insertTabVar(tv,variavel,t);
                    showMessage("Querie 7 relizada com sucesso!\n");
            }
            }
            else showError("Erros nos argumentos!\n");
        }
        else showError("Erro nos argumentos!\n");
        free(arg1);
    }
}

void runQ8 (TabVar tv,char *buffer,char *variavel,SGR s) {
    if (!variavel) showError("Sem variavel para atribuir o valor\n");
    else {
        char *arg1 = NULL,*arg2 = NULL,*arg3 = NULL;
        if (buffer) arg1 = getString(&buffer,",",0);
        if (buffer) arg2 = getString(&buffer,",",0);
        if (buffer) {
            arg3 = getString(&buffer,")",1);
            if (buffer) {
                int top = atoi(arg2);
                if (strcmp(arg1,"sgr") != 0) showError ("SGR nao atribuido\n");
                else if (!checkNumericalString(arg2)|| top <= 0) showError("Inteiro invalido!\n");
                else if (strlen(arg3) < 1) showError("Categoria invalida!\n"); 
                else {
                    TABLE t = top_businesses_with_category(s,top,arg3);
                    insertTabVar(tv,variavel,t);
                    showMessage("Querie 8 relizada com sucesso!\n");
                }
            }
            else showError("Erro nos argumentos!\n");
        }
        else showError("Erro nos argumentos!\n");
        free(arg1);free(arg2);free(arg3); 
    }
}

void runQ9 (TabVar tv,char *buffer,char *variavel,SGR s) {
    if (!variavel) showError("Sem variavel para atribuir o valor\n");
    else {
        char *arg1 = NULL,*arg2 = NULL;
        if (buffer) arg1 = getString(&buffer,",",0);
        if (buffer) {
            arg2 = getString(&buffer,")",1);
            if (buffer) {
                if (strcmp(arg1,"sgr") != 0) showError ("SGR nao atribuido\n");
                else if (strlen(arg2) < 1) showError("Palavra invalida!\n");
                else {
                    TABLE t = reviews_with_word(s,arg2);
                    insertTabVar(tv,variavel,t);
                    showMessage("Querie 9 relizada com sucesso!\n");
                }
            }
            else showError("Erro nos argumentos!\n");
        }
        else showError("Erro nos argumentos!\n");
        free(arg1);free(arg2);
    }
}

void runToCSV (TabVar tv,char * buffer) {
        char *arg1 = NULL,*arg2 = NULL,*arg3 = NULL;
        if (buffer) arg1 = getString(&buffer,",",0);
        if (buffer) arg2 = getString(&buffer,"\"",0);
        if (buffer) arg2 = getString(&buffer,"\"",0);
        if (buffer) arg3 = getString(&buffer,",",0);
        if (buffer) {
            arg3 = getString(&buffer,")",1);
            if (buffer) {
                if (!contemTabVar(tv,arg1)) showError ("Variavel nao existente!\n");
                else if (strlen (arg2) <= 0) showError("Delimitador invalido!\n");
                else if (strlen(arg3) < 1) showError("FilePath invalido!\n"); 
                else {
                    TABLE t = getTable(tv,arg1);
                    toCSV(t,arg2,arg3);
                    showMessage("Escrita para ficheiro realizada com sucesso!\n");
                }
            }
            else showError("Erro nos argumentos!\n");
        }
        else showError("Erro nos argumentos!\n");
        free(arg1);free(arg2);free(arg3);
}

void runFromCSV (char *buffer,char *variavel,TabVar tv) {
    if (!variavel) showError("Sem variavel para atribuir o valor\n");
    else {
        char *arg1 = NULL,*arg2 = NULL;
        if (buffer) arg1 = getString(&buffer,",",1);
        if (buffer) {
            arg2 = getString(&buffer,")",1);
            if (buffer) {
                if (strlen (arg1) < 1) showError ("FilePath invalido\n");
                else if (strlen(arg2) < 1) showError("Delim invalido!\n");
                else {
                    TABLE t = fromCSV(arg1,arg2);
                    if (t) {
                        insertTabVar(tv,variavel,t);
                        showMessage("Leitura do CSV realizada com sucesso!\n");
                    }
                    else showError("Erro na criacao da Table");
                }
            }
            else showError("Erro nos argumentos!\n");
        }
        else showError("Erro nos argumentos!\n");
        free(arg1);free(arg2);
    }
}

void runFilter(char *buffer,TabVar tv,char *variavel) {
    if (!variavel) showError("Variavel nao atribuida!\n");
    else if (buffer) {
        char *arg1 = NULL,*arg2 = NULL,*arg3 = NULL,*arg4 = NULL;
        arg1 = getString(&buffer,",",0);
        if (buffer) arg2 = getString(&buffer,",",1);
        if (buffer) arg3 = getString (&buffer,",",1);
        if (buffer) arg4 = getString(&buffer,")",1);
        if (buffer) {
            if (contemTabVar(tv,arg1) && strlen(arg2) > 0 && strlen(arg3) > 0) { 
                TABLE t = getTable(tv,arg1);
                if (strcmp(arg4,"LT") == 0) {
                    TABLE ret = filter(t,arg2,arg3,LT);
                    insertTabVar(tv,variavel,ret);
                    showMessage("Filter realizado com sucesso!\n");
                }
                else if (strcmp(arg4,"EQ") == 0) {
                    TABLE ret = filter(t,arg2,arg3,EQ);
                    insertTabVar(tv,variavel,ret);
                    showMessage("Filter realizado com sucesso!\n");
                }
                else if (strcmp(arg4,"GT") == 0) {
                    TABLE ret = filter(t,arg2,arg3,GT);
                    insertTabVar(tv,variavel,ret);
                    showMessage("Filter realizado com sucesso!\n");
                }
                else showError("Operador de comparacao nao reconhecido!\n");
                free(arg1);free(arg2);free(arg3);free(arg4);
            }
            else showError("Erros nos argumentos!!\n");
            }
        }
        else showError("Erro no argumento!\n");
}

static void insertSort(int *args,int col,int N) {
    int i , encontrado = 1;
    for (i = -1; i < N-1 && encontrado;i++) {
        if (args[i+1] > col) encontrado = 0;
    }
    if (encontrado) args[N] = col;
    else {
        int j = N;
        while (j > i) {
            args[j] = args[j-1];
            j--;
        }
        args[i] = col;
    }
}

void runProj (char *buffer,TabVar tv,char * variavel) {
    if (!variavel) showError("Variavel nao atribuida!\n");
    else if (buffer) {
        char *arg1 = NULL,*arg2 = NULL;
        int args[10],N = 0;
        arg1 = getString(&buffer,",",0);
        if (buffer) arg2 = getString(&buffer,"{",0);
        if (buffer) arg2 = getString(&buffer,"}",0);
        while (arg2) {
            char *tmp = getString(&arg2,",",0);
            int col = atoi(tmp);
            if (checkNumericalString(tmp) && col > 0) {insertSort(args,col,N);N++;}
            free(tmp);
        }
        if (buffer) {
            arg2 = getString(&buffer,")",0);
            if (buffer) {
                if (contemTabVar(tv,arg1) && N > 0 ) {
                    TABLE t = getTable(tv,arg1);
                    if (validaCol(args,N,t)) {
                        TABLE ret = proj_cols(t,args,N);
                        insertTabVar(tv,variavel,ret);
                        showMessage("Projecao executado com sucesso!\n");
                    }
                    else {
                        showError("Numero das colunas invalidas!\n");
                    }
                }
                else showError("Erros nos argumentos!\n");
            }
            else showError("Erros nos argumentos!\n");
            free(arg1);free(arg2);
        }
            else showError("Erros nos argumentos!\n");
        }
        else showError("Erro no argumento!\n");
}

void runGetValue (TabVar tv,char *args,char *variavel,char *q) {
    if (!variavel) showError("Variavel nao atribuida!\n");
    else if (args) {
        char *arg1 = NULL,*arg2 = NULL;
        arg1 = getString(&args,"]",0);
        if (args) arg2 = getString(&args,"[",0);
        if (args) {
            arg2 = getString(&args,"]",0);
            if (args) {
                int l = atoi(arg1);
                int c = atoi(arg2);
                if (contemTabVar(tv,q) && checkNumericalString(arg1) && checkNumericalString(arg2) ) { 
                    TABLE t = getTable(tv,q);
                    TABLE ret = indexing(t,l,c);
                    if (!ret) showError("Acesso out of bounds!\n");
                    else {
                        insertTabVar(tv,variavel,ret);
                        showMessage("Valor atribuido com sucesso!\n");
                    }
                }
                else showError("Erro na variavel ou nos indexs!!\n");
                free(arg1);free(arg2);
            }
            else showError("Erro nos argumentos!\n");
        }
        else showError("Erro nos argumentos!\n");
            }
        else showError("Erro no argumento!\n");
}

void runShow(TabVar tv,char *buffer) {
    if (buffer) {
        char *arg1 = getString(&buffer,")",0);
        if (buffer) {
            if (contemTabVar(tv,arg1)) { 
            TABLE t = getTable(tv,arg1);
            clear();
            show(t, 1);
            int exitS = 1;
            int pagina  = 1;
            while (exitS) {
            char *page = readline("Insira o numero da pagina/next/previous/leave -> ");
            int invalido =1;
            if (page && *page) add_history(page);
            char *comando = getString(&page,";",0);
            if (strcmp(comando,"leave") != 0) {
                if (strcmp(comando,"next") == 0) { 
                    pagina++;}
                else if (strcmp(comando,"previous") == 0) {if (pagina > 1) pagina--;}
                else if (checkNumericalString(comando)){pagina = atoi(comando);}
                else {
                    showError("Comando nao reconhecido!\n");
                    invalido = 0;
                }
                if (pagina != 0 && invalido)  {
                    clear();
                    show(t,pagina);
                }
            }
            else {exitS = 0;clear();}
            }
            }
            else showError("Variavel nao existente!!\n");
        }
        else showError("Erro no argumento!\n");
        free(arg1);
    }
    else showError("Erros nos argumentos!\n");
}

void runRemove(TabVar tv,char *buffer) {
    if (buffer) {
        char *arg1 = getString(&buffer,")",0);
        if (buffer) {
            if (contemTabVar(tv,arg1)) { 
            removeTabVar(tv,arg1);
            showMessage("Variavel removida com sucesso!\n");
        }
        else showError("Variavel nao existente!!\n");
        }
        else showError("Erro nos argumentos!\n");
        free(arg1);
    }
    else showError("Erro nos argumentos!\n");
}

void runManual() {
    clear();
    manuel();
    char *buffer = malloc(sizeof(char) * 200);
    int exitM = 0;
    while (!exitM) {
        buffer = readline("Digite o nome da querie ou leave -> ");
        if (buffer && *buffer) add_history(buffer);
        if (strcmp(buffer,"leave") == 0) {
            exitM = 1;
        }
        else if (strcmp(buffer,"load_sgr") == 0) {
            manload();
        }
        else if (strcmp(buffer,"businesses_started_by_letter") == 0) {
            manQ2();
        }
        else if (strcmp(buffer,"business_info") == 0) {
            manQ3();
        }
        else if (strcmp(buffer,"businesses_reviewed") == 0) {
            manQ4();
        }
        else if (strcmp(buffer,"businesses_with_stars_and_city") == 0) {
            manQ5();
        }
        else if (strcmp(buffer,"top_businesses_by_city") == 0) {
            manQ6();
        }
        else if (strcmp(buffer,"international_users") == 0) {
            manQ7();
        }
        else if (strcmp(buffer,"top_businesses_with_category") == 0) {
            manQ8();
        }
        else if (strcmp(buffer,"reviews_with_word") == 0) {
            manQ9();
        }
    }
    free(buffer);
    clear();
}
