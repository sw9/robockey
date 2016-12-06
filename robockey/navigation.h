#ifndef navigation__
#define navigation__

#define DEG_PER_RAD 57.2958
#define UP_TO 0x03FF

void init_motors(void);

void full_forward(void);

void navigation_angle(float deg);

void navigation_point(unsigned int* arr, float to_x, float to_y);

void navigation_puck(void);

void stop_motors(void);

#endif
