#include "keyboard_mouse.h"
float camera_speed_z = 0.0025;
float camera_speed_x = 0.25;

int  camera_front_switch = 0;

void key_press(unsigned char key, int mouse_x, int mouse_y) {
	//std::cout << "Keypress: " << keypress << std::endl;
	if (!TwEventKeyboardGLUT(key, mouse_x, mouse_y))
	{
		switch (key) {
			//// Camera controls
			//case('w'):
			//	camera_pos += camera_front*camera_speed_z;
			//	break;
			//case('s'):
			//	camera_pos -= camera_front*camera_speed_z;
			//	break;
			//case('a'):
			//	camera_pos -= normalise(cross(camera_front, camera_up))*camera_speed_x;
			//	break;
			//case('d'):
			//	camera_pos += normalise(cross(camera_front, camera_up))*camera_speed_x;
			//	break;
			// Reset Condition
		case('l'):
			std::cout << "Changing light position" << std::endl;
			lightPositionToggle = !lightPositionToggle;
			break;
		case('R'):
			camera_front = vec3(0.0f, 0.0f, -10.0f);
			camera_up = vec3(0.0f, 1.0f, 0.0f);
			// Sphere positions
			bunny_position = vec3(0.0f, -1.0f, -3.0f);
			break;
		}
	}
};

void mouse_move(int x, int y) {
	//// Fix x_mouse coordinates calculation to only take the first viewport
	//x_mouse = (float)-(x - width / 2) / (width / 2);
	//y_mouse = (float)-(y - height / 2) / (height / 2);
};

void mouse_wheel(int key, int wheeldir, int x, int y) {
	//if (wheeldir ==  1)
	//{
	//	z_mouse -= 0.2f;
	//} else if (wheeldir == -1){
	//	z_mouse += 0.2f;
	//}
}

void special_keypress(int key, int x, int y) {
	if (!TwEventSpecialGLUT(key, x, y))
	{
		switch (key) {
			// Pitch
		//case(GLUT_KEY_UP):
		//	std::cout << "Increasing unlit outline thickness" << std::endl;
		//	unlitOutlineThickness += 0.1;
		//	break;
		//case(GLUT_KEY_DOWN):
		//	std::cout << "Decreasing unlit outline thickness" << std::endl;
		//	unlitOutlineThickness -= 0.1;
		//	break;
		//	// Roll
		//case(GLUT_KEY_LEFT):
		//	std::cout << "Increasing lit outline thickness" << std::endl;
		//	litOutlineThickness += 0.1;
		//	break;
		//case(GLUT_KEY_RIGHT):
		//	std::cout << "Decreasing lit outline thickness" << std::endl;
		//	litOutlineThickness -= 0.1;
		//	break;
		//}
		}
	}
};

void mouse_click(int button, int state, int mouse_x, int mouse_y)
{
	if (!TwEventMouseButtonGLUT(button, state, mouse_x, mouse_y))
	{
		std::cout << "mouse_clickL: something goes here, maybe" << std::endl;
	}
}
