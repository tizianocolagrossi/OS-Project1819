/*
 *
 * Functions for controller and fingers
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
#define MED '3'
#define IND '2'
#define POL 'z'
#define SOGLIA 800

/*
 * Tiziano
 * Controller initialization
 *
 */
void Controller_init(Controller* cnt) {
	cnt->xdo = xdo_new(NULL);
	cnt->elementi = (Elemento*) malloc(NUM_ELEMENTS*sizeof(Elemento));
	cnt->size = NUM_ELEMENTS;
	cnt->t_id = NULL;
	cnt->soglia = SOGLIA;
	// default values:
	// 0 thumb
	// 1 index
	// 2 middle
	// 3 ring
	// 4 pinkie

	for(int i = 0; i<NUM_ELEMENTS; i++){
		cnt->elementi[i].premuto=0;
		cnt->elementi[i].statoFisico=0;
		cnt->elementi[i].sAss = (char*)malloc(10*sizeof(char));
		switch(i){
			case 4:
				cnt->elementi[i].tipo = mignolo;
				cnt->elementi[i].charAss = MIG;
				if(MIG=='1'){
					cnt->elementi[i].sAss[0] = 's';
					cnt->elementi[i].sAss[1] = 'p';
					cnt->elementi[i].sAss[2] = 'a';
					cnt->elementi[i].sAss[3] = 'c';
					cnt->elementi[i].sAss[4] = 'e';
					cnt->elementi[i].sAss[5] = '\0';
				}else if(MIG == '2'){
					cnt->elementi[i].sAss[0] = 'L';
					cnt->elementi[i].sAss[1] = 'e';
					cnt->elementi[i].sAss[2] = 'f';
					cnt->elementi[i].sAss[3] = 't';
					cnt->elementi[i].sAss[4] = '\0';
				}else if(MIG == '3'){
					cnt->elementi[i].sAss[0] = 'R';
					cnt->elementi[i].sAss[1] = 'i';
					cnt->elementi[i].sAss[2] = 'g';
					cnt->elementi[i].sAss[3] = 'h';
					cnt->elementi[i].sAss[4] = 't';
					cnt->elementi[i].sAss[5] = '\0';
				}else if(MIG == '4'){
					cnt->elementi[i].sAss[0] = 'U';
					cnt->elementi[i].sAss[1] = 'p';
					cnt->elementi[i].sAss[3] = '\0';
				}else if(MIG == '5'){
					cnt->elementi[i].sAss[0] = 'D';
					cnt->elementi[i].sAss[1] = 'o';
					cnt->elementi[i].sAss[2] = 'w';
					cnt->elementi[i].sAss[3] = 'n';
					cnt->elementi[i].sAss[4] = '\0';
				}else{
					cnt->elementi[i].sAss[0] = MIG;
					cnt->elementi[i].sAss[1] = '\0';
				}
				break;
			case 3:
				cnt->elementi[i].tipo = anulare;
				cnt->elementi[i].charAss = ANU;
				if(ANU=='1'){
					cnt->elementi[i].sAss[0] = 's';
					cnt->elementi[i].sAss[1] = 'p';
					cnt->elementi[i].sAss[2] = 'a';
					cnt->elementi[i].sAss[3] = 'c';
					cnt->elementi[i].sAss[4] = 'e';
					cnt->elementi[i].sAss[5] = '\0';
				}else if(ANU == '2'){
					cnt->elementi[i].sAss[0] = 'L';
					cnt->elementi[i].sAss[1] = 'e';
					cnt->elementi[i].sAss[2] = 'f';
					cnt->elementi[i].sAss[3] = 't';
					cnt->elementi[i].sAss[4] = '\0';
				}else if(ANU == '3'){
					cnt->elementi[i].sAss[0] = 'R';
					cnt->elementi[i].sAss[1] = 'i';
					cnt->elementi[i].sAss[2] = 'g';
					cnt->elementi[i].sAss[3] = 'h';
					cnt->elementi[i].sAss[4] = 't';
					cnt->elementi[i].sAss[5] = '\0';
				}else if(ANU == '4'){
					cnt->elementi[i].sAss[0] = 'U';
					cnt->elementi[i].sAss[1] = 'p';
					cnt->elementi[i].sAss[3] = '\0';
				}else if(ANU == '5'){
					cnt->elementi[i].sAss[0] = 'D';
					cnt->elementi[i].sAss[1] = 'o';
					cnt->elementi[i].sAss[2] = 'w';
					cnt->elementi[i].sAss[3] = 'n';
					cnt->elementi[i].sAss[4] = '\0';
				}else{
					cnt->elementi[i].sAss[0] = ANU;
					cnt->elementi[i].sAss[1] = '\0';
				}
				break;
			case 2:
				cnt->elementi[i].tipo = medio;
				cnt->elementi[i].charAss = MED;
				if(MED=='1'){
					cnt->elementi[i].sAss[0] = 's';
					cnt->elementi[i].sAss[1] = 'p';
					cnt->elementi[i].sAss[2] = 'a';
					cnt->elementi[i].sAss[3] = 'c';
					cnt->elementi[i].sAss[4] = 'e';
					cnt->elementi[i].sAss[5] = '\0';
				}else if(MED == '2'){
					cnt->elementi[i].sAss[0] = 'L';
					cnt->elementi[i].sAss[1] = 'e';
					cnt->elementi[i].sAss[2] = 'f';
					cnt->elementi[i].sAss[3] = 't';
					cnt->elementi[i].sAss[4] = '\0';
				}else if(MED == '3'){
					cnt->elementi[i].sAss[0] = 'R';
					cnt->elementi[i].sAss[1] = 'i';
					cnt->elementi[i].sAss[2] = 'g';
					cnt->elementi[i].sAss[3] = 'h';
					cnt->elementi[i].sAss[4] = 't';
					cnt->elementi[i].sAss[5] = '\0';
				}else if(MED == '4'){
					cnt->elementi[i].sAss[0] = 'U';
					cnt->elementi[i].sAss[1] = 'p';
					cnt->elementi[i].sAss[3] = '\0';
				}else if(MED == '5'){
					cnt->elementi[i].sAss[0] = 'D';
					cnt->elementi[i].sAss[1] = 'o';
					cnt->elementi[i].sAss[2] = 'w';
					cnt->elementi[i].sAss[3] = 'n';
					cnt->elementi[i].sAss[4] = '\0';
				}else{
					cnt->elementi[i].sAss[0] = MED;
					cnt->elementi[i].sAss[1] = '\0';
				}
				break;
			case 1:
				cnt->elementi[i].tipo = indice;
				cnt->elementi[i].charAss = IND;
				if(IND=='1'){
					cnt->elementi[i].sAss[0] = 's';
					cnt->elementi[i].sAss[1] = 'p';
					cnt->elementi[i].sAss[2] = 'a';
					cnt->elementi[i].sAss[3] = 'c';
					cnt->elementi[i].sAss[4] = 'e';
					cnt->elementi[i].sAss[5] = '\0';
				}else if(IND == '2'){
					cnt->elementi[i].sAss[0] = 'L';
					cnt->elementi[i].sAss[1] = 'e';
					cnt->elementi[i].sAss[2] = 'f';
					cnt->elementi[i].sAss[3] = 't';
					cnt->elementi[i].sAss[4] = '\0';
				}else if(IND == '3'){
					cnt->elementi[i].sAss[0] = 'R';
					cnt->elementi[i].sAss[1] = 'i';
					cnt->elementi[i].sAss[2] = 'g';
					cnt->elementi[i].sAss[3] = 'h';
					cnt->elementi[i].sAss[4] = 't';
					cnt->elementi[i].sAss[5] = '\0';
				}else if(IND == '4'){
					cnt->elementi[i].sAss[0] = 'U';
					cnt->elementi[i].sAss[1] = 'p';
					cnt->elementi[i].sAss[3] = '\0';
				}else if(IND == '5'){
					cnt->elementi[i].sAss[0] = 'D';
					cnt->elementi[i].sAss[1] = 'o';
					cnt->elementi[i].sAss[2] = 'w';
					cnt->elementi[i].sAss[3] = 'n';
					cnt->elementi[i].sAss[4] = '\0';
				}else{
					cnt->elementi[i].sAss[0] = IND;
					cnt->elementi[i].sAss[1] = '\0';
				}
				break;
			case 0:
				cnt->elementi[i].tipo = pollice;
				cnt->elementi[i].charAss = POL;
				if(POL=='1'){
					cnt->elementi[i].sAss[0] = 's';
					cnt->elementi[i].sAss[1] = 'p';
					cnt->elementi[i].sAss[2] = 'a';
					cnt->elementi[i].sAss[3] = 'c';
					cnt->elementi[i].sAss[4] = 'e';
					cnt->elementi[i].sAss[5] = '\0';
				}else if(POL == '2'){
					cnt->elementi[i].sAss[0] = 'L';
					cnt->elementi[i].sAss[1] = 'e';
					cnt->elementi[i].sAss[2] = 'f';
					cnt->elementi[i].sAss[3] = 't';
					cnt->elementi[i].sAss[4] = '\0';
				}else if(POL == '3'){
					cnt->elementi[i].sAss[0] = 'R';
					cnt->elementi[i].sAss[1] = 'i';
					cnt->elementi[i].sAss[2] = 'g';
					cnt->elementi[i].sAss[3] = 'h';
					cnt->elementi[i].sAss[4] = 't';
					cnt->elementi[i].sAss[5] = '\0';
				}else if(POL == '4'){
					cnt->elementi[i].sAss[0] = 'U';
					cnt->elementi[i].sAss[1] = 'p';
					cnt->elementi[i].sAss[3] = '\0';
				}else if(POL == '5'){
					cnt->elementi[i].sAss[0] = 'D';
					cnt->elementi[i].sAss[1] = 'o';
					cnt->elementi[i].sAss[2] = 'w';
					cnt->elementi[i].sAss[3] = 'n';
					cnt->elementi[i].sAss[4] = '\0';
				}else{
					cnt->elementi[i].sAss[0] = POL;
					cnt->elementi[i].sAss[1] = '\0';
				}
				break;
		}
	}
	
	
}

