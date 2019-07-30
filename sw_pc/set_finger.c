#include "set_finger.h"
#include "cont_sett_struct.h"

#include <xdo.h>
#include <stdio.h>
#include <stdlib.h>

#define DIM_VETT_MANO 4

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
void set_finger(Controller* cnt, int soglia, int* hand){
	
	int i = DIM_VETT_MANO;
	//hand = hand + i; //vado alla fine del vettore
	while (i >= 0){
		if (*hand >= soglia) {
			setElemento(cnt, i);
			printf("[set_finger]dito %d settato\n", i);
		}
		else {
			resetElemento(cnt, i);
		}
		hand++;
		i--;
	}
	setState(cnt);
}

// michele: [ALTERNATIVE]function that control if analog values sent by arduino
// are in a range that was set by user 
void set_finger_calib(Controller* cnt, int* vett_soglie, int* hand){
	
	int i = DIM_VETT_MANO, c = 0;
	//hand = hand + i; //vado alla fine del vettore
	while (i >= 0 && c <= 4){
		if (*hand >= *vett_soglie) {
			setElemento(cnt, i);
			printf("[set_finger]dito %d settato\n", i);
		}
		else {
			resetElemento(cnt, i);
		}
		hand++;        // incremento i valori
		i--;		   // dei puntatori
		vett_soglie++; // e dei contatori
		c++;		   //	
	}
	setState(cnt);
}

