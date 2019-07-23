#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <math.h>    /* For Math's functions */

#define MAX_SIZE 10
#define BAUDRATE B19200


struct termios current;

// michele: function to set the right bit in termios structure
// to enable the comunication throught the serial port
int port_configure(void){

	int fd = open("/dev/ttyS4", O_RDWR | O_NOCTTY);
	
	if(fd == -1) perror("cannot open dev/ttyACM0 port");
	
	else{
		fcntl(fd, F_SETFL, 0); // ho aggiunto solo questo ma mannaggia alla madonna seriale non ha senso 
		tcgetattr(fd, &current);//salvo valori correnti porta seriale 
		cfsetispeed(&current, B19200);//setto baud rate
		current.c_iflag &= ~IXOFF; //disabilito input flow control
		current.c_cflag &= ~PARENB; //nessun bit di parità
 		current.c_cflag &= ~CSTOPB; //1 bit di stop
        current.c_cflag &= ~CSIZE;//
		current.c_cflag |= CS8; //8-bit data
		current.c_cflag |= ( CLOCAL | CREAD ); //abilito ricevitore porta seriale
		current.c_cc[VMIN] = 0; //legge 25 caratteri
		current.c_cc[VTIME] = 0; //wait indefinitely
		tcsetattr(fd, TCSANOW, &current);
		return fd;
	}
}

void read_(int fd){
	
	char buff[30];
	int byte_read = 0;
	
	printf("entro nel while(1)\n");
	while (1){
		byte_read = read(fd, buff, 25);
		if (byte_read < 0) perror("error during read process");
		else if(byte_read == 0) continue;
		else{
			printf("leggo: %s\n", buff);
			byte_read = 0;
		}
	}
}

// michele: function that split string with fingers' values
int serial_string(int* buffer){
	
	int i = 0, c = 0;
	int hand[5];
	int b[MAX_SIZE];
	
	while(buffer[i]){
		
		char c = buffer[i];
		
		if (c != ","){
			b = strcat(b, buffer[i]);
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
	
	if (curren_val >= min_val && current_val <= max_val) return 1
	else return 0;
	
}


int main(void){
	
	int fd = port_configure();
	
	read_(fd);
	
	return 0;
}

/*davide: function to handle the transformation from
  arduino signal to an acceptable value for the controller
int signal_to_controller(int min_soglia, int max_soglia, int signal){
	float ret_val;
	
	if(signal < min_soglia) ret_val = min_soglia; 
	else if(signal > max_soglia) ret_val = max_soglia;
	
}*/




