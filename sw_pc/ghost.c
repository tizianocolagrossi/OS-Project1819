/*
 * SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL
 *
 * OS-prj1819
 * 
 * shell per il progetto di sistemi operativi anno 2018/19
 * 
 */

#include "cont_sett_struct.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // per la sleep
#include <pthread.h> 


#define MAX_SIZE 50 // max caratteri in input
#define MAX_CMD 10  // max comandi per linea supportati
#define DEBUG 1

/*
 * funzione per printate di debug
 */
void debugPrint(char *msg, char *val){
	if(DEBUG) printf("\n%s %s\n", msg, val);
}
void debugPrintInt(char *msg, int val){
	if(DEBUG) printf("\n%s %d\n", msg, val);
}
void debugPrintMsg(char *msg){
	if(DEBUG) printf("\n%s\n", msg);
}


/*
 * scrive a schermo una piccola intro...
 * l'ho fatta per stare tranquillo a scrivere
 */

void init_shell(){
	printf("##########################################################################\n");
	printf("#                     SISTEMI OPERATIVI PRJ SHELL                        #\n");                                                     
	printf("##########################################################################\n");                                                     
}

/*
 * HELP per la shell
 */

void help(){
	printf(
		"\n"
		"##########################################################################\n"
		"HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP\n"
		"\n"
		"\nComandi riconosciuti\n\n"
		"controller: stampa lo status del cotroller ONLINE OFFLINE e i settaggi vari\n"
		"controller -m modifica i settaggi del controller\n"
		"\tcontroller -m[mignolo|anulare|medio|indice|pollice] [comando associato]\n"
		"\tESEMPIO: controller -m mignolo w\n"
		"\til mignolo trigghera il tasto s\n"
		"\n\n"
		"start avvia il controller in modo che potra essere usato su altri programmi\n"
	);
}

void sayHi(char **parsed){
	if(parsed[1]==NULL){
		printf("ERRORE: non hai passato l'argomento vedi help per la guida\n");
	} else{
		printf("Ciao %s\n", parsed[1]);
	}
}

/*
 * funzione che gestisce il thread (valori virtualizzazione etc)______________________________________________________________________
 * ###################################################################################################################################
 * MICHELE MICHELE MICHELE MICHELE MICHELE MICHELE MICHELE MICHELE MICHELE MICHELE MICHELE MICHELE MICHELE MICHELE MICHELE MICHELE MIC
 *
 * le funzioni che te servono:
 * void setElemento(Controller* cnt, enum tipoElemento tipo);
 * void resetElemento(Controller* cnt, enum tipoElemento tipo);
 * void setState(Controller* cnt);
 *
 * cnt è quello che ti passo nella funzione void *playCnt(void* cnt <QUESTO QUA)
 * enum tipoElemento tipo è > enum tipoElemento{mignolo, anulare, medio, indice, pollice};
 * quindi se vuoi settare im mignolo attivo fai
 * 		setElemento(cnt, mignolo);
 * idem se vuoi resettarlo (ovvero rilasciare il tasto associato)
 * 		resetElemento(cnt, mignolo);
 * cosa importante è che alla fine te lanci la funzione setState(cnt);
 * perche serve per rendere effettive le modifiche che te hai fatto alla struttura
 * ovvero serve per virtualizzare il premere dei tasti della tastiera
 * esempio:
 * 		leggi da seriale;
 * 
 * 		setElemento(cnt, mignolo);
 * 		setElemento(cnt, pollice);
 * 		resetElemento(cnt, indice);
 * 		resetElemento(cnt, medio);
 * 		resetElemento(cnt, anulare);
 * 
 * 		setState(cnt);
 *
 * 		ripeti poi per ogni volta che ricevi dati
 *
 * puoi anche non settare o resettare tutte le dita insieme puoi anche solo modificare le dita che cambiano
 * ovvero non è necessario che imposti ogni volta lo stato di ogni dito, il dito mantiene lo stato che ha fino
 * a che non lo cambi te		
 */
void *playCnt(void* cnt) {
	//puoi scrivere roba anche qua, non so per inizializzare la comunicazione fai te
	while(1){
		//togli sta sleep quando scrivi e fai un po quello che vuoi qua
		//una funzione nuova che fai su un file esterno un po quello che te pare
		sleep(3);
	}
    return NULL; 
} 
/*
 * funzione che resetta lo stato a tutti i tasti del controller
 */
void clearCnt(Controller* cnt){
	for(int i = 0; i<cnt->size; i++){
		resetElemento(cnt, i);
	}
	setState(cnt);
}

/*
 * funzione che lancia un thread per il controller
 */
void start(Controller* cnt){
	pthread_t thread_id; 
    debugPrintMsg("Sto per lanciare il thread"); 
    pthread_create(&thread_id, NULL, playCnt, cnt);
    cnt->t_id = (void*) thread_id;
}

/*
 * funzione che ferma il thread per il controller
 */
void stop(Controller* cnt){
	pthread_t thread_id = (pthread_t)cnt->t_id;
	pthread_cancel(thread_id);
	debugPrintMsg("Thread finito\n");
	//per sicurezza rilascia tutti i tasti del controller
	clearCnt(cnt);
}

