#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#define MAX_SIZE 10
#define BAUDRATE B19200


struct termios current;

int port_configure(void){

	int fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
	
	if(fd == -1) perror("cannot open dev/ttyUSB0 port");
	
	else{
		tcgetattr(fd, &current);//salvo valori correnti porta seriale 
		cfsetispeed(&current, B19200);//setto baud rate
		current.c_iflag &= ~IXOFF; //disabilito input flow control
		current.c_cflag &= ~PARENB; //nessun bit di parità
 		current.c_cflag &= ~CSTOPB; //1 bit di stop
        current.c_cflag &= ~CSIZE;//
		current.c_cflag |= CS8; //8-bit data
		current.c_cflag |= CREAD; //abilito ricevitore porta seriale
		current.c_cc[VMIN] = 0; //legge 25 caratteri
		current.c_cc[VTIME] = 0; //wait indefinitely
		tcsetattr(fd, TCSANOW, &current);
		return fd;
	}
}

void read_(int fd){
	
	char buff[30];
	int byte_read = 0;
	
	printf("entro nel while(1)");
	while (1){
		printf("read stringa\n");
		byte_read = read(fd, buff, 25);
		if (byte_read < 0) perror("error during read process");
		else if(byte_read == 0) continue;
		else{
			printf("%s\n", buff);
			byte_read = 0;
		}
	}
}

int main(void){
	
	int fd = port_configure();
	
	printf("chiamo la read");
	read_(fd);
	
	return 0;
}
