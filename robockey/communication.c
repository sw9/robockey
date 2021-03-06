#include "communication.h"
#include "m_rf.h"
#include "m_general.h"
#include <stdbool.h>
#include "m_usb.h"

/* D5 turns LED on or off
B1 reads state of red-blue switch
*/

bool on_red_side(void) {
	if (check(PINB, 1)) { // on red side
		return false;
	} else {	
		return true;
	}
}

void init_led(void) {
	set(DDRD, 5);
	set(PORTD, 5);
	clear(DDRB, 1);
	clear(DDRB, 3);
}

char read_instruction(char * buffer, int * state) {
	char ret = m_rf_read(buffer, 10);
	
	switch (buffer[0]) {
		case 0xA0:
			clear(PORTD, 5);
			m_wait(500);
			set(PORTD, 5);
			m_wait(1000);
			clear(PORTD, 5);
			m_wait(500);
			set(PORTD, 5);
			break;
		case 0xA1:
			*state = PLAY;
			set(PORTD, 5);
			break;
		case 0xA4:
			m_wait(2500);
			*state = STANDBY;
			break;
		default:
			*state = STANDBY;
			break;
	}
	
	return ret;
}