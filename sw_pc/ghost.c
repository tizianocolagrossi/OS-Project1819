/*
 * SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL
 *
 * OS-prj1819
 * 
 * shell per il progetto di sistemi operativi anno 2018/19
 * 
 */

//tiziano
#include "cont_sett_struct.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // per la sleep
#include <pthread.h> 
//michele
#include "set_finger.h"
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */


#define MAX_SIZE 150 // max caratteri in input
#define MAX_CMD 10  // max comandi per linea supportati
#define DEBUG 1

#define MIN_SOGL_VAL 600
#define MAX_SOGL_VAL 900

//michele
#define MAX_SIZE_ 25
#define BAUDRATE B19200

//michele: struct per inizializzazione porta seriale
struct termios current;
/*
 * Tiziano
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
 * Tiziano
 */
void test(Controller* cnt, char** parsed){
		xdo_t * x = cnt->xdo;
		if(parsed[1]==NULL)return;
		if(strcmp(parsed[1],"-mSngStrk")==0){
			printf(
				"TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST\n"
			);
			setElemento(cnt,mignolo);
			setState(cnt);
			resetElemento(cnt, mignolo);
			setState(cnt);
			setElemento(cnt,anulare);
			setState(cnt);
			resetElemento(cnt, anulare);
			setState(cnt);
			setElemento(cnt,medio);
			setState(cnt);
			resetElemento(cnt, medio);
			setState(cnt);
			setElemento(cnt,indice);
			setState(cnt);
			resetElemento(cnt, indice);
			setState(cnt);
			setElemento(cnt,pollice);
			setState(cnt);
			resetElemento(cnt, pollice);
			setState(cnt);
		}
		if(strcmp(parsed[1],"-mLngStrk")==0){
			printf(
				"TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST\n"
			);
			setElemento(cnt,mignolo);
			setElemento(cnt,mignolo);
			setState(cnt);
			sleep(1);
			resetElemento(cnt, mignolo);
			setState(cnt);
			setElemento(cnt,anulare);
			setState(cnt);
			sleep(1);
			resetElemento(cnt, anulare);
			setState(cnt);
			setElemento(cnt,medio);
			setState(cnt);
			sleep(1);
			resetElemento(cnt, medio);
			setState(cnt);
			setElemento(cnt,indice);
			setState(cnt);
			sleep(1);
			resetElemento(cnt, indice);
			setState(cnt);
			setElemento(cnt,pollice);
			setState(cnt);
			sleep(1);
			resetElemento(cnt, pollice);
			setState(cnt);
			printf("\n");
		}
		
		

}

/*
 * Tiziano
 * scrive a schermo una piccola intro...
 * l'ho fatta per stare tranquillo a scrivere
 */

void init_shell(){
	printf(
			"SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SH\n"
			"HELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHE\n"
			"ELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHEL\n"
			"LL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL\n"
			"L SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL \n"
			" SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL S\n"
			"SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SH\n"
			"##########################################################################\n"
			"#                     SISTEMI OPERATIVI PRJ SHELL                        #\n"                                                   
			"##########################################################################\n"
			"SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SH\n"
			"HELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHE\n"
			"ELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHEL\n"
			"LL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL\n"
			"L SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL \n"
			" SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL S\n"
			"SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SH\n"
			"\n\n\n"
			);                                                     
}

/*
 * Tiziano
 * HELP per la shell
 */

