#ifndef localization__
#define localization__
#include <stdbool.h>

char get_location(unsigned int* arr, double* result);
// FUNCTIONALITY:
// Figure out position and orientation of robot
//
// TAKES:
// arr: pointer to a 12-element unsigned int buffer
// result: pointer to a 3-element double buffer
//
// RETURNS:
// whatever m_wii_read returns

bool not_enough_stars(unsigned int* arr);

#endif