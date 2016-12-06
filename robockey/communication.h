#ifndef communication__
#define communication__

#include "m_rf.h"

#define STANDBY 0
#define PLAY 1

void init_led(void);

char read_instruction(char * buffer, int * state);

#endif