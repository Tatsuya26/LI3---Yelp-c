#include "../../Includes/view/table.h" 

struct table{
	int  ls_alloc;
	int  ls;
	int  cs;
	char ***matrix;
	int  *sizes;
	int  *isNum;
	char **header;
};

#define MAX_FROMCSV 100000
#define PAGE_SIZE   10

// calcula o número total de caracteres numa linha
static int sizeof_line(TABLE t){
	int sum = 0;
	
	for(int j = 0; j < t-> cs; j++)
		sum += t-> sizes[j];

	sum += t-> cs;
	return sum;
}

static void printHyphens(TABLE t){
	int line_size = sizeof_line(t);
		
	for(int k = 0; k < line_size; k ++) 
		putchar('-');
	
	putchar('\n');
}

// faz print do header
static void show_top(TABLE t){
	printHyphens(t);
	for(int j = 0; j < t-> cs; j++){
		int size = t-> sizes[j]/2;
		printf("|%*s%*s", (int) (size+strlen(t-> header[j])/2), t-> header[j], (int) (size-strlen(t-> header[j])/2), "");
	}
	printf("|\n");
	printHyphens(t);
}

static TABLE copyInfo(TABLE cpy, TABLE t, int n){
	for(int j = 0; j < n; j++){
		cpy-> isNum[j] = t-> isNum[j];
		cpy-> header[j] = strdup(t-> header[j]);
	}

	return cpy;
}

TABLE set_header(TABLE t, char *line, char *delim){
	char *s = strdup(line);

	for(int j = 0; j < t-> cs; j++){
		t-> header[j] = strsep(&s, delim);
		if(strlen(t-> header[j]) + 8 > t-> sizes[j]){
			t-> sizes[j] = strlen(t-> header[j]) + 8;
		}

	}

	return t;	
}

TABLE set_col_as_Num(TABLE t, int col){
	if(col >= 0 && col < t-> cs)
		t-> isNum[col] = 1;

	return t;
}

static int final_PAGE(TABLE t){
	return (int) ceil((t-> ls * 1.0) / PAGE_SIZE);
}

// faz print de uma página
static void showPAGE(TABLE t, int page){
	int ini, fim;
	page--;
	ini = page * PAGE_SIZE;
	if(page * PAGE_SIZE + PAGE_SIZE > t-> ls)
		fim = t-> ls;
	else
		fim = page * PAGE_SIZE + PAGE_SIZE;

	for(int i = ini; i < fim; i ++){
		for(int j = 0; j < t-> cs; j++){
			int size = t-> sizes[j]/2;
			printf("|%*s%*s", (int) (size+strlen(t-> matrix[i][j])/2), t-> matrix[i][j], (int) (size-strlen(t-> matrix[i][j])/2), "");
		}
		printf("|\n");
		printHyphens(t);
	}
	int total_pages = final_PAGE(t);
	char *s = malloc(sizeof(char) * 56);
	sprintf(s, "(%d a %d de %d) | (página %d de %d)\n", ini + 1, fim, t-> ls, page + 1, total_pages);
	int size_line = sizeof_line(t) / 2;
	printf("%*s%*s\n", (int) (size_line + strlen(s)/2), s, (int) (size_line + strlen(s)/2), "");
	free(s);
}

// faz print de a página selecionada e do cabeçalho
void show(TABLE t, int page){
	if(t != NULL){
		if(page < 1)
			page = 1;
		else if(page > final_PAGE(t))
			page = final_PAGE(t);
		show_top(t);
		if(t-> ls > 0)
			showPAGE(t, page);
	}
}

// aloca espaço para uma TABLE
TABLE alloc_TABLE(int ls, int cs){
	TABLE t = malloc(sizeof(struct table));

	t-> ls_alloc = ls;
	t-> ls = 0;
	t-> cs = cs;
	t-> header = malloc(sizeof(char *) * t-> cs);
	t-> matrix = malloc(sizeof(char **) * ls);
	t-> sizes = malloc(sizeof(int) * cs);
	t-> isNum = malloc(sizeof(int) * cs);
	for(int i = 0; i < ls; i++){
		t-> matrix[i] = malloc(sizeof(char *) * cs);
		for(int j = 0; j < cs; j++){
			t-> sizes[j] = 10;
			t-> isNum[j] = 0;
		}
	}
	
	return t;	
}


