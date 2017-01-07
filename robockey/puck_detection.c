#include "m_general.h"
#include <stdbool.h>
#include "m_usb.h"

int adc_vals[7];
double angles[7] = {0.0, 32.0, 76.0, 145.0, -145.0, -76.0, -32.0};
int num_detectors = 7;

bool has_puck(void) {
	if (check(PIND, 4) || check(PIND, 6)) {
		return true;
	} else {
		return false;
	}
}

void init_puck_detection(void) {
	// D4 and D6 are momentary switches
	// set them for input
	clear(DDRD, 4);
	clear(DDRD, 6);

	// configure ADC
	// reference voltage is 5V
	clear(ADMUX, REFS1);
	set(ADMUX, REFS0);
	
	// divide system clock by 128
	set(ADCSRA, ADPS2);
	set(ADCSRA, ADPS1);
	set(ADCSRA, ADPS0);
	
	// disable digital inputs
	set(DIDR0, ADC0D);
	set(DIDR0, ADC1D);
	set(DIDR0, ADC4D);
	set(DIDR0, ADC5D);
	set(DIDR0, ADC6D);
	set(DIDR0, ADC7D);
	set(DIDR2, ADC10D);
	
	m_disableJTAG();
}

void adc_begin(int * val) {
	// enable ADC
	set(ADCSRA, ADEN);
	set(ADCSRA, ADSC);
	while(check(ADCSRA, ADSC)) {
	}
	set(ADCSRA, ADIF);
	*val = ADC;
}

void get_adc_values(int * arr) {
	// read F0
	clear(ADCSRB, MUX5);
	clear(ADMUX, MUX2);
	clear(ADMUX, MUX1);
	clear(ADMUX, MUX0);
	adc_begin(&arr[0]);
	// read F1
	clear(ADCSRB, MUX5);
	clear(ADMUX, MUX2);
	clear(ADMUX, MUX1);
	set(ADMUX, MUX0);
	adc_begin(&arr[1]);
	// read F4
	clear(ADCSRB, MUX5);
	set(ADMUX, MUX2);
	clear(ADMUX, MUX1);
	clear(ADMUX, MUX0);
	adc_begin(&arr[2]);
	// read F5
	clear(ADCSRB, MUX5);
	set(ADMUX, MUX2);
	clear(ADMUX, MUX1);
	set(ADMUX, MUX0);
	adc_begin(&arr[3]);
	// read F6
	clear(ADCSRB, MUX5);
	set(ADMUX, MUX2);
	set(ADMUX, MUX1);
	clear(ADMUX, MUX0);
	adc_begin(&arr[4]);
	// read F7
	clear(ADCSRB, MUX5);
	set(ADMUX, MUX2);
	set(ADMUX, MUX1);
	set(ADMUX, MUX0);
	adc_begin(&arr[5]);
	// read D7
	set(ADCSRB, MUX5);
	clear(ADMUX, MUX2);
	set(ADMUX, MUX1);
	clear(ADMUX, MUX0);
	adc_begin(&arr[6]);	
}

double puck_angle(void) {
	// ADC values are between 0 and 1023
	
	get_adc_values(adc_vals);
		
	for (int i = 0; i < num_detectors; i++) {
		adc_vals[i] = 1023 - adc_vals[i];	
	}
	
	int max_ind = 0;
	double max_val = 0.0;
	for (int i = 0; i < num_detectors; i++) {
		if (adc_vals[i] > max_val) {
			max_ind = i;
			max_val = adc_vals[i];
		}
		m_usb_tx_int(adc_vals[i]);
		m_usb_tx_string("\t");
	}
	m_usb_tx_string("\n");
	
	return angles[max_ind];
}