#include <light_position.h>

void lightPositionUpdate(vec3 & lightPosDir, vec3 & lightPos, bool & lightPosToggle){
	// Updating Light Position in X
	if (lightPosToggle == true) {
		// Moving up/down Y Axis
		if (lightPos.v[1] >= 10.0f) {
			lightPosDir.v[1] = 1;
		}
		else if (lightPos.v[1] <= -10.0f) {
			lightPosDir.v[1] = 0;
		}
		if (lightPosDir.v[1] == 0) {
			lightPos.v[1] = lightPos.v[1] + 0.005;
		}
		else if (lightPosDir.v[1] == 1) {
			lightPos.v[1] = lightPos.v[1] - 0.005;
		}
		// Moving up/down Z axis
		if (lightPos.v[2] >= 10.0f) {
			lightPosDir.v[2] = 1;
		}
		else if (lightPos.v[2] <= -10.0f) {
			lightPosDir.v[2] = 0;
		}
		if (lightPosDir.v[2] == 0) {
			lightPos.v[2] = lightPos.v[2] + 0.005;
		}
		else if (lightPosDir.v[2] == 1) {
			lightPos.v[2] = lightPos.v[2] - 0.005;
		}
	}

}