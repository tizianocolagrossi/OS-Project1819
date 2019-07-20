//#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "serial.h"
//***************************
//* DEFINE PER BAUD CONTROL *
//***************************

#define MAX_SIZE 10
#define STRING_SIZE 8
#define BAUD 19600
#define MYUBRR (F_CPU/16/BAUD-1)

//**********************
//* PROTOTIPI FUNZIONI *
//**********************

void serial_init(void);
uint8_t serial_read_poll(void);
uint8_t* serial_read(uint8_t* buff);
uint8_t value_control(uint8_t min_val, uint8_t max_val);
void set_finger(void);

uint8_t value_control(uint8_t min_val, uint8_t max_val, uint8_t current_val){
	
	if (curren_val >= min_val && current_val <= max_val) return 1
	else return 0;
	
}

void set_finger(void){

	//quando chiamata detta il valore del dito nella struttura "CONTROL"

}

void serial_init(void){

	UBRR0H = (uint8_t)(MYUBRR>>8);
	UBRR0L = (uint8_t)MYUBRR;

 	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // 8-bit data 
  	UCSR0B = (1<<RXEN0); // abilito RX

}

uint8_t serial_read_poll(void){

	// controllo lo status bit
	while ( !(UCSR0A & (1<<RXC0)) );
  
  	// ritorno il valore contenuto nel registro
  	return UDR0;
}

/*uint8_t* serial_read(uint8_t* buf){
	
	uint8_t i = 0;
	uint8_t* hand[5];
	uint8_t* b0=buf; //beginning of buffer
  	while(1){
    	uint8_t c=UART_getChar();
    	*buf=c;
    	++buf;
    	// reading a 0 terminates the string
    	if (c==0)
    		return *hand;
    	//se leggo una virgola avanzo nella stringa
    	if(c==","){
			++buf;
    	}
    	else {
			hand[i] = c;
    	}
  	}
}
*/
 
int main(void){

	serial_init();
	uint8_t control;
	uint8_t* buff[MAX_SIZE];
	
	printf("*******************************");
	printf("* ASCOLTO SULLA PORTA SERIALE *");
	printf("*******************************");
	
	while(1){
		control = serial_read(*buff);
		if (control > 8 && control <= 0)
		printf("errore sulla serial_read");	
	}
}









