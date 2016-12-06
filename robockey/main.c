#include "m_general.h"
#include "m_wii.h"
#include "m_rf.h"
#include "communication.h"
#include "navigation.h"
#include "localization.h"
#include "m_usb.h"
#include "puck_detection.h"
#include <stdlib.h>

volatile bool flag = false;
int state;
unsigned int blobs[12];
char buffer[10];
double result[3];
bool side_change = false;
const double red_x = 120.0;
const double blue_x = -red_x;
const address = 112;

void init(void) {
	state = STANDBY;
	// set clock to 16 MHz
	m_clockdivide(0);
	// enable interrupts
	sei();
		
	m_usb_init();
	m_wii_open();
	m_rf_open(1, address + 0, 10);
	init_led();
	init_motors();
	init_puck_detection();
}

void play_game(void) {
	if (not_enough_stars(blobs)) {
		full_forward();
	} else {
		navigation_point(result, red_x, 0.0);
	}
}

int main(void)
{
	init();
	while(1) {

		if (flag) {
			read_instruction(buffer, &state);
			flag = 0;
		}
		
		if(state == STANDBY) {
			stop_motors();
		}
				
		get_location(blobs, result);
		if (state == PLAY) {
			play_game();
		}
		
		m_wait(1000);
	}
}

ISR(INT2_vect) {
	flag = true;
}