void destroy_TABLE(TABLE t){
	for(int i = 0; i < t-> ls; i++)
		for(int j = 0; j < t-> cs; j++)
			free(t-> matrix[i][j]);
	
	free(t-> header[0]);
	free(t-> header);
	free(t-> sizes);
	free(t-> isNum);

	for(int i = 0; i < t-> ls; i++)
		free(t-> matrix[i]);

	free(t-> matrix);
	free(t);
}

TABLE duplicate_TABLE(TABLE t){
	TABLE r = alloc_TABLE(t-> ls_alloc * 2, t-> cs);
	r-> ls = t-> ls;

	for(int i = 0; i < t-> ls_alloc; i++)
		for(int j = 0; j < t-> cs; j++)
			r-> matrix[i][j] = strdup(t-> matrix[i][j]);

	r = copyInfo(r, t, t-> cs);

	destroy_TABLE(t);
	return r;
}

// adiciona uma linha (preenchendo todas as colunas) no final da TABLE
TABLE add_line_to_TABLE(TABLE t, char *v[]){
	if(t->ls >= t-> ls_alloc){ 
		t = duplicate_TABLE(t);
	}
	
	for(int j = 0; j < t-> cs; j++){
		t-> matrix[t-> ls][j] = strdup(v[j]);
		if(strlen(t-> matrix[t-> ls][j]) + 2 > t-> sizes[j])
			t-> sizes[j] = strlen(t-> matrix[t-> ls][j]) + 10;
	}

	t->ls++;
	return t; 
}

int toCSV(TABLE t, char *delim, char *filepath){
	FILE *f = fopen(filepath, "w");
	
	int i, j;
	for(j = 0; j < t-> cs - 1; j++)
		fprintf(f, "%s%s", t-> header[j], delim);
	fprintf(f, "%s\n", t-> header[j]);

	for(i = 0; i < t-> ls; i++){
		for(j = 0; j < t-> cs - 1; j++)
			fprintf(f, "%s%s", t-> matrix[i][j], delim);
		fprintf(f, "%s\n", t-> matrix[i][j]);
	}

	fclose(f);
	return 0;
}

TABLE indexing(TABLE t, int l, int c){
	TABLE r = alloc_TABLE(1, 1);
	if (t->cs < c || t->ls < l) return NULL;
	if(strlen(t-> matrix[l][c]) > strlen(t-> header[c]))
		r-> sizes[0] = strlen(t-> matrix[l][c]) + 10;
	else
		r-> sizes[0] = strlen(t-> header[c]) + 10;

	r-> header[0] = strdup(t-> header[c]);
	r-> matrix[0][0] = strdup(t-> matrix[l][c]);
	r-> isNum[0] = t-> isNum[c];
	r-> ls = 1;

	return r;
}

int validaCol(int args[],int N,TABLE t) {
	int c = t->cs,valido = 1;
	for (int i = 0;i < N && valido;i++) {
		if (args[i] > c || args[i] <= 0) valido = 0;
	}
	return valido;
}

TABLE proj_cols(TABLE t, int args[], int n){
	TABLE r = alloc_TABLE(1, n);
	for(int i = 0; i < n; i++){
		r-> sizes[i] = t-> sizes[args[i] - 1];
		r-> header[i] = strdup(t-> header[args[i] - 1]);

	}

	for(int i = 0; i < t-> ls; i++){
		char **line = malloc(sizeof(char *) * n);
		for(int j = 0; j < n; j++) 
			line[j] = strdup(t-> matrix[i][args[j] - 1]);
		r = add_line_to_TABLE(r, line);
	}

	return r;
}

static int get_nrCols_fromLine(char *line, char *delim){
	if(line == NULL)
		return 0;

	int r;
	char *s = strdup(line);
	for(r = 0; s != NULL; r++){
		strsep(&s, delim);
	}

	return r;
}

