#pragma once

#include <string.h>
#include "cont_sett_struct.h"

int str_len(char* buffer);
int serial_string(char* buffer);
void set_finger(Controller* cnt, int soglia, int* hand);
void set_finger_calib(Controller* cnt, int* vett_soglie, int* hand);
