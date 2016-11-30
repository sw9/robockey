#include "m_general.h"
#include "m_wii.h"
#include "m_rf.h"
#include "communication.h"
#include "navigation.h"
#include "localization.h"

volatile bool flag = false;

int state = STANDBY;
unsigned int blobs[12];
char buffer[10];
float result[3];
bool side_change = false;

void init(void) {
	// set clock to 16 MHz
	m_clockdivide(0);
		
	// enable interrupts
	sei();
		
	char ret = m_wii_open();
	
	if (ret == 0) {
		m_red(ON);
	}
	
	m_rf_open(1, 112, 10);
	
	// enabled output pins
	set(DDRB, 4);
    set(DDRB, 5);
    set(DDRB, 6);
	set(DDRC, 6);
	set(DDRC, 7);
	set(DDRE, 6);
	set(DDRD, 4);
    
	// setup timer 1
	// set clock pre-scaler to 1
	clear(TCCR1B, CS12);
	clear(TCCR1B, CS11);
	set(TCCR1B, CS10);

	// mode 7
	clear(TCCR1B, WGM13);
	set(TCCR1B, WGM12);
	set(TCCR1A, WGM11);
	set(TCCR1A, WGM10);

	// clear at OCR1A, set at rollover
	set(TCCR1A, COM1A1);
	clear(TCCR1A, COM1A0);
	
	// clear at OCR1B, set at rollover
	set(TCCR1A, COM1B1);
	clear(TCCR1A, COM1B0);	
}

void play_game(void) {
	get_location(blobs, result);
	navigation_point(result[0], result[1], result[2], 0.0, -115.0);
}

int main(void)
{
	init();
	while(1) {
				
		if (flag) {
			read_instruction(buffer, &state, &side_change);
			flag = 0;
		}
		
		if(STANDBY) {
			stop_motors();			
		}
		
		if (state == COMMTEST) {
			state = STANDBY;
			
		}
		
		if (state == PLAY) {
			play_game();
		}
		
		full_forward();
		m_wait(100);
	}
}

ISR(INT2_vect) {
	flag = true;
}