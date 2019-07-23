/*
 * SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL
 *
 * OS-prj1819
 * 
 * shell per il progetto di sistemi operativi anno 2018/19
 * 
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cont_sett_struct.h"

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
 * funzione che lancia un thread per il controller
 */
void start(){
	printf("START\n");
}

/*
 * funzione per il controllo del controller
 */
void controller(char **parsed){
	int sw1= -1;
	if( NULL == parsed[1])sw1=0;
	if( ""   == parsed[1])sw1=1;
	if( "-m" == parsed[1])sw1=2;
	switch (sw1){
		case 0:
			printf("ERRORE parsed[1] NULL\n");
		case 1:
			printf("status del controller\n");
			break;
		case 2:
			if(parsed[2]==NULL||parsed[2]==""){
				printf("ERRORE devi indicare quale dito devi modificare\n");
				break;
			}
			if(parsed[3]==NULL||parsed[3]==""){
				printf("ERRORE devi indicare il tasto da associare\n");
				break;
			}
			int sw2 =-1;
			if("mignolo" ==parsed[2])sw2=0;
			if("anulare" ==parsed[2])sw2=1;
			if("medio"   ==parsed[2])sw2=2;
			if("indice"  ==parsed[2])sw2=3;
			if("pollice" ==parsed[2])sw2=4;
			switch (sw2){
				case 0:
					printf("MOD mignolo in %c\n", parsed[3][0]);
					break;
				case 1:
					printf("MOD anulare in %c\n", parsed[3][0]);
					break;
				case 2:
					printf("MOD medio in %c\n", parsed[3][0]);
					break;
				case 3:
					printf("MOD indice in %c\n", parsed[3][0]);
					break;
				case 4:
					printf("MOD pollice in %c\n", parsed[3][0]);
					break;
				default:
					printf("%s non rientra tra i valori che puoi inserire\n che sono: mignolo|anulare|medio|indice|pollice", parsed[2]);
					break;
				
			}
			break;
		default:
			printf("comando non riconosiuto\n");
			break;
	}
}

/*
 * funzione per prendere comandi
 */

int getCmd(char* cmd){
	char *buf;
	size_t bufsize = MAX_SIZE;
	size_t characters;

	buf = (char *)malloc(bufsize * sizeof(char));
    if( buf == NULL){
        perror("Impossibile allocare il buffer");
        exit(1);
    }
    printf("#> ");
	characters = getline(&buf,&bufsize,stdin);
	if(characters != 0){
		strcpy(cmd, buf);  //TODO Davide suggerisce: dovresti usare strncpy(char* dest, const char* source, size_t n), grisetti piange
		debugPrint("FUN getCmd value >", cmd);
		free(buf);
		return 0;
	}
	else {
		free(buf);
		return 1;
	}
}

/*
 * funzione per lo split della tringa in input
 */
 
int splitString(char *str, char **split){
	int i;
	str = strsep(&str, "\n"); // elimino il carattere \n alla fine preso dalla getline
	for(i = 0; i < MAX_CMD; i++){
		split[i] = strsep(&str," ");
		if(split[i]==NULL) break;
		if(strlen(split[i])==0) i--;
	}
}

/*
 * funzione per la gestione dei comandi della shell
 */
 
int cmdHandler(char** parsed){
	
	debugPrintMsg("dentroCmdHandler");
	
	int nCmdSupportati=7, i, switchArg=100;
	char* ListCmd[nCmdSupportati];

   	ListCmd[0]="help";
   	ListCmd[1]="h";
   	ListCmd[2]="quit";
   	ListCmd[3]="exit";
   	ListCmd[4]="hi";
   	ListCmd[5]="controller";
   	ListCmd[6]="start";
   	

	debugPrintMsg("prima del for");
	
   	for(i=0; i<nCmdSupportati; i++ ){
		debugPrint("parsed[0] in cmdHandler >", parsed[0]);
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
			printf("ARRIVEDERCI, NON FA DANNI ME RACCOMANDO !\n");
			exit(0);
		case 4:
			sayHi(parsed);
			break;
		case 5:
			controller(parsed);
			break;
		case 6:
			start();
			break;
		default:
			printf("comando non riconosciuto, digit h o help per vedere la guida\n");
			break;
	}
}

/*
 * funzione per il parse della stringa in input
 */
 
int parseString(char *str, char** parsed){
	splitString(str, parsed);
	if (cmdHandler(parsed)) return 0;
	else return 1; 
}





int main(int argc, char **argv){
	
	char inStr[MAX_SIZE], *parsedArg[MAX_CMD];
	
	init_shell();

	struct Controller *my_controller = newController();
	
	while(1){
		if(getCmd(inStr)) continue;
		parseString(inStr, parsedArg);
		
	
	}
}