// transforma uma string num array de strings (linha da matriz da TABLE) para inserir na TABLE
static char** READline_to_TABLEline(char *read, char *delim, int cs){
	if(read == NULL)
		return NULL;

	char *s = strdup(read);
	char **line = malloc(sizeof(char *) * cs);
	int i;
	for(i = 0; s != NULL && i < cs; i++)
		line[i] = strsep(&s, delim);	

	i--;
	line[i][strlen(line[i]) - 1] = '\0';
	i++;
	for(; i < cs; i++)
		line[i] = "";

	return line;
}

static void free_LINE(char **line, int cs){
	free(line[0]);
	free(line);
}

TABLE fromCSV(char *filepath, char *delim){
	FILE *f = fopen(filepath, "r");
	char *line = malloc(sizeof(char) * MAX_FROMCSV);
	
	if(fgets(line, MAX_FROMCSV, f) <= 0)
		return NULL;

	char *header = line;
	header[strlen(header) - 1] = '\0';
	int cs = get_nrCols_fromLine(line, delim);
	TABLE t = alloc_TABLE(1, cs);
	t = set_header(t, header, delim);

	int index = 0;
	while(fgets(line, MAX_FROMCSV, f) > 0){
		char **lineT = READline_to_TABLEline(line, delim, cs);
		if(!index){
			int j;
			for(j = 0; j < cs && strcmp("", lineT[j]) != 0; j++){
				if(checkNumericalString(lineT[j]))
					t-> isNum[j] = 1;
				else
					t-> isNum[j] = 0;
			}
			for(; j < cs; j++)
				t-> isNum[j] = 0;
			index++;
		}
		t = add_line_to_TABLE(t, lineT);
		free_LINE(lineT, cs);
	}
	free(line);

	return t;
}

TABLE from_RVA_to_table(RVA r) {
	TABLE t = alloc_TABLE(1,3);
	int dim = get_RVA_size(r);
	for(int i = 0; i < dim; i++) {
		char** a = RVAindex_to_line(r,i);
		add_line_to_TABLE(t,a);
	}
	return t;
}

// função usada para transformar poder utilizar atof sem problemas numa string que representa um número
static void turn_comma_to_point(char *value){
	for(int i = 0; value[i] != '\0'; i++)
		if(value[i] == ',')
			value[i] = '.';
}

static float compareNums(char *value, char *matrix){
	char *tmpMatrix = strdup(matrix);
	float fValue, fMatrix;
	
	fValue = atof(value);
	turn_comma_to_point(tmpMatrix);
	fMatrix = atof(tmpMatrix);
	free(tmpMatrix);

	return (fValue - fMatrix);
}

static char** strdup_line(char **v, int n){
	char **line = malloc(sizeof(char *) * n);
	
	for(int j = 0; j < n; j++)
		line[j] = strdup(v[j]);

	return line;
}

static int searchColumnInHeader(TABLE t, char *column_name){
	int col = -1;

	for(int j = 0; col == -1 && j < t-> cs; j++)
		if(!strcmp(column_name, t-> header[j]))
			col = j;
	
	return col;
}


TABLE filter(TABLE t, char *column_name, char *value, enum OPERATOR op){//column_name
	int col = searchColumnInHeader(t, column_name);
	if(col == -1) return NULL;

	TABLE r = alloc_TABLE(1, t-> cs);
	char *tmpValue = NULL;
	if(t-> isNum[col]){
		tmpValue = strdup(value);
		turn_comma_to_point(tmpValue);
	}

	r = copyInfo(r, t, t-> cs);
	
	for(int i = 0; i < t-> ls; i++){
		float res;
		if(t-> isNum[col])
			res = compareNums(tmpValue, t-> matrix[i][col]);
		else
			res = (float) strcmp(value, t-> matrix[i][col]);

		int bool = 0;
		switch(op){
			case 0:	if(res > 0) bool = 1; 
			break;

			case 1: if(res == 0) bool = 1; 
			break;

			case 2: if(res < 0) bool = 1; 
			break;
		}
		if(bool){
			char **line = strdup_line(t-> matrix[i], t-> cs);
			r = add_line_to_TABLE(r, line);
		}
	}
	free(tmpValue);

	return r;
}
