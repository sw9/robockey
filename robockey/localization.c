#include "localization.h"
#include "m_wii.h"
#include "navigation.h"
#include <math.h>
#include "m_general.h"
#include "m_usb.h"
#include <stdbool.h>

int x_left = 860;
int x_right = 258;
int y_top = 450;
int y_down = 215;
int x_mid = 532;
int y_mid = 365;

bool not_enough_stars(unsigned int* arr) {
	bool ret = false;
	for (int i=0; i < 3; i++) {
		int i_x = arr[i*3];
		int i_y = arr[i*3 + 1];
		if (i_x == 1023 && i_y == 1023) {
			ret = true;
		}
	}
	return ret;
}

char get_location(unsigned int* arr, double* result) {
	char ret = m_wii_read(arr);
	
	double x_avg = (x_left + x_right)/2.0;
	double y_avg = (y_top + y_down)/2.0;
	double x_mul = 115.0/(x_left - x_avg);
	double y_mul = 60.0/(y_top - y_avg);

	double max_d = -1;
	int ind_i = 0;
	int ind_j = 0;
	int point_i_x = 0;
	int point_i_y = 0;
	int point_j_x = 0;
	int point_j_y = 0;

	/* for (int i = 0; i < 4; i++) {
		m_usb_tx_int(arr[i*3]);
		m_usb_tx_string("\t");

		m_usb_tx_int(arr[i*3+1]);
		m_usb_tx_string("\t");
	}
	m_usb_tx_string("\n");*/

	for (int i=0; i < 3; i++) {
		for (int j=i+1; j < 4; j++) {

			int i_x = arr[i*3];
			int i_y = arr[i*3 + 1];
			int j_x = arr[j*3];
			int j_y = arr[j*3 + 1];

			double d = pow((i_x - j_x), 2) + pow((i_y - j_y), 2);
			d = sqrt(d);
			
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
	
	if (not_enough_stars(arr)) {
		m_green(ON);
	} else {
		m_green(OFF);
	}
	
	double center_x = (point_i_x + point_j_x)/2.0;
	double center_y = (point_i_y + point_j_y)/2.0;

	double i_sum = 0;
	double j_sum = 0;
	for (int k=0; k < 4; k++) {
		if ((k != ind_i) && (k != ind_j)) {
			int k_x = arr[k*3];
			int k_y = arr[k*3 + 1];
			double d_i = sqrt(pow((k_x - point_i_x), 2) + pow((k_y - point_i_y), 2));
			double d_j = sqrt(pow((k_x - point_j_x), 2) + pow((k_y - point_j_y), 2));
			i_sum += d_i;
			j_sum += d_j;
		}
	}
	
	double y_axis_x = i_sum > j_sum ? point_i_x: point_j_x;
	double y_axis_y = i_sum > j_sum ? point_i_y: point_j_y;
	y_axis_x -= center_x;
	y_axis_y -= center_y;
	double rad = acos(y_axis_y/sqrt(pow(y_axis_x, 2) + pow(y_axis_y, 2)));

	if (y_axis_x > 0) {
		result[2] = -rad*DEG_PER_RAD;
	} else {
		result[2] = rad*DEG_PER_RAD;
	}
	
	/*m_usb_tx_int((int) i_sum);
	m_usb_tx_string(" i_sum \n");
	m_usb_tx_int((int) j_sum);
	m_usb_tx_string(" j_sum \n");*/

	double old_x = center_x - x_mid;
	double old_y = center_y - y_mid;
	
	double rot = -result[2]/DEG_PER_RAD;
	center_x = old_x*cos(rot) - old_y*sin(rot) + x_mid;
	center_y = old_x*sin(rot) + old_y*cos(rot) + y_mid;

	result[0] = -(center_x - x_avg)*x_mul;
	result[1] = -(center_y - y_avg)*y_mul;

	m_usb_tx_int(result[0]);
	m_usb_tx_string("x\n");
	m_usb_tx_int(result[1]);
	m_usb_tx_string("y\n");
	m_usb_tx_int(result[2]);
	m_usb_tx_string("rot\n");
	
	return ret;
}
