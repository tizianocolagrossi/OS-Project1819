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

#define MAX_SIZE 50
#define DEBUG 1

/*
 * funzione per printate di debug
 */
void debugPrint(char *msg, char *val){
	if(DEBUG) printf("%s %s", msg, val);
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
		free(buf);
		return 0;
	}
	else {
		free(buf);
		return 1;
	}
}

int main(int argc, char **argv){
	init_shell();
	char inStr[MAX_SIZE];
	int i = getCmd(inStr);
	debugPrint("DEBUG CMD IN", inStr);
	help();
}