void help(){
	printf(
		"\n"
		"LP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP H\n"
		"HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP\n"
		"\n"
		"controller\n"
		"\tscrive su schermo lo stato del controller ATTIVO o INATTIVO\n"
		"\te scrive la configurazione del controller\n"
		"\n"
		"\tcontroller -m [mignolo|anulare|medio|indice|pollice] [newChar]\n"
		"\tcon newChar il nuovo carattere da associare al dito\n"
		"\n\n"
		"\tcontroller -s [valore soglia (intero compreso tra %d e %d)]"
		"\n\n"
		"start avvia il controller in modo che potra essere usato su altri programmi\n"
		"stop arresta il funzionamento del controller \n"
		"\n\n"
		"quit o exit\n"
		"\tesce dalla shell\n"
		"\n\n"
		"LP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP H\n"
		"HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP\n"
		,MIN_SOGL_VAL, MAX_SOGL_VAL
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
 
// michele: function to set the right bit in termios structure
// to enable the comunication throught the serial port
int port_configure(void){

	int fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
	
	if(fd == -1) perror("cannot open dev/ttyACM0");
	
	else{
		fcntl(fd, F_SETFL, 0); // ho aggiunto solo questo ma mannaggia alla madonna seriale non ha senso 
		tcgetattr(fd, &current);//salvo valori correnti porta seriale 
		cfsetispeed(&current, B19200);//setto baud rate
		current.c_iflag &= ~IXOFF; //disabilito hw input flow control
		current.c_cflag &= ~PARENB; //nessun bit di parità
 		current.c_cflag &= ~CSTOPB; //1 bit di stop
        current.c_cflag &= ~CSIZE;//
		current.c_cflag |= CS8; //8-bit data
		current.c_cflag |= ( CLOCAL | CREAD ); //abilito ricevitore porta seriale
		current.c_cc[VMIN] = 0; //
		current.c_cc[VTIME] = 0; // se VTIME & VMIN sono == 0 la read funzionerà in polling
		tcsetattr(fd, TCSANOW, &current);
	}
	return fd;
}

//davide: function to read from the serial
//michele: set finger in the struct 
void read_(int fd){
	int i;
	char buf[26];
	int byte_read = 0;
	
	while(byte_read < 20){
		int br = read(fd, buf + byte_read, 1);
		if (byte_read < 0) perror("error during read process");
		else if(br > 0){
			byte_read += br;
			//printf("HO LETTO %d BYTES\n", byte_read);
		}
	}
	for(i=0; i<20; i++){
		char c = buf[i];
		//davide: if c is "-", the string is terminated 
		if(c == 45) printf("\n");
		//davide: if c is digit or "," acceptable = true
		int acceptable = (c >= 48 && c <= 57) || c == 44;
		if(acceptable) printf("%c", c);
	}
	
	//michele: call set finger
	
}

void *playCnt(void* cnt) {

	//michele: chiamo funzioni per comunicazione seriale
	
	int fd = port_configure();
	if(fd<0) perror("[playCnt]errore nel file descriptor");

	int soglia = getSoglia(cnt);
	//int soglia = getSoglia(cnt);
	while(1){
		//reading from serial forever
		read_(fd);
		//printf("\n");
	}
} 
/*
 * Tiziano
 * funzione che resetta lo stato a tutti i tasti del controller
 */
void clearCnt(Controller* cnt){
	for(int i = 0; i<cnt->size; i++){
		resetElemento(cnt, i);
	}
	setState(cnt);
}

/*
 * Tiziano
 * funzione che lancia un thread per il controller
 */
void start(Controller* cnt){
	pthread_t thread_id; 
    debugPrintMsg("Sto per lanciare il thread"); 
    pthread_create(&thread_id, NULL, playCnt, cnt);
    cnt->t_id = (void*) thread_id;
}

/*
 * Tiziano
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
 * Tiziano
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
	if(strcmp(parsed[1],"-s")==0){
		int val = atoi(parsed[2]);
		if(val!=0 && val > MIN_SOGL_VAL && val < MAX_SOGL_VAL ){
			printf(
				"CONTROLLER EDIT SOGLIA CONTROLLER EDIT SOGLIA CONTROLLER EDIT SOGLIA CONTR\n"
				"OLLER EDIT SOGLIA CONTROLLER EDIT SOGLIA CONTROLLER EDIT SOGLIA CONTROLLER\n"
				"\n"
			);
			printf("\tsoglia impostata a %d\n",val);
			
		}else{
			printf("\tIl valore deve essere compreso tra %d e %d\n", MIN_SOGL_VAL, MAX_SOGL_VAL);
		}
	}
}

/*
 * Tiziano
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
 * Tiziano
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
 * Tiziano
 * funzione per chiudere il programma
 */
void quitShell(Controller* cnt){
	pthread_t id = (pthread_t)cnt->t_id;
	pthread_cancel(id);
	pthread_join(id, NULL);
	//per sicurezza rilascia tutti i tasti del controller
	clearCnt(cnt);
	for(int i = 0; i<cnt->size;i++){
		free(cnt->elementi[i].sAss);
	}
	free(cnt->elementi);
	cntXdoFree(cnt);
	printf("ARRIVEDERCI, NON FA DANNI ME RACCOMANDO !\n");
	exit(0);
}

/*
 * Tiziano
 * funzione per la gestione dei comandi della shell
 */
 
int cmdHandler(char** parsed, Controller *cnt){
	debugPrintMsg("dentroCmdHandler");
	if(parsed[0]==NULL)parsed[0]="";
	int nCmdSupportati=9, i, switchArg=100;
	char* ListCmd[nCmdSupportati];

   	ListCmd[0]="help";
   	ListCmd[1]="h";
   	ListCmd[2]="quit";
   	ListCmd[3]="exit";
   	ListCmd[4]="hi";
   	ListCmd[5]="controller";
   	ListCmd[6]="start";
   	ListCmd[7]="stop";
   	ListCmd[8]="test";
   	
   	

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
		case 8:
			test(cnt, parsed);
			break;
		default:
			printf(
				"CAPO LEGGI QUA SE NON SAI CHE FARE CAPO LEGGI QUA SE NON SAI CHE FARE CAPO\n"
				"LEGGI QUA SE NON SAI CHE FARE CAPO LEGGI QUA SE NON SAI CHE FARECAPO LEGGI\n"
				"\ncomando non riconosciuto, digit h o help per vedere la guida\n\n"
				"##########################################################################\n"
			);
			break;
	}
}

/*
 * Tiziano
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


