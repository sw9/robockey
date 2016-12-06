#ifndef communication__
#define communication__

#include "m_rf.h"
#include <stdbool.h>

#define STANDBY 0
#define PLAY 1

bool on_red_side(void);
void init_led(void);

char read_instruction(char * buffer, int * state);

#endif