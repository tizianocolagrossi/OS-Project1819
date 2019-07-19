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
		"TODO help da scrivere\n\n"
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
		strcpy(cmd, buf);
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
	
	int nCmdSupportati=5, i, switchArg=100;
	char* ListCmd[nCmdSupportati];

   	ListCmd[0]="help";
   	ListCmd[1]="h";
   	ListCmd[2]="quit";
   	ListCmd[3]="exit";
   	ListCmd[4]="hi";

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

	while(1){
		if(getCmd(inStr)) continue;
		parseString(inStr, parsedArg);
		
	
	}
}
