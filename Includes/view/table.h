#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include "../Model/AuxStructs.h"
#include <math.h>

typedef struct table* TABLE;

enum OPERATOR{LT, EQ, GT};

TABLE set_header(TABLE t, char *line, char *delim);
TABLE set_col_as_Num(TABLE t, int col);
void  show(TABLE t, int page);
TABLE alloc_TABLE(int ls, int cs);
void  destroy_TABLE(TABLE t);
TABLE duplicate_TABLE(TABLE t);
TABLE add_to_TABLE(TABLE t, char *s, int l, int c);
TABLE add_line_to_TABLE(TABLE t, char *v[]);
int   toCSV(TABLE t, char *delim, char *filepath);
TABLE indexing(TABLE t, int l, int c);
int validaCol(int args[],int N,TABLE t);
TABLE proj_cols(TABLE t, int agrs[], int n);
TABLE fromCSV(char *filepath, char *delim);
TABLE from_RVA_to_table(RVA r);
TABLE filter(TABLE t, char *column_name, char *value, enum OPERATOR op);
//TABLE filter(TABLE t, int col, char *value, enum OPERATOR op);

#endif
