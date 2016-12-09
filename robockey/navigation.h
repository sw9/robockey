#ifndef navigation__
#define navigation__

#define DEG_PER_RAD 57.2958
#define UP_TO 0x3FF

void init_motors(void);

void full_forward(void);

void navigation_angle(double deg);

void navigation_point(double* arr, double to_x, double to_y);

void navigation_puck(void);

void stop_motors(void);

#endif
