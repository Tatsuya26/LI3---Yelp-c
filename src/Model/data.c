#include "../../Includes/Model/data.h"
	
struct data{
	int ano;
	int mes;
	int dia;
	int hora;
	int min;
	int seg;
};

static data create_data (){
	data new = malloc (sizeof (struct data));
	return new;
}

//Funcao que valida o dia
static int validDay (data d) {
	int valido = 1;
	if (get_mes(d) == 1 || get_mes(d) == 3 || get_mes(d) == 5 || get_mes(d) == 7 || get_mes(d) == 8 || get_mes(d) == 10 || get_mes(d) == 12) {
		if (get_dia(d) < 0 || get_dia(d) > 31) valido = 0;
	}
	
	if (get_mes(d) == 4 || get_mes(d) == 6 || get_mes(d) == 9 || get_mes(d) == 11) {
		if (get_dia(d) < 0 || get_dia(d) > 30) valido = 0;
	}

	if (get_mes(d) == 2 && get_ano(d) %4 == 0) {
		if (get_dia(d) < 0 || get_dia(d) > 29) valido = 0;
	}

	else if (get_mes(d) == 2)  {
		if (get_dia(d) < 0 || get_dia(d) > 28) valido = 0;
	}
	return valido;
}

//Funcao que valida uma data.
static int valid_data(data d) {
	int valido = 1;
	if (get_ano(d) < 1992 || get_ano(d) > 2021) valido = 0;
	if (get_mes(d) < 0 || get_mes(d) > 12) valido = 0;
	if (!validDay (d)) valido = 0;
	if (get_hora(d) < 0 || get_hora(d) > 23) valido = 0;
	if (get_min(d) < 0 || get_min(d) > 59) valido = 0;
	if (get_seg(d) < 0 || get_seg(d) > 59) valido = 0;
	return valido;
}

data init_data (int ano,int mes,int dia,int hora,int min,int seg){
	data d = create_data();
	d->ano = ano;
	d->mes = mes;
	d->dia = dia;	
	d->hora = hora;
	d->min = min;
	d->seg = seg;
	if (valid_data(d)) return d;
	else return NULL;
}

data init_data_fromtext (char *line) {
	int ano,mes,dia,hora,min,seg;
	if (sscanf (line,"%d-%d-%d %d:%d:%d",&ano,&mes,&dia,&hora,&min,&seg) == 6) {
		data d = init_data(ano,mes,dia,hora,min,seg);
		return d;
	}
	return NULL;
}

int get_ano (data d) {
	return d->ano;
}

int get_mes (data d) {
	return d->mes;
}

int get_dia (data d){
	return d->dia;
}

int get_hora (data d){
	return d->hora;
}

int get_min (data d) {
	return d->min;
}

int get_seg (data d) {
	return d->seg;
}

char *get_data (data d) {
	char info[30];
	sprintf(info,"%d-%d-%d %d:%d:%d",d->ano,d->mes,d->dia,d->hora,d->min,d->seg);
	return strdup(info);
}

int set_ano (data d,int ano) {
	if (d){
		d->ano = ano;
		return 1;
	}
	return 0;
}

int set_mes (data d,int mes){
	if(d){
		d->mes = mes;
		return 1;
	}
	return 0;
}

int set_dia (data d,int dia){
	if(d){
		d->dia = dia;
		return 1;
	}
	return 0;
}

int set_hora (data d,int hora) {
	if(d){
		d->hora = hora;
		return 1;
	}
	return 0;
}

int set_min (data d,int min){
	if (d){
		d->min = min;
		return 1;
	}
	return 0;
}

int set_seg (data d,int seg){
	if(d){
		d->seg = seg;
		return 1;
	}
	return 0;
}

data clone_data (data d){
	if (d) {
		data new = malloc (sizeof (struct data));
		new->ano = d->ano;
		new->mes = d->mes;
		new->dia = d->dia;
		new->hora = d->hora;
		new->min = d->min;
		new->seg = d->seg;
		return new;
	}
	else return NULL;
}

void destroy_data (data d){
	if (d) free(d);
}
