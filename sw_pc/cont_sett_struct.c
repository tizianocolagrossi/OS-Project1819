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

#define DELAY 200000
#define NUM_ELEMENTS 5
#define MIG 's'
#define ANU 'w'
#define MED 'a'
#define IND 'd'
#define POL 'c'
#define SOGLIA 800

/*
 * Tiziano
 * inizializza il controller 
 */
void Controller_init(Controller* cnt) {
	cnt->xdo = xdo_new(NULL);
	cnt->elementi = (Elemento*) malloc(NUM_ELEMENTS*sizeof(Elemento));
	cnt->size = NUM_ELEMENTS;
	cnt->t_id = NULL;
	cnt->soglia = SOGLIA;
	//settaggi di default
	// 0 mignolo
	// 1 anulare
	// 2 medio
	// 3 indice
	// 4 pollice

	for(int i = 0; i<NUM_ELEMENTS; i++){
		cnt->elementi[i].premuto=0;
		cnt->elementi[i].statoFisico=0;
		cnt->elementi[i].sAss = (char*)malloc(2*sizeof(char));
		switch(i){
			case 0:
				cnt->elementi[i].tipo = mignolo;
				cnt->elementi[i].charAss = MIG;
				cnt->elementi[i].sAss[0] = MIG;
				cnt->elementi[i].sAss[1] = '\0';
				break;
			case 1:
				cnt->elementi[i].tipo = anulare;
				cnt->elementi[i].charAss = ANU;
				cnt->elementi[i].sAss[0] = ANU;
				cnt->elementi[i].sAss[1] = '\0';
				break;
			case 2:
				cnt->elementi[i].tipo = medio;
				cnt->elementi[i].charAss = MED;
				cnt->elementi[i].sAss[0] = MED;
				cnt->elementi[i].sAss[1] = '\0';
				break;
			case 3:
				cnt->elementi[i].tipo = indice;
				cnt->elementi[i].charAss = IND;
				cnt->elementi[i].sAss[0] = IND;
				cnt->elementi[i].sAss[1] = '\0';
				break;
			case 4:
				cnt->elementi[i].tipo = pollice;
				cnt->elementi[i].charAss = POL;
				cnt->elementi[i].sAss[0] = POL;
				cnt->elementi[i].sAss[1] = '\0';
				break;
		}
	}
	
	
}

/*
 * Tiziano
 * restituisce il valore della soglia del controller
 */
int getSoglia(Controller* cnt){
	int sgl = cnt->soglia;
	return sgl;
}
/*
 * Tiziano
 * restituisce il valore della soglia del controller
 */
void setSoglia(Controller* cnt, int newSgl){
	int sgl = cnt->soglia;
	if(newSgl != sgl){ //michele
			cnt->soglia = newSgl;
	}
}

/*
 * Tiziano
 * libera lo spazio allocato da xdo
 */
void cntXdoFree(Controller* cnt){
	//dio cristo che non liberi un blocco libreria di merda
	xdo_free(cnt->xdo);
}

/*
 * Tiziano
 * modofica il carattere associato all' elemento
 */
void editElemCharAss(Controller* cnt, enum tipoElemento tipo, char newCharAss){
	printf(
		"\n"
		"CONTROLLER EDIT CONTROLLER EDIT CONTROLLER EDIT CONTROLLER EDIT CONTROLLER\n"
		"OLLER EDIT CONTROLLER EDIT CONTROLLER EDIT CONTROLLER EDIT CONTROLLER EDIT\n"
	);
	if(cnt->elementi[tipo].charAss == newCharAss){
		printf("\n\tzi non me fa fa cose inutili il carattere è gia impostato\n\n");
		return;
	}
	cnt->elementi[tipo].charAss = newCharAss;
	cnt->elementi[tipo].sAss[0] = newCharAss;
	cnt->elementi[tipo].sAss[1] = '\0';
	printf("\tnuovo carattere impostato per l'elemento %d > %c\n", tipo, cnt->elementi[tipo].charAss);
	return;
}

/*
 * Tiziano
 * stampa lo stato del controller
 */
void printControllerSetting(Controller* cnt){
	printf(
		"\n"
		"CONTROLLER STATUS CONTROLLER STATUS CONTROLLER STATUS CONTROLLER STATUS CO\n"
		"TROLLER STATUS CONTROLLER STATUS CONTROLLER STATUS CONTROLLER STATUS CONTR\n"
		"\n\n"
	);
	printf("\t0=mignolo, 1=anulare, 2=medio, 3=indice, 4=pollice\n\n");
	for(int i = 0; i<NUM_ELEMENTS; i++){
		char tasto;
		enum tipoElemento t;
		tasto = cnt->elementi[i].charAss;
		t = cnt->elementi[i].tipo;
		printf("\tl'elemento %d ha il carattere associato %c\n", t, tasto);
	}
	printf("\n\tla soglia del controller è impostata a %d\n\n\n", cnt->soglia);
		
}

/*
 * Tiziano
 * setta lo stato (premuto) di un elemento del controller
 */
void setElemento(Controller* cnt, enum tipoElemento tipo){
	if(cnt->elementi[tipo].statoFisico == 0){
		cnt->elementi[tipo].statoFisico = 1;
	}
}

/*
 * Tiziano
 * setta lo stato (rilasciato) di un elemento del controller
 */
void resetElemento(Controller* cnt, enum tipoElemento tipo){
	if(cnt->elementi[tipo].statoFisico == 1){
		cnt->elementi[tipo].statoFisico = 0;
	}
}

/*
 * Tiziano
 * controlla lo stato del controller e gestisce la virtualizzazione dei tasti
 */
void setState(Controller* cnt){
	xdo_t * x = cnt->xdo;
	
	for(int i = 0; i<cnt->size;i++){
		if(cnt->elementi[i].statoFisico==1){
			xdo_send_keysequence_window_down(x, CURRENTWINDOW, cnt->elementi[i].sAss, 0);
		}else{
			xdo_send_keysequence_window_up(x, CURRENTWINDOW, cnt->elementi[i].sAss, 0);
		}
	}
}
