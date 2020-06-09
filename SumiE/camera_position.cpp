#include "camera_position.h"

CameraCoordinates::CameraCoordinates(float x, float y, float z, float x_d, float y_d, float z_d) {
	origin_x = x;
	origin_y = y;
	origin_z = z;
	hypot_front_z_y = hypot(origin_z, origin_y);
	destination_x = x_d;
	destination_y = y_d;
	destination_z = z_d;
	hypot_front_z_y = hypot(destination_z, destination_y);
}