/*
 * Tiziano
 * returns the threeshold of controller
 */
int getSoglia(Controller* cnt){
	int sgl = cnt->soglia;
	return sgl;
}
/*
 * Tiziano
 * set the threeshold of controller
 */
void setSoglia(Controller* cnt, int newSgl){
	int sgl = cnt->soglia;
	if(newSgl != sgl) cnt->soglia = newSgl;
}

/*
 * Tiziano
 * free space allocated by xdo
 */
void cntXdoFree(Controller* cnt){
	xdo_free(cnt->xdo);
}

/*
 * Tiziano
 * modify character associated to the element
 */
void editElemCharAss(Controller* cnt, enum tipoElemento tipo, char newCharAss){
	printf(
		"\n"
		"CONTROLLER EDIT CONTROLLER EDIT CONTROLLER EDIT CONTROLLER EDIT CONTROLLER\n"
		"OLLER EDIT CONTROLLER EDIT CONTROLLER EDIT CONTROLLER EDIT CONTROLLER EDIT\n"
	);
	if(cnt->elementi[tipo].charAss == newCharAss){
		printf("\n\tCharacter already associated to this element\n\n");
		return;
	}
	cnt->elementi[tipo].charAss = newCharAss;
	if(newCharAss=='1'){
					cnt->elementi[tipo].sAss[0] = 's';
					cnt->elementi[tipo].sAss[1] = 'p';
					cnt->elementi[tipo].sAss[2] = 'a';
					cnt->elementi[tipo].sAss[3] = 'c';
					cnt->elementi[tipo].sAss[4] = 'e';
					cnt->elementi[tipo].sAss[5] = '\0';
				}else if(newCharAss == '2'){
					cnt->elementi[tipo].sAss[0] = 'L';
					cnt->elementi[tipo].sAss[1] = 'e';
					cnt->elementi[tipo].sAss[2] = 'f';
					cnt->elementi[tipo].sAss[3] = 't';
					cnt->elementi[tipo].sAss[4] = '\0';
				}else if(newCharAss == '3'){
					cnt->elementi[tipo].sAss[0] = 'R';
					cnt->elementi[tipo].sAss[1] = 'i';
					cnt->elementi[tipo].sAss[2] = 'g';
					cnt->elementi[tipo].sAss[3] = 'h';
					cnt->elementi[tipo].sAss[4] = 't';
					cnt->elementi[tipo].sAss[5] = '\0';
				}else if(newCharAss == '4'){
					cnt->elementi[tipo].sAss[0] = 'U';
					cnt->elementi[tipo].sAss[1] = 'p';
					cnt->elementi[tipo].sAss[3] = '\0';
				}else if(newCharAss == '5'){
					cnt->elementi[tipo].sAss[0] = 'D';
					cnt->elementi[tipo].sAss[1] = 'o';
					cnt->elementi[tipo].sAss[2] = 'w';
					cnt->elementi[tipo].sAss[3] = 'n';
					cnt->elementi[tipo].sAss[4] = '\0';
				}else{
		cnt->elementi[tipo].sAss[0] = newCharAss;
		cnt->elementi[tipo].sAss[1] = '\0';
	}
	printf("\tNew character associated to element %d > %c\n", tipo, cnt->elementi[tipo].charAss);
	return;
}

