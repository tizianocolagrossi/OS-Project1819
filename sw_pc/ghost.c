/*
 *
 * SHELL for OS-prj1819
 * 
 */

#include "cont_sett_struct.h"
#include "set_finger.h"
#include <signal.h>  // POSIX signal handler
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For sleep()
#include <pthread.h>
#include <fcntl.h>   // File control definitions
#include <errno.h>   // Error number definitions
#include <termios.h> // POSIX terminal control definitions

#define MAX_SIZE 150 // max input characters 
#define MAX_CMD 10   // max commands supported
#define MIN_SOGL_VAL 600
#define MAX_SOGL_VAL 999
#define NUM_CALIB_SAMPLES 1000
#define MAX_SIZE_ 25
#define BAUDRATE B19200
#define DEBUG 1      // set to 1 for debug prints

int termReq = 0; //termination flag
Controller* cnt;  //controller global variable for easy access from functions

//michele: struct for initialization of the serial port
struct termios current;
int vett[5]; //debug

//davide: global variables to handle data read from serial
char current_num[4];
int current_finger = 0; //0,1,2,3,4
int hand[5];
int structure_ready = 0;  //it will be set to 1 when hand is filled
int calib_threesholds[5]; //in case of calibration request
int calib_request = 0;    //it will be set to 1 when a calibration has been done


/*
 * Tiziano
 * functions for debug prints
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
 * Tiziano: test for controller
 */
void test(char** parsed){
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
			"#                     OPERATIVE SYSTEMS PRJ SHELL                        #\n"                                                   
			"##########################################################################\n"
			"SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SH\n"
			"HELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHE\n"
			"ELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHEL\n"
			"LL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL\n"
			"L SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL \n"
			" SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL S\n"
			"SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SHELL SH\n"
			"\n\n"
			"Type \"help\" o \"h\" to get information about shell commands\n"
			"\n\n"
			"Type \"quit\" ,\"q\" or \"exit\" to close the shell\n"
			"\n\n\n"
			);                                                     
}

/*
 * Tiziano
 * HELP for the shell
 */

void help(){
	printf(
		"\n"
		"HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP\n"
		"LP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP H\n"
		"P HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HELP HEL\n"
		"\n\n"
		"- controller\n"
		"\tDisplays state of controller [ACTIVE or NOT ACTIVE]\n"
		"\tand current configuration\n"
		"\n\n"
		"- controller -m {thumb|index|middle|ring|pinkie} {newChar}\n"
		"\tnewChar -> new character to associate to finger {thumb|...]\n"
		"\tSpecial characters:\n"
		"\t1 -> space\n"
		"\t2 -> left\n"
		"\t3 -> right\n"
		"\t4 -> up\n"
		"\t5 -> down\n"
		"\n\n"
		"- controller -s {threeshold_value (integer in [%d , %d]) }\n"
		"\tSets minimum value [bend angle] to trigger the key. Default is %d.\n"
		"\tOn success, this value will be used despite previous calibrations\n"
		"\n\n"
		"- calibrate\n"
		"\tStarts process of calibration of sensors.\n"
		"\tIt will set a dedicated threeshold value for each finger.\n"
		"\tOn success, calibrated values will be used despite previous threesholds' sets\n"
		"\n\n"
		"- start\n" 
		"\tStarts the controller\n"
		"\n\n"
		"- help, h\n"
		"\tDisplays this guide\n"
		"\n\n"
		"- quit, q, exit\n"
		"\tQuits the shell\n"
		"\n\n"
		,MIN_SOGL_VAL, MAX_SOGL_VAL, 800 //"SOGLIA" in cont_sett_struct.c
	);
}

void sayHi(char **parsed){
	if(parsed[1]==NULL){
		printf("ERROR: no argument. Type help or h to display the guide\n");
	} else{
		printf("Hello %s\n", parsed[1]);
	}
}
 
