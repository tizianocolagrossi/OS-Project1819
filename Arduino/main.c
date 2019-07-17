#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 19600
#define MYUBRR (F_CPU/16/BAUD-1)
#define MAX_BUF 256
#define TIMER_DURATION_MS 100;

volatile uint8_t interrupt_occurred=0;

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

void UART_init(void){
  // Set baud rate
  UBRR0H = (uint8_t)(MYUBRR>>8);
  UBRR0L = (uint8_t)MYUBRR;

  UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); /* 8-bit data */ 
  UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);   /* Enable RX and TX */  
}

void UART_putChar(uint8_t c){
  // wait for transmission completed, looping on status bit
  while ( !(UCSR0A & (1<<UDRE0)) );
  // Start transmission
  UDR0 = c;
}

uint8_t UART_getChar(void){
  // Wait for incoming data, looping on status bit
  while ( !(UCSR0A & (1<<RXC0)) );
  // Return the data
  return UDR0;
}

// reads a string until the first newline or 0, returns the size read
uint8_t UART_getString(uint8_t* buf){
  uint8_t* b0=buf; //beginning of buffer
  while(1){
    uint8_t c=UART_getChar();
    *buf=c;
    ++buf;
    // reading a 0 terminates the string
    if (c==0)
      return buf-b0;
    // reading a \n  or a \r return results
    // in forcedly terminating the string
    if(c=='\n'||c=='\r'){
      *buf=0;
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

int main(void){
  UART_init();
  //UART_putString((uint8_t*)"write PERONI, Virginia will hear you\n");
  uint8_t buf[MAX_BUF];
  while(1) {
    UART_getString(buf);
    UART_putString((uint8_t*)"ao\n");
    UART_putString(buf);
  }
}
