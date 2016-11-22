#ifndef communication__
#define communication__

#include "m_rf.h"

#define STANDBY 0
#define COMMTEST 1
#define PLAY 2

char read_instruction(char * buffer, int * state, bool * side_change);

#endif