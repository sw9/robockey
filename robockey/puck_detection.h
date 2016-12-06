#ifndef puck_detection__
#define puck_detection__
#include <stdbool.h>

void init_puck_detection(void);

void adc_begin(int * val);

void get_adc_values(int * arr);

double puck_angle(void);

bool has_puck(void);

#endif