#ifndef _CAMERA_POSITION_H
#define _CAMERA_POSITION_H

#include <maths_funcs.h> //Anton's math class
#include <math.h>

class CameraCoordinates {
public:
	CameraCoordinates(float x, float y, float z, float x_d, float y_d, float z_d);
	float origin_x;
	float origin_y;
	float origin_z;
	float hypot_origin_z_y;
	float destination_x;
	float destination_y;
	float destination_z;
	float hypot_front_z_y;

};

#endif //_CAMERA_POSITION_H