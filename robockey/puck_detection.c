#include "m_general.h"
#include <stdbool.h>

int adc_vals[7];
double angles[7] = {0.0, -45.0, -90.0, -135.0, 135.0, 90.0, 45.0};
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
	int max_ind = 0;
	int max_val = -1;
	
	for (int i = 0; i < num_detectors; i++) {
		if (adc_vals[i] > max_val) {
			max_val = adc_vals[i];
			max_ind = i;
		}
	}
	
	double ret = angles[max_ind];
	int before;
	int after;
	double total;
	if 	(max_ind == 0) {
		before = num_detectors - 1;
		after = 1;
	} else {
		before = (max_ind - 1) % num_detectors;
		after = (max_ind + 1) % num_detectors;
		before = before == 0 ? num_detectors - 1 : before;
		after = after == 0 ? 1 : after;
	}
	total = adc_vals[before] + adc_vals[after];
	ret += (angles[before] - ret)*adc_vals[before]/total +
	(angles[after] - ret)*adc_vals[after]/total;
	return ret;
}