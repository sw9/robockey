#include "m_general.h"
#include "m_wii.h"
#include "m_rf.h"
#include "communication.h"

volatile bool flag = false;

int main(void)
{
	// set clock to 16 MHz
	m_clockdivide(0);
		
	// enable interrupts
	sei();
	
	m_wii_open();
	m_rf_open(1, 112, 10);
	
	int state = STANDBY;
	unsigned int blobs[12];
	char buffer[10];
	float result[3];
	bool side_change = false;
	
	while(1) {
				
		if (flag) {
			read_instruction(buffer, &state, &side_change);
			flag = 0;
		}
		
		if(STANDBY) {
			
		}

		
		if (state == COMMTEST) {
			state = STANDBY;
			
		}
		
		if (state == PLAY) {
				
		}
	}	
}

ISR(INT2_vect) {
	flag = true;
}