/*
 * Tiziano
 * prints state of controller
 */
void printControllerSetting(Controller* cnt){
	printf(
		"\n"
		"CONTROLLER STATUS CONTROLLER STATUS CONTROLLER STATUS CONTROLLER STATUS CO\n"
		"TROLLER STATUS CONTROLLER STATUS CONTROLLER STATUS CONTROLLER STATUS CONTR\n"
		"\n\n"
	);
	printf("\t0=thumb, 1=index, 2=middle, 3=ring, 4=pinkie\n\n");
	for(int i = 0; i<NUM_ELEMENTS; i++){
		char tasto;
		enum tipoElemento t;
		tasto = cnt->elementi[i].charAss;
		t = cnt->elementi[i].tipo;
		printf("\tElement %d has associated character %c\n", t, tasto);
	}
	printf("\n\tController's threeshold is %d\n\n\n", cnt->soglia);
		
}

/*
 * Tiziano
 * set the state (pressed) of an element of controller
 */
void setElemento(Controller* cnt, enum tipoElemento tipo){
	if(cnt->elementi[tipo].statoFisico == 0){
		cnt->elementi[tipo].statoFisico = 1;
	}
}

/*
 * Tiziano
 * set the state (released) of an element of controller
 */
void resetElemento(Controller* cnt, enum tipoElemento tipo){
	if(cnt->elementi[tipo].statoFisico == 1){
		cnt->elementi[tipo].statoFisico = 0;
	}
}

/*
 * Tiziano
 * check the state of the controller and handle key's virtualization
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
