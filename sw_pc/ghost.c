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
#include <signal.h> // POSIX signal handler
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // per la sleep
#include <pthread.h>

#define MAX_SIZE 150 // max caratteri in input
#define MAX_CMD 10  // max comandi per linea supportati
#define DEBUG 1

#define MIN_SOGL_VAL 700
#define MAX_SOGL_VAL 999 
#define NUM_CALIB_SAMPLES 1000

volatile sig_atomic_t termReq = 0; //Flag per terminazione 

//michele
#include "set_finger.h"
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#define MAX_SIZE_ 25
#define BAUDRATE B19200


//michele: struct per inizializzazione porta seriale
struct termios current;
int vett[5];//debug

//davide: global variables to handle data read from serial
char current_num[4];
int current_finger = 0; //0,1,2,3,4
int hand[5];
int structure_ready = 0; //it will be set to 1 when hand is filled
int calib_threesholds[5]; //in case of calibration request


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
 * Tiziano handler per SIGKILL o SIGINT o SIGTERM
 * setta un flag che viene controllato sempre dal main 
 */
 void interrupt_handler(int sig, siginfo_t *siginfo, void *context){
	//handler
	debugPrintMsg("Dentro interrupt handler");
	termReq = 1;
}

/*
 * Tiziano test per il controller
 */
void test(Controller* cnt, char** parsed){
		xdo_t * x = cnt->xdo;
		if(parsed[1]==NULL)return;
		sleep(3);
		if(strcmp(parsed[1],"-mSngStrk")==0){
			printf(
				"TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST\n"
			);
			setElemento(cnt,mignolo);
			setState(cnt);
			resetElemento(cnt, mignolo);
			setState(cnt);
			setElemento(cnt,pollice);
			setState(cnt);
			resetElemento(cnt, pollice);
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
			printf("\n");
			for(int i = 1; i<cnt->size; i++){
				setElemento(cnt,i);
				setState(cnt);
				sleep(1);
				resetElemento(cnt, i);
				setState(cnt);
				printf("\n");
			}
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
			"\n\n"
			"Digita \"help\" o \"h\" per ricevere informazioni per l'utilizzo della shell\n"
			"\n\n"
			"Digita \"quit\" ,\"q\" o \"exit\" per chiuedere la shell\n"
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
		"HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP\n"
		"LP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP H\n"
		"P HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HEL\n"
		"\n\n"
		"- controller\n"
		"\tVisualizza stato controller [ATTIVO o NON ATTIVO]\n"
		"\tVisualizza la configurazione corrente del controller\n"
		"\n\n"
		"- controller -m [mignolo|anulare|medio|indice|pollice] [newChar]\n"
		"\tnewChar -> nuovo carattere da associare al dito in [mignolo|...]\n"
		"\n\n"
		"- controller -s [valore soglia (intero compreso tra %d e %d)]\n"
		"\tSetta il valore minimo [angolo piega] per l'attivazione del tasto\n"
		"\n\n"
		"- start\n" 
		"\tAvvia il controller\n"
		"\n\n"
		"- stop\n"
		"\tarresta il controller\n"
		"\n\n"
		"- quit o exit\n"
		"\tesce dalla shell\n"
		"\n\n"
		"HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP\n"
		"LP HELP HELP HELP HELP HELP HinteroELP HELP HELP HELP HELP HELP HELP HELP HELP H\n"
		"P HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HEL\n"
		,MIN_SOGL_VAL, MAX_SOGL_VAL
	);
}

void sayHi(char **parsed){
	if(parsed[1]==NULL){
		printf("ERRORE: nessun argomento. Consulta la sezione help per la guida\n");
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


//davide: function to read from the serial port and set fingers in the struct
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
		//we add the number to the array, reset the string, reset finger number and trigger "structure_ready"
		if(c == 45) {
			//printf("\n");
			int value = atoi(current_num);
			hand[current_finger] = value;
			strcpy(current_num, "");
			current_finger = 0; //restart from thumb finger
			structure_ready = 1;
		}
		//davide: if c is digit add it to the string
		else if(c >= 48 && c <= 57){
			//printf("%c", c);
			strncat(current_num, (buf+i), 1);
		}
		//davide: if c is a comma add the number to the array, reset the string, increment finger number
		else if(c == 44){
			//printf("%c", c);
			int value = atoi(current_num);
			hand[current_finger] = value;
			strcpy(current_num, "");
			current_finger++;
			if(current_finger > 4) current_finger = 0; //just to be sure
			
		}
	}
}