// michele: function to set the right bits in termios structure
// to enable the comunication through the serial port
int port_configure(void){

	int fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
	
	if(fd == -1) perror("cannot open dev/ttyACM0");
	
	else{
		printf("questo è l'fd prima di set %d\n", fd);
		setFd(cnt, fd);
		printf("questo è l'fd dopo set %d\n", (cnt->fd));
		
		fcntl(fd, F_SETFL, 0);
		tcgetattr(fd, &current); //save current values of serial port 
		cfsetispeed(&current, B19200);//set baud rate
		current.c_iflag &= ~IXOFF; //disable hw input flow control
		current.c_cflag &= ~PARENB; //no parity bit
 		current.c_cflag &= ~CSTOPB; //1 stop bit
        current.c_cflag &= ~CSIZE;//
		current.c_cflag |= CS8; //8-bit data
		current.c_cflag |= ( CLOCAL | CREAD ); //enable serial port receiver
		current.c_cc[VMIN] = 0; //
		current.c_cc[VTIME] = 0; // if VTIME & VMIN are == 0 the read will work in polling
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
			//printf("READ %d BYTES\n", byte_read);
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

//michele: debug function for writing
int* debug_(){
	
	sleep(5);
	int i = 0, j = 0;
	while(j <= 20){
		vett[i] = rand() % 999;
		if(i == 4){
			set_finger(cnt, cnt->soglia, vett);
			memset(vett, 0, sizeof(vett));
			i = 0;
		}
		else{
			i++;
		}
		j++;
	}
	return 0;
}

//michele: debug thread
void *debug_thread(){
	debug_();
	return NULL; 
}

//davide: main thread
void *playCnt() {
	
	int fd = port_configure();
	//davide: if connection not created, return to shell
	if(fd<0) {
		printf("Please verify connection of your controller\n");
		return NULL;
	}
	//davide: set to empty string
	strcpy(current_num, "");
	
	//davide: reading from serial forever (until Ctrl+C)
	while(1){
		printf("sono nel while woohoo\n");
		read_(fd);
		if(structure_ready){
			if(calib_request) set_finger_calib(cnt, calib_threesholds, hand);
			else set_finger(cnt, cnt->soglia, hand);
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

//davide: function for calibrate threesholds for each finger
//TODO - POSSO CAMBIARE APPROCCIO, FORSE è LEGGERMENTE PIù VELOCE
void *calibration(){
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
	printf("Please keep your hand opened and firm\n");
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
	sleep(1);
	
	//davide: restart and get maximum values
	counter = 0;
	
	//davide: reset to empty string, just to be sure
	strcpy(current_num, "");
	printf("Please keep your hand closed and firm\n");
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
	
	//davide: set global variable and trigger
	for(i=0; i<5; i++){
		int val = (min[i] + max[i]) / 2;
		calib_threesholds[i] = val;
		printf("%d > %d\n", i, calib_threesholds[i]);
	}
	//calib_threesholds[3] += 30; //uncomment to make ring finger less sensitive (due to pinkie bending)
	calib_request = 1;
	
	printf("\n----------CALIBRATION SUCCESFULLY COMPLETED!----------\n");
	
	//davide: free allocated memory and return to shell
	for(i=0; i<5; i++) free(calib_matrix[i]);
	free(calib_matrix);
	
	return NULL;
}

//davide: optimized calibration, allocate much less memory and does less memory accesses
void *calibration_opt(){
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
	
	//davide: allocate an array[5] and initialize to zero
	int* calib_matrix = (int*) calloc(5, sizeof(int));
	
	//davide: first let's get minimum values
	counter = 0;
	
	//davide: set to empty string
	strcpy(current_num, "");
	printf("Please keep your hand opened and firm\n");
	sleep(2);
	while(counter < NUM_CALIB_SAMPLES){
		//davide: read from serial and fill calib_matrix with minimum values
		read_(fd);
		if(structure_ready){
			for(i=0; i<5; i++){
				calib_matrix[i] += hand[i];
			}
			counter++;
			structure_ready = 0;
		}
	}
	
	//davide: get average minimum values for each finger
	for(i=0; i<5; i++){
		int val = (int) (calib_matrix[i] / NUM_CALIB_SAMPLES);
		min[i] = val;
		//davide: after saving the value, reset calib_matrix
		calib_matrix[i] = 0;
	}
	printf("DONE!\n");
	sleep(1);
	
	//davide: restart and get maximum values
	counter = 0;
	
	//davide: reset to empty string, just to be sure
	strcpy(current_num, "");
	
	printf("Please keep your hand closed and firm\n");
	sleep(2);
	while(counter < NUM_CALIB_SAMPLES){
		//davide: read from serial and fill calib_matrix with maximum values
		read_(fd);
		if(structure_ready){
			for(i=0; i<5; i++){
				calib_matrix[i] += hand[i];
			}
			counter++;
			structure_ready = 0;
		}
	}
	
	//davide: get average maximum values for each finger
	for(i=0; i<5; i++){
		int val = (int) (calib_matrix[i] / NUM_CALIB_SAMPLES);
		max[i] = val;
	}
	printf("DONE!\n");
	
	//davide: set global variable and trigger
	for(i=0; i<5; i++){
		//davide: we think that for our use-case, the middle value between
		//        min and max should be a nice threeshold
		int val = (min[i] + max[i]) / 2;
		calib_threesholds[i] = val;
		printf("%d > %d\n", i, calib_threesholds[i]);
	}
	//calib_threesholds[3] += 30; // uncomment to make ring finger less sensitive (due to pinkie bending)
	calib_request = 1;
	
	printf("\n----------CALIBRATION SUCCESFULLY COMPLETED!----------\n");
	
	//davide: free allocated memory and return to shell
	free(calib_matrix);
	
	return NULL;
}

/*
 * Tiziano
 * function that resets the state of all controller's keys
 */
void clearCnt(){
	for(int i = 0; i<cnt->size; i++){
		printf("resetto i=%d\n", i);
		resetElemento(cnt, i);
		printf("ho resettato i=%d\n", i);
	}
	printf("vorrei settare\n");
	setState(cnt);
	printf("non so se ho settato\n");
}

/*
 * Tiziano:
 * function that calls the thread for the controller
 */
void start(char **parsed){
	int ret;
    pthread_t thread_id; 
    debugPrintMsg("Starting thread"); 
    
    if (parsed[1] == NULL){
    	ret = pthread_create(&thread_id, NULL, playCnt, NULL);
    	if(ret) {
    		perror("Create error in start-thread: ");
    		exit(-1);
    	}
    	cnt->t_id = (void*) thread_id;
    }
    //Michele: if "-t" is added, debug thread is called
    else if(strcmp(parsed[1],"-t")==0){
    	ret = pthread_create(&thread_id, NULL, debug_thread, NULL);
    	if(ret) {
    		perror("Create error in start-thread: ");
    		exit(-1);
    	}
    	cnt->t_id = (void*) thread_id;
    }
    printf("Created thread %lu\n", thread_id);
    ret = pthread_join(thread_id, NULL);
    if(ret) {
    	perror("Join error in start-thread: ");
    	exit(-1);
    }
    
    printf("Closed thread %lu\n", thread_id);
    //davide: closing fd and clearing things
    cnt->t_id = NULL;
    
    if(cnt->fd != -1){
    	printf("chiudo fd %d\n", cnt->fd);
		ret = close(cnt->fd);
		if(ret) {
    		perror("Close fd error in start-thread: ");
    		exit(-1);
    	}
		cnt->fd = -1;
	}
	printf("pulisco cnt\n");
	//clearCnt();
	printf("ho pulito cnt\nflusho tutto\n");
	fflush(stdout);
	printf("ho flushato\nmo stampo:\n");
	printf("ora dovrei tornare diocane\ntermReq = %d\n", termReq);
}

/*
 * Tiziano
 * function to control the controller (lol)
 */
void controller(char **parsed){
	if(parsed[1]==NULL){
		printControllerSetting(cnt);
		return;
	}
	if(strcmp(parsed[1],"-m")==0){
		if(parsed[2]==NULL){
			printf("\t[USAGE] controller -m {thumb|index|...} {new character}\n\n");
			return;
		}
		enum tipoElemento sw1 = -1;
		if(strcmp(parsed[2],"pinkie")==0)sw1=mignolo;
		if(strcmp(parsed[2],"ring")==0)sw1=anulare;
		if(strcmp(parsed[2],"middle")==0)sw1=medio;
		if(strcmp(parsed[2],"index")==0)sw1=indice;
		if(strcmp(parsed[2],"thumb")==0)sw1=pollice;
		 
		if(parsed[3] == NULL) printf("\t[USAGE] controller -m {thumb|index|...} {new character}\n\n");
		else {
			switch (sw1){
				case 0:
					editElemCharAss(cnt, pollice, parsed[3][0]);
					break;
				case 1:
					editElemCharAss(cnt, indice, parsed[3][0]);
					break;
				case 2:
					editElemCharAss(cnt, medio, parsed[3][0]);
					break;
				case 3:
					editElemCharAss(cnt, anulare, parsed[3][0]);
					break;
				case 4:
					editElemCharAss(cnt, mignolo, parsed[3][0]);
					break;
				default:
					printf("Fingers available to be set are:\n"
						   "\tthumb|index|middle|ring|pinkie\n");
					break;
			}
		}
	}
	if(strcmp(parsed[1],"-s")==0){
		if(parsed[2] != NULL){
			int val = atoi(parsed[2]);
			if(val!=0 && val >= MIN_SOGL_VAL && val <= MAX_SOGL_VAL ){
				printf(
					"CONTROLLER EDIT SOGLIA CONTROLLER EDIT SOGLIA CONTROLLER EDIT SOGLIA CON\n"
					"TROLLER EDIT SOGLIA CONTROLLER EDIT SOGLIA CONTROLLER EDIT SOGLIA CONTRO\n"
					"\n"
				);
				setSoglia(cnt, val);
				calib_request = 0;

				printf("\tThreeshold set to %d\n\n",val);
			
			}else{
				printf("\tError: value must be in [%d , %d]\n\n\n", MIN_SOGL_VAL, MAX_SOGL_VAL);
			}
		}
		else{
			printf("\t[USAGE] controller -s {integer in [%d , %d] }\n\n", MIN_SOGL_VAL, MAX_SOGL_VAL);
		}
	}
}

/*
 * Tiziano
 * function to get the commands
 */

int getCmd(char* cmd){
	size_t characters;
	size_t bufsize = MAX_SIZE;
	
	debugPrintMsg("Dentro getCmd");
    printf("#> ");
	// function getline takes commands from stdin, buffer dimension and
	// address of buffer where to write
	characters = getline(&cmd,&bufsize,stdin);
	//printf("characters = %ld\n", characters);
	printf("FUN getCmd value >%s", cmd);
	
	if(characters == -1){
		perror("");
		return 0;
	}
	else if(characters > 0){
		//strncpy(cmd, buf, characters);
		debugPrint("FUN getCmd value >", cmd);
		return 1;
	}
	else {
		return 0;
	}
}

/*
 * Tiziano
 * function to split input string
 */
 
void splitString(char *str, char **split){
	int i;
	int len = strlen(str);
	str[len-1]='\0'; // erase character '\n' taken by getline
	debugPrint("command taken >",str);
	for(i = 0; i < MAX_CMD; i++){
		split[i] = strsep(&str," ");
		if(split[i]==NULL) break;
		if(strlen(split[i])==0) i--;
	}
	
}

/*
 * Tiziano
 * function to close the program
 */
void quitShell(){
	//Tiziano: "unstroke" every key of controller, just to be sure
	clearCnt();
	for(int i = 0; i<cnt->size;i++){
		//printf("libero il comando %d\n", i);
		free(cnt->elementi[i].sAss);
		//printf("liberato\n");
	}
	free(cnt->elementi);
	cntXdoFree(cnt);
	termReq = 1;
}

/*
 * Tiziano
 * function to handle shell's commands
 */
 
void cmdHandler(char** parsed){
	debugPrintMsg("dentroCmdHandler");
	if(parsed[0]==NULL)parsed[0]="";
	int nCmdSupportati=10, i, switchArg=100;
	char* ListCmd[nCmdSupportati];

   	ListCmd[0]="help";
   	ListCmd[1]="h";
   	ListCmd[2]="quit";
   	ListCmd[3]="q";
   	ListCmd[4]="exit";
   	ListCmd[5]="hi";
   	ListCmd[6]="controller";
   	ListCmd[7]="start";
   	ListCmd[8]="test";
   	ListCmd[9]="calibrate";
   	
   	

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
			break;
		case 2:
		case 3:
		case 4:
			quitShell();
			break;
		case 5:
			sayHi(parsed);
			break;
		case 6:
			controller(parsed);
			break;
		case 7:
			start(parsed);
			break;
		case 8:
			test(parsed);
			break;
		case 9:
			calibration_opt();
			break;
		default:
			printf(
				"Command not recognized, type h or help to display the guide\n\n"
				"##########################################################################\n"
			);
			break;
	}
}

/*
 * Tiziano
 * function to parse input string
 */
 
void parseString(char *str, char** parsed){
	splitString(str, parsed);
	cmdHandler(parsed); 
}


void interrupt_handler(int sig, siginfo_t *siginfo, void *context){
	debugPrintMsg("Dentro interrupt handler");
	int ret;
	if(cnt->t_id){
		pthread_t id = (pthread_t)cnt->t_id;
		printf("Closing thread %lu\n", id);
		ret = pthread_cancel(id);
		if(ret){
			perror("Cancel failed: ");
			exit(-1);
		}
		cnt->t_id = NULL;
	}
	else printf("Controller thread already closed\n");
}

/*
 * Tiziano: shell, struct and int_handler initialization
 */
int main(int argc, char **argv){

	struct sigaction act;
	memset (&act, '\0', sizeof(act));
	/* Use the sa_sigaction field because the handler has two additional parameters */
	act.sa_sigaction = &interrupt_handler;
	/* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
	act.sa_flags = SA_RESTART;
	
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGKILL, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
	//signal(SIGINT | SIGKILL | SIGTERM, &interrupt_ha);
    
	char inStr[MAX_SIZE], *parsedArg[MAX_CMD];
	
	init_shell();
	
	//alloc and initialize controller variable
	cnt = (Controller*) malloc(sizeof(Controller));
	Controller_init(cnt);
	
	while(!termReq){
		if(!getCmd(inStr)) continue;  //getCmd returns 0 when an error has occurred or interrupt has been triggered
		parseString(inStr, parsedArg);
	}
	free(cnt);
	printf("\n\tGOODBYE, SEE YOU SOON !");
	printf("\n\n");
	exit(0);
}


