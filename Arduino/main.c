#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define BAUD 19600
#define MYUBRR (F_CPU/16/BAUD-1)
#define MAX_BUF_SIZE 256
#define TIMER_DURATION_MS 1000

volatile uint8_t interrupt_occurred = 0;

ISR(TIMER5_COMPA_vect) {
	interrupt_occurred=1;
}

void TIMER_init(){
	// configure timer, set the prescaler to 1024
	TCCR5A = 0;
	TCCR5B = (1 << WGM52) | (1 << CS50) | (1 << CS52); 
 
	// processor at 16Mhz, prescaler at 1024 -> 16000000/1024 = 15625 counts per second
	uint16_t ocrval=(uint16_t)(15.625 * TIMER_DURATION_MS);
	OCR5A = ocrval;

	// clear int
	cli();
	TIMSK5 |= (1 << OCIE5A);  // enable the timer interrupt
	// enable int
	sei();
}

void UART_init(){
	// Set baud rate
	UBRR0H = (uint8_t)(MYUBRR>>8);
	UBRR0L = (uint8_t)MYUBRR;

	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); /* 8-bit data */ 
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);   /* Enable RX and TX */  
}

// see page 281+ from atmega2560 datasheet =)
void ADC_init(){
	//Prescaler at 128 so we have an 125Khz clock source
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)); 
	//Auto Trigger Enable: Signal source, in this case is the free-running 
	ADCSRA |= (1<<ADATE);
	//ADTS2, ADTS1, ADTS0 set to 0 for free-running mode, MUX5 for input selection
	ADCSRB &= 0;
	//Enable bit set to 1 in order to chose the mux input
	ADCSRB |= (1<<ACME);
	// REFS1 = 0, ADLAR = 0, MUX[4:0] = 0 for ANALOG-0 input
	ADMUX &= 0;
	//Avcc(+5v) as voltage reference
	ADMUX |= (1<<REFS0);
	//Power up the ADC
	ADCSRA |= (1<<ADEN);
	//Start converting
	ADCSRA |= (1<<ADSC);
}

void ADC_init2(){
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz

   ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
   ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading

   // No MUX values needed to be changed to use ADC0

   //ADCSRA |= (1 << ADFR);  // Set ADC to Free-Running Mode

   ADCSRA |= (1 << ADEN);  // Enable ADC
   ADCSRA |= (1 << ADSC);  // Start A2D Conversions
}

void PORT_init(){
	// Analog pin 0 is connected to Port F Pin 0 (ADC0) 
	const uint8_t mask=(1<<0);

	// we configure the pin as input, clearing the bit 0
	DDRF &= ~mask;
  
	// we enable pullup resistor on that pin
	PORTF |= mask;
}

void UART_putChar(uint8_t c){
	// wait for transmission completed, looping on status bit
	while ( !(UCSR0A & (1<<UDRE0)) );
	// Start transmission
	UDR0 = c;
}

uint8_t UART_getChar(){
	// Wait for incoming data, looping on status bit
	while ( !(UCSR0A & (1<<RXC0)) );
	// Return the data
	return UDR0;
}

// reads a string until the first newline or 0, returns the size read
uint8_t UART_getString(uint8_t* buf){
	uint8_t* b0 = buf; //beginning of buffer
	while(1){
		uint8_t c = UART_getChar();
		*buf = c;
		++buf;
		// reading a 0 terminates the string
		if (c==0) return buf-b0;
		// reading a \n  or a \r return results in forcedly terminating the string
		if(c=='\n'||c=='\r'){
			*buf = 0;
			++buf;
			return buf-b0;
		}
	}
}

void UART_putString(uint8_t* buf){
	while(*buf){
		UART_putChar(*buf);
		++buf;
	}
}

int main(){
	UART_init();
	//PORT_init();
	TIMER_init();
	//ADC_init();
	ADC_init2();
	int analog_val;
	//uint8_t buf[MAX_BUF_SIZE];
	
	UART_putString((uint8_t*) "Starting program\n");	
	while(1) {
		while (! interrupt_occurred);  //busy-wait for interrupt to be triggered
		// we reset the flag;
		interrupt_occurred = 0;
		
		UART_putString( (uint8_t*) "Interrupt triggered\n");
		analog_val = ADCH;
		/*if(analog_val < 128){
			UART_putString( (uint8_t*) "IF 1\n");
		}
		if(analog_val >= 128){
			UART_putString( (uint8_t*) "IF 2\n");
		}*/
		//UART_putString( (uint8_t*) "VALUE:\n");
		uint8_t* print_val = (uint8_t*) itoa(analog_val);
		uint8_t* terminator = print_val;
		terminator += 8;
		*terminator = '\0';
		UART_putString(print_val);
		UART_putString( (uint8_t*) "\n");
	}
}