//michele: funzione di debug per scrittura
int* debug_(Controller* cnt){
	
	sleep(5);
	int i = 0;
	while(1){
		vett[i] = rand() % 999;
		if(i == 4){
			set_finger(cnt, MIN_SOGL_VAL, vett);
			memset(vett, 0, sizeof(vett));
			i = 0;
		}
		else{
			i++;
		}
	}
}

//michele: thread di debug
void *playCnt_(void* cnt){
	while(1){
		debug_(cnt); 
	}
}

//michele: thread quello buono
void *playCnt(void* cnt) {
	
	int fd = port_configure();
	//davide: if connection not created, return to shell
	if(fd<0) {
		printf("Please verify connection of your controller");
		return NULL;
	}
	// davide: set to empty string
	strcpy(current_num, "");
	//davide: reading from serial forever
	while(1){
		read_(fd);
		if(structure_ready){
			//michele: chiama funzione settaggio dito
			set_finger(cnt, MIN_SOGL_VAL, hand);
			structure_ready = 0;
		}
	}
}

//davide: function to compute average value of given array
int average(int* array, int size){
	if(!array || size < 1) return -1;
	int i;
	int sum = 0;
	for(i=0; i<size; i++){
		sum += array[i];
	}
	return sum/size;
}

//davide: thread for initial calibration
//TODO - POSSO CAMBIARE APPROCCIO, FORSE è LEGGERMENTE PIù VELOCE
//TODO - CHIEDETEMI COME APPENA SPIZZATE LA FUNZIONE CHE VI SPIEGO L'ALTRA OPZIONE
//prova
void *calibration(void){
	int counter;
	int min[5];
	int max[5];
	int i;
	
	int fd = port_configure();
	//davide: if connection not created, return to shell
	if(fd<0) {
		printf("Please verify connection of your controller\n");
		return NULL;
	}
	
	//davide: allocate a matrix[5 * NUM_CALIB_SAMPLES] and initialize to zero
	int** calib_matrix = (int**) malloc(sizeof(int*)*5);
	for(i=0; i<5; i++){
		calib_matrix[i] = (int*) calloc(NUM_CALIB_SAMPLES, sizeof(int));
	}
	
	//davide: first let's get minimum values
	counter = 0;
	
	//davide: set to empty string
	strcpy(current_num, "");
	printf("Please keep you hand opened and firm\n");
	sleep(2);
	while(counter < NUM_CALIB_SAMPLES){
		//davide: read from serial and fill calib_matrix with minimum values
		read_(fd);
		if(structure_ready){
			for(i=0; i<5; i++){
				calib_matrix[i][counter] = hand[i];
			}
			counter++;
			structure_ready = 0;
		}
	}
	
	//davide: get average minimum values for each finger
	for(i=0; i<5; i++){
		int val = average(calib_matrix[i], NUM_CALIB_SAMPLES);
		min[i] = val;
	}
	printf("DONE!\n");
	
	//davide: restart and get maximum values
	counter = 0;
	
	//davide: reset to empty string, just to be sure
	strcpy(current_num, "");
	printf("Please keep you hand close and firm\n");
	sleep(2);
	while(counter < NUM_CALIB_SAMPLES){
		//davide: read from serial and fill calib_matrix with maximum values
		read_(fd);
		if(structure_ready){
			for(i=0; i<5; i++){
				calib_matrix[i][counter] = hand[i];
			}
			counter++;
			structure_ready = 0;
		}
	}
	
	//davide: get average maximum values for each finger
	for(i=0; i<5; i++){
		int val = average(calib_matrix[i], NUM_CALIB_SAMPLES);
		max[i] = val;
	}
	printf("DONE!\n");
	
	//davide: set global variable
	for(i=0; i<5; i++){
		int val = (min[i] + max[i]) / 2;
		calib_threesholds[i] = val;
	}
	printf("\n----------CALIBRATION SUCCESFULLY COMPLETED!----------\n");
	
	//davide: free allocated memory and return to shell
	for(i=0; i<5; i++) free(calib_matrix[i]);
	free(calib_matrix);
	
	return NULL;
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
 * michele
 * funzione che lancia un thread per il controller
 * se digitato in aggiunta "-t" lancia il thread di debug
 */
void start(Controller* cnt, char **parsed){
	pthread_t thread_id_; 
	pthread_t thread_id; 
    debugPrintMsg("Sto per lanciare il thread"); 
    
    if (parsed[1] == NULL){
    	pthread_create(&thread_id, NULL, playCnt, cnt);
    	cnt->t_id = (void*) thread_id;
    }
    
    else if(strcmp(parsed[1],"-t")==0){
    	pthread_create(&thread_id, NULL, playCnt_, cnt);
    	cnt->t_id = (void*) thread_id_;
    }

}

/*
 * Tiziano
 * funzione che ferma il thread per il controller
 */
void stop(Controller* cnt){
	pthread_t thread_id = (pthread_t)cnt->t_id;
	pthread_cancel(thread_id);
	debugPrintMsg("Controller fermato\n");
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
			printf("\t[USAGE] controller -m [mignolo|anulare|...] [carattere da assegnare]\n\n");
			return;
		}
		enum tipoElemento sw1 = -1;
		if(strcmp(parsed[2],"mignolo")==0)sw1=mignolo;
		if(strcmp(parsed[2],"anulare")==0)sw1=anulare;
		if(strcmp(parsed[2],"medio")==0)sw1=medio;
		if(strcmp(parsed[2],"indice")==0)sw1=indice;
		if(strcmp(parsed[2],"pollice")==0)sw1=pollice;
		
		//michele 
		if(parsed[3] == NULL) printf("\t[USAGE] controller -m [mignolo|anulare|...] [carattere da assegnare]\n\n");
		else {
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
					printf("Le dita disponibili per modificare i settaggi sono:\n"
						   "\tmignolo|anulare|medio|indice|pollice\n");
					break;
			}
		}
	}
	if(strcmp(parsed[1],"-s")==0){
		if(parsed[2] != NULL){
			int val = atoi(parsed[2]);
			if(val!=0 && val > MIN_SOGL_VAL && val < MAX_SOGL_VAL ){
				printf(
					"CONTROLLER EDIT SOGLIA CONTROLLER EDIT SOGLIA CONTROLLER EDIT SOGLIA CON\n"
					"TROLLER EDIT SOGLIA CONTROLLER EDIT SOGLIA CONTROLLER EDIT SOGLIA CONTRO\n"
					"\n"
				);
				setSoglia(cnt, val); //michele
				printf("\tsoglia impostata a %d\n\n",val);
			
			}else{
				printf("\tIl valore deve essere compreso tra %d e %d\n\n\n", MIN_SOGL_VAL, MAX_SOGL_VAL);
			}
		}
		else{
			printf("\t[USAGE] controller -s [intero tra %d e %d]\n\n", MIN_SOGL_VAL, MAX_SOGL_VAL);
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
	printf("\tARRIVEDERCI, NON FA DANNI ME RACCOMANDO !");
	printf("\n\n");
	exit(0);
}

/*
 * Tiziano
 * funzione per la gestione dei comandi della shell
 */
 
int cmdHandler(char** parsed, Controller *cnt){
	debugPrintMsg("dentroCmdHandler");
	if(parsed[0]==NULL)parsed[0]="";
	int nCmdSupportati=10, i, switchArg=100;
	char* ListCmd[nCmdSupportati];

   	ListCmd[0]="help";
   	ListCmd[1]="h";
   	ListCmd[2]="quit";
   	ListCmd[3]="q"; //michele: ho aggiunto questo, è più comodo
   	ListCmd[4]="exit";
   	ListCmd[5]="hi";
   	ListCmd[6]="controller";
   	ListCmd[7]="start";
   	ListCmd[8]="stop";
   	ListCmd[9]="test";
   	
   	

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
		case 4:
			quitShell(cnt);
		case 5:
			sayHi(parsed);
			break;
		case 6:
			controller(parsed, cnt);
			break;
		case 7:
			start(cnt, parsed);
			break;
		case 8:
			stop(cnt);
			break;
		case 9:
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


/*
 * Tiziano: iniziallizzazione shell, struct e int_handler
 */
int main(int argc, char **argv){

	struct sigaction act;
	memset (&act, '\0', sizeof(act));
	/* Use the sa_sigaction field because the handles has two additional parameters */
	act.sa_sigaction = &interrupt_handler;
	/* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
	act.sa_flags = SA_SIGINFO;
	
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGKILL, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
	
    
	char inStr[MAX_SIZE], *parsedArg[MAX_CMD];
	
	init_shell();

	Controller cnt;
	Controller_init(&cnt);
	
	while(!termReq){
		if(getCmd(inStr)) continue;
		parseString(inStr, parsedArg, &cnt);
	}
	//Tiziano: se arriva qui è perche ha ricevito il segnale di SIGINT SIGKILL o SIGTERM
	//	quindi pulisco quello che devo pulire
	quitShell(&cnt);
}