/*
 * funzione per il controllo del controller
 */
void controller(char **parsed, Controller *cnt){
	if(parsed[1]==NULL){
		printControllerSetting(cnt);
		return;
	}
	if(strcmp(parsed[1],"-m")==0){
		if(parsed[2]==NULL){
			printf("devi inserire il dito di cui vuoi modificare il settaggio\n");
			return;
		}
		enum tipoElemento sw1 = -1;
		if(strcmp(parsed[2],"mignolo")==0)sw1=mignolo;
		if(strcmp(parsed[2],"anulare")==0)sw1=anulare;
		if(strcmp(parsed[2],"medio")==0)sw1=medio;
		if(strcmp(parsed[2],"indice")==0)sw1=indice;
		if(strcmp(parsed[2],"pollice")==0)sw1=pollice;
		switch (sw1){
			case 0:
				editElemCharAss(cnt, mignolo, parsed[3][0]);
				break;
			case 1:
				editElemCharAss(cnt, anulare, parsed[3][0]);
				break;
			case 2:
				editElemCharAss(cnt, medio, parsed[3][0]);
				break;
			case 3:
				editElemCharAss(cnt, indice, parsed[3][0]);
				break;
			case 4:
				editElemCharAss(cnt, pollice, parsed[3][0]);
				break;
			default:
				printf("le dita disponibili per modificare i settaggi sono:\n"
					   "\tmignolo|anulare|medio|indice|pollice\n");
				break;
		}
	}
}

/*
 * funzione per prendere comandi
 */

int getCmd(char* cmd){
	size_t characters;
	size_t bufsize = MAX_SIZE;

    printf("#> ");
// la funzione getline prende i comandi da stdin e indico la dimensione del
// buffer allocato sopra e l'indirizzo del buffer su cui scrivere    
	characters = getline(&cmd,&bufsize,stdin);
	if(characters != 0){
		//strncpy(cmd, buf, characters);
		debugPrint("FUN getCmd value >", cmd);
		return 0;
	}
	else {
		return 1;
	}
}

/*
 * funzione per lo split della tringa in input
 */
 
int splitString(char *str, char **split){
	int i;
	int len = strlen(str);
	str[len-1]='\0'; // elimino il carattere \n alla fine preso dalla getline
	debugPrint("comando preso>",str);
	for(i = 0; i < MAX_CMD; i++){
		split[i] = strsep(&str," ");
		if(split[i]==NULL) break;
		if(strlen(split[i])==0) i--;
	}
	
}

/*
 * funzione per chiudere il programma
 */
void quitShell(Controller* cnt){
	pthread_t id = (pthread_t)cnt->t_id;
	pthread_cancel(thread_id);
	pthread_join(id, NULL);
	//per sicurezza rilascia tutti i tasti del controller
	clearCnt(cnt);
	free(cnt->elementi);
	cntXdoFree(cnt);
	printf("ARRIVEDERCI, NON FA DANNI ME RACCOMANDO !\n");
	exit(0);
}

/*
 * funzione per la gestione dei comandi della shell
 */
 
int cmdHandler(char** parsed, Controller *cnt){
	
	debugPrintMsg("dentroCmdHandler");
	
	int nCmdSupportati=8, i, switchArg=100;
	char* ListCmd[nCmdSupportati];

   	ListCmd[0]="help";
   	ListCmd[1]="h";
   	ListCmd[2]="quit";
   	ListCmd[3]="exit";
   	ListCmd[4]="hi";
   	ListCmd[5]="controller";
   	ListCmd[6]="start";
   	ListCmd[7]="stop";
   	

	debugPrintMsg("prima del for");
	debugPrint("parsed[0] in cmdHandler >", parsed[0]);
   	for(i=0; i<nCmdSupportati; i++ ){
		if(strcmp(parsed[0], ListCmd[i])==0){
			switchArg = i;
			debugPrintInt("FUN cmdHandler valore della i >", i);
			break;
		}
	}
	debugPrintMsg("dopo del for");

	switch(switchArg){
		case 0:
		case 1:
			help();
			return 1; // perche usato dall if che chiama cmdHandler!
		case 2:
		case 3:
			quitShell(cnt);
		case 4:
			sayHi(parsed);
			break;
		case 5:
			controller(parsed, cnt);
			break;
		case 6:
			start(cnt);
			break;
		case 7:
			stop(cnt);
			break;
		default:
			printf("comando non riconosciuto, digit h o help per vedere la guida\n");
			break;
	}
}

/*
 * funzione per il parse della stringa in input
 */
 
int parseString(char *str, char** parsed, Controller *cnt){
	splitString(str, parsed);
	if (cmdHandler(parsed, cnt)) return 0;
	else return 1; 
}



int main(int argc, char **argv){
	
	char inStr[MAX_SIZE], *parsedArg[MAX_CMD];
	
	init_shell();

	Controller cnt;
	Controller_init(&cnt);
	
	while(1){
		if(getCmd(inStr)) continue;
		parseString(inStr, parsedArg, &cnt);
		
	
	}
}
