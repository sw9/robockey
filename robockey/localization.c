#include "localization.h"
#include "m_wii.h"
#include "navigation.h"
#include <math.h>

char get_location(unsigned int* arr, float* result) {
	char ret = m_wii_read(arr);
	
	float max_d = -1;
	float f = 1280.0;
	unsigned int offset_x = 512;
	unsigned int offset_y = 384;
	unsigned int ind_i;
	unsigned int ind_j;
	unsigned int point_i_x;
	unsigned int point_i_y;
	unsigned int point_j_x;
	unsigned int point_j_y;
	unsigned int i_x;
	unsigned int i_y;
	unsigned int j_x;
	unsigned int j_y;
	unsigned int k_x;
	unsigned int k_y;

	for (int i=0; i < 3; i++) {
		for (int j=i+1; j < 4; j++) {
			i_x = arr[i*3];
			i_y = arr[i*3 + 1];
			j_x = arr[j*3];
			j_y = arr[j*3 + 1];
			float d = sqrt(pow((i_x - j_x), 2) + pow((i_y - j_y), 2));
			
			if (d > max_d) {
				ind_i = i;
				ind_j = j;
				max_d = d;
				point_i_x = i_x;
				point_i_y = i_y;
				point_j_x =  j_x;
				point_j_y = j_y;
			}
		}
	}
	
	float center_x = (point_i_x + point_j_x)/2.0;
	float center_y = (point_i_x + point_j_x)/2.0;
	result[0] = -(center_x - offset_x)/f*5;
	result[1] = -(center_y - offset_y)/f*5;
	
	float i_sum = 0;
	float j_sum = 0;

	for (int k=0; k < 4; k++) {
		if (k != ind_i && k != ind_j) {
			k_x = arr[k*3];
			k_y = arr[k*3 + 1];
			float d_i = sqrt(pow((k_x - point_i_x), 2) + pow((k_y - point_i_y), 2));
			float d_j = sqrt(pow((k_x - point_j_x), 2) + pow((k_y - point_j_y), 2));
			i_sum += d_i;
			j_sum += d_j;

		}
	}
	
	float y_axis_x = i_sum > j_sum ? point_i_x: point_j_x;
	y_axis_x -= center_x;
	float y_axis_y = i_sum > j_sum ? point_i_y: point_j_y;
	y_axis_y -= center_y;
	float rad = acos(y_axis_y/sqrt(pow(y_axis_x, 2) + pow(y_axis_y, 2)));

	if (y_axis_x > 0) {
		result[3] = -rad*DEG_PER_RAD;
	} else {
		result[3] = rad*DEG_PER_RAD;
	}
	return ret;
}
