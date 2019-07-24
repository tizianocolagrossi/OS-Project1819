/*
 * CONTROLLER & DITO CONTROLLER & DITO CONTROLLER & DITO CONTROLLER  
 *
 * funzioni per controller e dito
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "cont_sett_struct.h"
#include <xdo.h>

#define DELAY 1
#define NUM_ELEMENTS 5
#define MIG 's'
#define ANU 'w'
#define MED 'a'
#define IND 'd'
#define POL 'c'

/*
 * preme un dito se non è premuto quando chiamata
 */
void premi(Elemento* elem, xdo_t * x){
	if(elem->premuto == 0){
		const char * charAss = elem->charAss;
		xdo_send_keysequence_window_down(x, CURRENTWINDOW, charAss, DELAY);
		elem->premuto = 1;
	}
}

/*
 * rilascia un dito se è premuto quando chiamata
 */
void rilascia(Elemento* elem, xdo_t * x){
	if(elem->premuto == 1){
		const char * charAss = elem->charAss;
		xdo_send_keysequence_window_up(x, CURRENTWINDOW, charAss, DELAY);
		elem->premuto = 0;
	}
}

/*
 * inizializza il controller 
 */
void Controller_init(Controller* cnt) {
	cnt->xdo = xdo_new(NULL);
	cnt->elementi = (Elemento*) malloc(NUM_ELEMENTS*sizeof(Elemento));
	cnt->size = NUM_ELEMENTS;
	//settaggi di default
	// 0 mignolo
	// 1 anulare
	// 2 medio
	// 3 indice
	// 4 pollice

	for(int i = 0; i<NUM_ELEMENTS; i++){
		cnt->elementi[i].premuto=0;
		cnt->elementi[i].statoFisico=0;
		switch(i){
			case 0:
				cnt->elementi[i].tipo = mignolo;
				cnt->elementi[i].charAss = MIG;
				break;
			case 1:
				cnt->elementi[i].tipo = anulare;
				cnt->elementi[i].charAss = ANU;
				break;
			case 2:
				cnt->elementi[i].tipo = medio;
				cnt->elementi[i].charAss = MED;
				break;
			case 3:
				cnt->elementi[i].tipo = indice;
				cnt->elementi[i].charAss = IND;
				break;
			case 4:
				cnt->elementi[i].tipo = pollice;
				cnt->elementi[i].charAss = POL;
				break;
		}
	}
	
	
}

/*
 * libera lo spazio allocato da xdo
 */
void cntXdoFree(Controller* cnt){
	//dio cristo che non liberi un blocco libreria di merda
	xdo_free(cnt->xdo);
}

/*
 * modofica il carattere associato all' elemento
 */
void editElemCharAss(Controller* cnt, enum tipoElemento tipo, char newCharAss){
	printf(
		"\n"
		"##########################################################################\n"
		"CONTROLLER EDIT CONTROLLER EDIT CONTROLLER EDIT CONTROLLER EDIT CONTROLLER\n"
	);
	if(cnt->elementi[tipo].charAss == newCharAss){
		printf("\n\tzi non me fa fa cose inutili il carattere è gia impostato\n\n");
		return;
	}
	cnt->elementi[tipo].charAss = newCharAss;
	printf("\tnuovo carattere impostato per l'elemento %d > %c\n", tipo, cnt->elementi[tipo].charAss);
	return;
}

/*
 * stampa lo stato del controller
 */
void printControllerSetting(Controller* cnt){
	printf(
		"\n"
		"##########################################################################\n"
		"CONTROLLER STATUS CONTROLLER STATUS CONTROLLER STATUS CONTROLLER STATUS CO\n"
	);
	printf("\t0=mignolo, 1=anulare, 2=medio, 3=indice, 4=pollice\n");
	for(int i = 0; i<NUM_ELEMENTS; i++){
		char tasto;
		enum tipoElemento t;
		tasto = cnt->elementi[i].charAss;
		t = cnt->elementi[i].tipo;
		printf("\tl'elemento %d ha il carattere associato %c\n", t, tasto);
	}
		
}

/*
 * setta lo stato (premuto) di un elemento del controller
 */
void setElemento(Controller* cnt, enum tipoElemento tipo){
	if(cnt->elementi[tipo].statoFisico == 0){
		cnt->elementi[tipo].statoFisico = 1;
	}
}

/*
 * setta lo stato (rilasciato) di un elemento del controller
 */
void resetElemento(Controller* cnt, enum tipoElemento tipo){
	if(cnt->elementi[tipo].statoFisico == 1){
		cnt->elementi[tipo].statoFisico = 0;
	}
}

/*
 * controlla lo stato del controller e gestisce la virtualizzazione dei tasti
 */
void setState(Controller* cnt){
	for(int i = 0; cnt->size; i++){
		Elemento elem = cnt->elementi[i];
		if(cnt->elementi[i].statoFisico != cnt->elementi[i].premuto){
			if(cnt->elementi[i].statoFisico == 1){
				premi(&elem, cnt->xdo);
			}else{
				rilascia(&elem, cnt->xdo);
			}
		}
	}
}
