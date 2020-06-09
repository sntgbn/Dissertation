#ifndef KEYBOARD_MOUSE_H_
#define KEYBOARD_MOUSE_H_

// OpenGL Libraries
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <AntTweakBar.h>

#include <camera_position.h>
#include <maths_funcs.h> //Anton's math class

// C++ standard libraries
#include <fstream>
#include <iostream>

extern vec3 lightPosition;
extern bool lightPositionToggle;

// Camera Position
extern vec3 camera_front;
extern vec3 camera_up;
extern vec3 bunny_position;

// keyboard/mouse input function declarations
void key_press(unsigned char key, int mouse_x, int mouse_y);

void mouse_move(int x, int y);

void mouse_wheel(int key, int wheeldir, int x, int y);

void special_keypress(int key, int x, int y);

void mouse_click(int button, int state, int mouse_x, int mouse_y);


#endif //KEYBOARD_MOUSE_H_