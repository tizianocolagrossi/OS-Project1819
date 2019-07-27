#pragma once

#include <string.h>
#include "cont_sett_struct.h"

int str_len(char* buffer);
int serial_string(char* buffer);
void set_finger(Controller* cnt, int soglia, int* hand);
void mannaggia_a_dio_porco(Controller* cnt, int soglia, int hand[]);
