#pragma once
#include <xdo.h>

//Tiziano
enum tipoElemento{mignolo, anulare, medio, indice, pollice};

//Tiziano
typedef struct Elemento{
	int premuto;
	int statoFisico;
	char charAss;
	char *sAss;
	enum tipoElemento tipo;
}Elemento;


//Tiziano
typedef struct Controller{
	Elemento* elementi;
	int soglia;
	int size;
	xdo_t* xdo;
	void* t_id;
}Controller;

//Tiziano
int getSoglia(Controller* cnt);
void setSoglia(Controller* cnt, int newSgl);
void cntXdoFree(Controller* cnt);
void printControllerSetting(Controller* cnt);
void Controller_init(Controller* cnt);
void editElemCharAss(Controller* cnt, enum tipoElemento tipo, char newCharAss);
void setElemento(Controller* cnt, enum tipoElemento tipo);
void resetElemento(Controller* cnt, enum tipoElemento tipo);
void setState(Controller* cnt);
