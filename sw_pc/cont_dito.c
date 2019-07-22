/*
 * CONTROLLER & DITO CONTROLLER & DITO CONTROLLER & DITO CONTROLLER  
 *
 * funzioni per controller e dito
 * 
 */


#include <cont_sett_struct.h>
#include <xdo.h>

#define MIGNOLO 's'
#define ANULARE 'w'
#define MEDIO 'a'
#define INDICE 'd'
#define POLLICE 'SPACE'

/*
 * crea un istanza di Dito con il carattere associato
 * indicato in charAss
 */
Dito* newDito(char charAss){
	Dito nuovoDito;
	nuovoDito.char_associato=charAss;
	return &nuovoDito;
}

/*
 * modifica il carattere associato ad un dito
 */
void modCharAssDito(Dito* dito, char newChar){
	dito.char_associato = newChar;
}

/*
 * preme un dito se non è premuto quando chiamata
 */
void premi(Dito* dito, xdo_t * x){
	if(dito.premuto = 0){
		xdo_send_keysequence_window_down(x, CURRENTWINDOW, dito.char_associato, 100);
		dito.premuto = 1;
	}
}

/*
 * rilascia un dito se è premuto quando chiamata
 */
void rilascia(Dito* dito, xdo_t * x){
	if(dito.premuto = 1){
		xdo_send_keysequence_window_up(x, CURRENTWINDOW, dito.char_associato, 100);
		dito.premuto = 0;
	}
}

/*
 * inizializza e restituisce un puntatore al controller
 * con i valori di default vedi inizio file
 */
Controller* newInitDefaultColtroller(){
	Controller newController;
	newController.mignolo = MIGNOLO;
	newController.anulare = ANULARE;
	newController.medio   = MEDIO;
	newController.indice  = INDICE;
	newController.pollice = POLLICE;
	return &newController;
}

/*
 * inizializza e restituisce un puntatore al controller
 * con valori personalizzati
 */
Controller* newInitPersonalizedController(char mig,char anu,char med,char ind,char pol){
	Controller newController;
	newController.mignolo = min;
	newController.anulare = anu;
	newController.medio   = med;
	newController.indice  = ind;
	newController.pollice = pol;
	return &newController;
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
       cnt.indice.char_associato,
       cnt.medio.char_associato,
       cnt.anulare.char_associato,
       cnt.mignolo.char_associato,
       cnt.pollice.char_associato);

}



