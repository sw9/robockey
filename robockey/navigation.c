#include "navigation.h"
#include "m_general.h"
#include <math.h>
#include "localization.h"
#include "puck_detection.h"
#include "m_usb.h"

/*
 M2 is left motor
 M1 is right motor
 B5 = m2 pwm, controlled by OC1A
 B2 = m2 in1, controlled by OC1B
 C6 = m2 in2
 C7 = en
 B7 = m1 pwm
 B0 = m1 in1
 D3 = m1 in2
*/

void init_motors(void) {
	set(DDRB, 5);
    set(DDRB, 2);
	set(DDRC, 6);
	set(DDRC, 7);
    set(DDRB, 7);
	set(DDRB, 0);
	set(DDRD, 3);
	
	// setup timer 1
	// set clock pre-scaler to 1
	set(TCCR1B, CS12);
	clear(TCCR1B, CS11);
	clear(TCCR1B, CS10);

	// mode 7
	clear(TCCR1B, WGM13);
	set(TCCR1B, WGM12);
	set(TCCR1A, WGM11);
	set(TCCR1A, WGM10);

	// clear at OCR1A, set at rollover
	set(TCCR1A, COM1A1);
	clear(TCCR1A, COM1A0);
	
	// clear at OCR1C, set at rollover
	set(TCCR1A, COM1C1);
	clear(TCCR1A, COM1C0);
}

void full_forward(void) {
    clear(PORTB, 2); // motor 2 in forward direction
    set(PORTC, 6);
    set(PORTC, 7); // enable motors
    set(PORTB, 0); // motor 1 in forward direction
    clear(PORTD, 3);
    OCR1A = UP_TO;
	OCR1C = UP_TO;
}

void full_backward(void) {
	set(PORTB, 2); // motor 2 in forward direction
	clear(PORTC, 6);
	set(PORTC, 7); // enable motors
	clear(PORTB, 0); // motor 1 in forward direction
	set(PORTD, 3);
	OCR1A = UP_TO;
	OCR1C = UP_TO;
}


void navigation_angle(double deg) {
	full_forward();
	double frac;
	if (deg > 0) {
		/* turn towards the left */
		if (deg < 90) {
			frac = (90-fabs(deg))/(90.0);
			OCR1A = round(UP_TO*frac);
		} else {
			frac = (fabs(deg) - 90)/(90.0);
			OCR1A = round(UP_TO*frac);
			set(PORTB, 2);
			clear(PORTC, 6);
		}
	} else {
		/* turn towards the right */
		if (deg > -90) {
			frac = (90-fabs(deg))/(90.0);
			OCR1C = round(UP_TO*frac);
		} else {
			frac = (fabs(deg) - 90)/(90.0);
			clear(PORTB, 0);
		    set(PORTD, 3);
			OCR1C = round(UP_TO*frac);
		}
	}
}

void stop_motors(void) {
	clear(PORTC, 7);
}

void navigation_puck(void) {
	double angle = puck_angle();
	navigation_angle(angle);
}

void navigation_point(double* arr, double to_x, double to_y) {
	double from_x = arr[0];
	double from_y = arr[1];
	double deg = arr[2];
	double x = to_x - from_x;
	double y = to_y - from_y;
	
	double rad = acos(y/(sqrt(pow(x, 2) + pow(y, 2))));
	
	/* assume y axis points to front of device */
	rad = x < 0 ? -rad : rad;
	
	navigation_angle(rad*DEG_PER_RAD - deg);
}
