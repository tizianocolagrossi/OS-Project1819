#include <stdio.h>   /* Standard input/output definitions */
#include <stdlib.h>  /* For exit and allocation */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <math.h>    /* For Math's functions */

#define MAX_SIZE 25
#define BAUDRATE B19200


struct termios current;

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
		if(c == 45) printf("\n");
		//davide: if c is digit or "," acceptable = true
		int acceptable = (c >= 48 && c <= 57) || c == 44;
		if(acceptable) printf("%c", c);
	}
}

/* porcoddio se levi sto commento e non compila te faccio impazzi coi merge mannaggiacristo

// michele: function that split string with fingers' values
int serial_string(int* buffer){
	
	int i = 0, c = 0;
	int hand[5];
	int b[MAX_SIZE];
	
	while(buffer[i]){
		
		char c = buffer[i];
		
		if (c != ","){
			b = strncat(b, buffer[i]);
		}
		else {
			hand[c] = (int)b;
			c++;
			b = 0;
		}
		i++;
	}
}

// michele: function that set right value of every finger in structure
void set_finger(void){

	//quando chiamata setta il valore del dito nella struttura "CONTROL"

}

// michele: function that control if analog values sent by arduino
// are in a range that was set by user 
int value_control(int min_val, int max_val, int current_val){
	
	if (current_val >= min_val && current_val <= max_val) return 1;
	else return 0;
	
}
*questa è la fine del cristo di commento, vedi di controllare bene prima di togliere
*dio scalzo nella valle dei chiodi arrugginiti 
*/

int main(void){
	
	int fd = port_configure();
	if(fd<0) exit(-1);
	
	while(1){
		//reading from serial forever
		read_(fd);
		//printf("\n");
	}
	
	return 0;
}

/*davide: function to handle the transformation from
  arduino signal to an acceptable value for the controller
int signal_to_controller(int min_soglia, int max_soglia, int signal){
	float ret_val;
	
	if(signal < min_soglia) ret_val = min_soglia; 
	else if(signal > max_soglia) ret_val = max_soglia;
	
}*/




