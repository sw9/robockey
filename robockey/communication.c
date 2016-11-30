#include "communication.h"
#include "m_rf.h"
#include "m_general.h"

char read_instruction(char * buffer, int * state, bool * side_change) {
	char ret = m_rf_read(buffer, 10);
	
	switch (buffer[0]) {
		case 0xA0:
			*state = COMMTEST;
			break;
		case 0xA1:
			*state = PLAY;
			break;
		case 0xA2:
			*state = STANDBY;
			break;
		case 0xA3:
			*state = STANDBY;
			break;
		case 0xA4:
			m_wait(2500);
			*state = STANDBY;
			break;
		case 0xA6:
			*side_change = !(*side_change);
			*state = STANDBY;
			break;
		case 0xA7:
			*state = STANDBY;
			break;
	}
	
	return ret;
}