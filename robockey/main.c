#define F_CPU 16000000

#include "m_general.h"
#include "m_wii.h"
#include "m_rf.h"
#include "communication.h"
#include "navigation.h"
#include "localization.h"
#include "m_usb.h"
#include "puck_detection.h"
#include <stdbool.h>

volatile bool flag = false;
int state;
unsigned int blobs[12];
char buffer[10];
double result[3];
bool side_change = false;
double red_x = 120.0;
double blue_x;
int address = 112;
int puck_counter = 0;

void init(void) {
	blue_x = -red_x;

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
	if (puck_counter == 0) {
		m_red(OFF);
		navigation_puck();
	} else {
		m_red(ON);
		if (not_enough_stars(blobs)) {
			full_forward();
		} else {
			if (on_red_side()) {
				navigation_point(result, blue_x, 0.0);
			} else {
				navigation_point(result, red_x, 0.0);
			}
		}
		puck_counter--;
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

		if (has_puck()) {
			puck_counter = 12;
		}

		if(state == STANDBY) {
			stop_motors();
		}
		get_location(blobs, result);

		if (state == PLAY) {
			play_game();
			// full_forward();
			// navigation_angle(-170);
		}

		m_wait(250);
	}
}

ISR(INT2_vect) {
	flag = true;
}
