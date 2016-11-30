#ifndef navigation__
#define navigation__

#define DEG_PER_RAD 57.2958
#define UP_TO 0x03FF

void full_forward(void);

void navigation_angle(float deg);

void navigation_point(float from_x, float from_y, float deg, float to_x, float to_y);

void stop_motors(void);

#endif
