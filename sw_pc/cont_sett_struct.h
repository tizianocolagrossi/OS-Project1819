#pragma once
#include <xdo.h>

enum tipoElemento{mignolo, anulare, medio, indice, pollice};

typedef struct Elemento{
	int premuto;
	int statoFisico;
	char charAss;
	enum tipoElemento tipo;
}Elemento;

void premi(Elemento* dito, xdo_t * x);
void rilascia(Elemento* dito, xdo_t * x);


typedef struct Controller{
	Elemento* elementi;
	int size;
	xdo_t* xdo;
}Controller;

void Controller_init(Controller* cnt);
void editElemCharAss(Controller* cnt, enum tipoElemento tipo, char newCharAss);
void setElemento(Controller* cnt, enum tipoElemento tipo);
void resetElemento(Controller* cnt, enum tipoElemento tipo);
void setState(Controller* cnt);
