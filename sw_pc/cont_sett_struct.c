/*
 * CONTROLLER & DITO CONTROLLER & DITO CONTROLLER & DITO CONTROLLER  
 *
 * funzioni per controller e dito
 * 
 */

#include <stdio.h>
#include "cont_sett_struct.h"
#include <xdo.h>

#define MIGNOLO 's'
#define ANULARE 'w'
#define MEDIO 'a'
#define INDICE 'd'
#define POLLICE 'c'

/*
 * crea un istanza di Dito con il carattere associato
 * indicato in charAss
 */
Dito* newDito(char charAss){
	Dito nuovoDito;
	nuovoDito.char_associato=charAss;
	Dito *nd = &nuovoDito;
	return nd;
}

/*
 * modifica il carattere associato ad un dito
 */
void modCharAssDito(Dito* dito, char newChar){
	dito->char_associato = newChar;
}

/*
 * preme un dito se non è premuto quando chiamata
 */
void premi(Dito* dito, xdo_t * x){
	if(dito->premuto = 0){
		const char * charAss = &dito->char_associato;
		xdo_send_keysequence_window_down(x, CURRENTWINDOW, charAss, 100);
		dito->premuto = 1;
	}
}

/*
 * rilascia un dito se è premuto quando chiamata
 */
void rilascia(Dito* dito, xdo_t * x){
	if(dito->premuto = 1){
		const char * charAss = &dito->char_associato;
		xdo_send_keysequence_window_up(x, CURRENTWINDOW, charAss, 100);
		dito->premuto = 0;
	}
}

/*
 * inizializza e restituisce un puntatore al controller
 * con i valori di default vedi inizio file
 */
Controller* newController(){
	Controller newController;
	newController.mignolo = newDito(MIGNOLO);
	newController.anulare = newDito(ANULARE);
	newController.medio   = newDito(MEDIO);
	newController.indice  = newDito(INDICE);
	newController.pollice = newDito(POLLICE);
	Controller *nc = &newController;
	return nc;
}

/*
 * inizializza e restituisce un puntatore al controller
 * con valori personalizzati
 */
Controller* newPersController(char mig,char anu,char med,char ind,char pol){
	Controller newController;
	newController.mignolo = newDito(mig);
	newController.anulare = newDito(anu);
	newController.medio   = newDito(med);
	newController.indice  = newDito(ind);
	newController.pollice = newDito(pol);
	Controller *nc = &newController;
	return nc;
}

/*
 * stampa i settaggi del contoller
 */
void printControllerSetting(Controller* cnt){

printf("       _.-._     \n"
       "     /|%c|%c|%c|_   \n"
       "     || | | |%c|  \n"
       "     || | | | |  \n"
       "    _||     ` |  \n"
       "   \\`\       ;  \n"
       "    \\%c       |  \n"
       "     \\      /   \n"
       "     | |    |    \n"
       "     | |    |    \n",
       cnt->indice->char_associato,
       cnt->medio->char_associato,
       cnt->anulare->char_associato,
       cnt->mignolo->char_associato,
       cnt->pollice->char_associato);

}



