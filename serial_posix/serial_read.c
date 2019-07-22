#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#define MAX_SIZE 10
#define BAUDRATE B19200
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

struct termios current;

int port_configure(void){

	int fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
	
	if(fd == -1) perrror("cannot open dev/ttyUSB0 port");
	
	else{
		tcgetattr(fd, &current);//salvo valori correnti porta seriale 
		cfsetispeed(&options, B19200);//setto baud rate
		current.c_flag &= ~PARENB; //nessun bit di parità
        current.c_flag &= ~CSIZE;//
		current.c_flag |= CS8; //8-bit data
		current.c_cflag |= CLOCAL | CREAD; //abilito ricevitore porta seriale
		current.c_cc[VMIN] = 25; //legge 25 caratteri
		current.c_cc[VTIME] = 0; //wait indefinitely
		tcsetattr(fd, TCSANOW, &current);
		return fd;
	}
}

void read_(int fd){
	
	char buff[30];
	int byte_read = 0;
	
	while (1){
		byte_read = read(fd, &buff, 30);
		
		if (byte_read < 0) perror("error during read process");
		else{
			printf("String ricevuta: %s", buff);
		}
	}
}

int main(void){
	
	int fd = port_configure();
	
	printf("chiamo la read");
	read_(fd);
	printf("read chiamata, se non ha scritto nulla c'è un problema per dio");
	
	return 0;
}
