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
			case('w'):
				camera_pos.v[1] += 0.01f;
				break;
			case('s'):
				camera_pos.v[1] -= 0.01f;
				break;
			case('a'):
				camera_pos.v[0] -= 0.01f;
				break;
			case('d'):
				camera_pos.v[0] += 0.01f;
				break;
			// Reset Condition
		case('l'):
			std::cout << "Changing light position" << std::endl;
			lightPositionToggle = !lightPositionToggle;
			break;
		case('r'):
			std::cout << "Toggling rotation" << std::endl;
			//lightPositionToggle = !lightPositionToggle;
			bunny_rotation_toggle = !bunny_rotation_toggle;
			break;
		case('R'):
			camera_pos = vec3(0.0f, 0.0f, 0.0f);
			camera_front = camera_pos;
			camera_front.v[2] -= 10.0f;
			camera_up = vec3(0.0f, 1.0f, 0.0f);
			// Sphere positions
			bunny_position = vec3(0.0f, -1.0f, -3.0f);
			bunny_rotation_toggle = false;
			rotation_deg = 0.0f;
			break;
		}
	}
};

void mouse_move(int x, int y) {
	//// Fix x_mouse coordinates calculation to only take the first viewport
	//x_mouse = (float)-(x - width / 2) / (width / 2);
	//y_mouse = (float)-(y - height / 2) / (height / 2);
};

void mouse_wheel_click(int key, int wheeldir, int x, int y) {
	if (!TwEventMouseButtonGLUT(key, wheeldir, x, y))
	{
		if (wheeldir == GLUT_DOWN) {
			switch (key) {
			case GLUT_LEFT_BUTTON:
				//std::cout << "left click at: (" << x << ", " << y << ")\n";
				break;
			case GLUT_RIGHT_BUTTON:
				//std::cout << "right click at: (" << x << ", " << y << ")\n";
				break;
			case GLUT_MIDDLE_BUTTON:
				//std::cout << "middle click at: (" << x << ", " << y << ")\n";
				break;
				//mouse wheel scrolls
			case 3:
				camera_pos.v[2] += 0.01f;
				break;
			case 4:
				camera_pos.v[2] -= 0.01f;
				break;
			default:
				break;
			}
		}
	}
	glutPostRedisplay();
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