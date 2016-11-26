#include "navigation.h"
#include "m_general.h"
#include <math.h>

/*
 M2 is left motor
 M1 is right motor
 B5 = m2 pwm, controlled by OC1A
 B4 = m2 in1, controlled by OC1B
 C7 = m2 in2
 C6 = en
 B6 = m1 pwm
 E6 = m1 in1
 D4 = m1 in2
*/

void navigation_angle(float deg) {
    set(PORTB, 4); // motor 2 in forward direction
    clear(PORTC, 7);
    set(PORTC, 6); // enable motors
    set(PORTE, 6); // motor 1 in forward direction
	set(PORTD, 4);
    
    float frac = (180-fabs(deg))/(180.0);
    
	if (deg > 0) {
		/* turn towards the left */
        OCR1A = round(0x00FF*frac);
        OCR1B = 0x00FF;
	} else {
		/* turn towards the right */
        OCR1A = 0x00FF;
        OCR1B = round(0x00FF*frac);
	}
}

void stop_motors(void) {
	clear(PORTC, 6);
}

void navigation_point(float from_x, float from_y, float deg, float to_x, float to_y) {
	float x = to_x - from_x;
	float y = to_y - from_y;
	
	float rad = acos(x/(sqrt(pow(x, 2) + pow(y, 2))));
	
	/* assume x axis points to front of device */
	rad = y > 0 ? rad : -rad;
	navigation_angle(rad*DEG_PER_RAD - deg);
}
