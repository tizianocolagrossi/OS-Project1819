//michele
#include <stdio.h>   /* Standard input/output definitions */
#include <stdlib.h>  /* For exit and allocation */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <math.h>    /* For Math's functions */

//michele
#define MAX_SIZE 25
#define BAUDRATE B19200

//michele
struct termios current;

//davide: global variables to handle data read from serial
char* current_num = "";
int current_finger = 0; //0,1,2,3,4
int hand[5];
int structure_ready = 0; //it will be set to 1 when hand is filled


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


//davide: function to read from the serial port
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
			printf("\n");
			int value = atoi(current_num);
			hand[current_finger] = value;
			current_num = "";
			current_finger = 0; //return to thumb finger
			structure_ready = 1;
		}
		//davide: if c is digit add it to the string
		else if(c >= 48 && c <= 57){
			printf("%c", c);
			strncat(current_num, &c, 1);
		}
		//davide: if c is a comma add the number to the array, reset the string, increment finger number
		else if(c == 44){
			printf("%c", c);
			int value = atoi(current_num);
			hand[current_finger] = value;
			current_num = "";
			current_finger++;
			if(current_finger > 4) current_finger = 0; //just to be sure
			
		}
	}
}

//michele: dai, si capisce su
int str_len(char* buffer){
	int i = 0;
	
	while(buffer){
		i++;
		buffer++;
	}
	return i;
}

// michele: function that split string with fingers' values
int serial_string(char* buffer){
	
	int i = 0;
	int hand[5];
	char* b;
	char len = str_len(buffer);
	
	// while on buffer string
	while(buffer){
		
		// if a character in buffer is != from ","
		if (*buffer != 44){
			b = strncat(b, buffer, len); // b is strncat of character in buffer and same b (es b=12 and buffer=3 -> b=123)
		}
		// if character in buffer is == "," i cast in integer the value of b (es b=123 -> hand[i] = 123;)
		else {
			hand[i] = (int)*b;
			b = 0; // i set b = 0 to save next value in buffer
			i++;
		}
		buffer++; // next char in buffer;
	}
}

// michele: function that control if analog values sent by arduino
// are in a range that was set by user 
int value_control(int soglia, int current_val){
	
	if (current_val >= soglia) return 1;
	else return 0;
	
}

int main(void){
	
	int fd = port_configure();
	if(fd<0) exit(-1);
	
	while(1){
		//reading from serial forever
		read_(fd);
		if(structure_ready){
			//TODO inviare hand dove serve
			printf("structure ready\n");
			for(int i=0; i<5; i++){
				printf("%d\n", hand[i]);
			}
			structure_ready = 0;
		}
	}
	
	return 0;
}




