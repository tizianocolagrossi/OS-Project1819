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
 * funzione per prendere comandi
 */

int getCmd(char* cmd){
	char *buf;
	size_t bufsize = MAX_SIZE;
	size_t characters;

	buf = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL){
        perror("Unable to allocate buffer");
        exit(1);
    }
	characters = getline(&buf,&bufsize,stdin);
	strcpy(cmd, buf); 
}

int main(int argc, char **argv){
	init_shell();
}
