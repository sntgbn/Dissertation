//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>

// OpenGL Libraries
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <AntTweakBar.h>

// C++ standard libraries
#include <fstream>
#include <iostream>
#include <sstream>
#include <string> 

// Custom Libraries
#include <blender_loader.h>
#include <camera_position.h>
#include <keyboard_mouse.h>
#include <light_position.h>
#include <maths_funcs.h> //Anton's math class
#include <projection_matrices.h>
#include <textures_cube_map.h>
#include <shaders_buffers.h>
#include <yaw_pitch_roll.h>
#include "teapot.h" // teapot mesh


// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;
GLuint reflection_program_id;
GLuint cubeMapShaderID;
shaderLocations reflection_locations;
shaderLocations cubemap_shader_locations;

// Variables referenced in header files
bool camera_switch = false;
vec3 camera_pos(0.0f, 0.0f, 0.0f);
vec3 camera_front(0.0f, 0.0f, -10.0f);
vec3 camera_up(0.0f, 1.0f, 0.0f);

// ViewPort Dimensions
const int width = 800.0;
const int height = 600.0;

// Shader Variables
// Vertex Coordenates and Normals
GLuint vertexPositionLocation;
GLuint vertexNormalsLocation;
// Light Position
vec3 lightPositionDirection = vec3(0, 0, 0);
vec3 lightPosition = vec3(0, 10, 0);
bool lightPositionToggle = false;

// View Position
GLuint viewPositionLocation;
// Shader Type Ambient and Specular Lighting Strength
float unlit_outline_thickness = 0.5f; // specular_strength_location
float lit_outline_thickness = 0.5f; // ambient_strength_location
float wobble_distortion = 1.0f;
float paper_roughness = 0.5f;
float diffuse_factor = 1.0f;
float dry_brush_granulation = 0.0f;
float dry_brush_density = 0.5f;

// Model Load Variables & VAO Variables
//BlenderObj sphereMesh("../meshes/cone.obj");
BlenderObj sphereMesh("../meshes/bunny.obj");
GLuint sphereVao;
GLuint cubeMapVao;
GLuint texCube;

// Mesh Projection Matrices
ProjectionMatrices bunny_mesh;
//vec3 bunny_position = vec3(0.0f, -0.9f, -30.0f);
vec3 bunny_position = vec3(0.0f, -0.9f, -2.5f);
float rotation_deg = 0;

// Texture/Normal map ID
GLuint paper_texture_id;
GLuint paper_normal_id;
GLuint paper_height_id;

// Tweak Bar
TwBar* shader_settings;

void display(){
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// View Port
	glViewport(0, 0, width, height);
	// Cube Map
	int p_cube_location = glGetUniformLocation(cubeMapShaderID, "P");
	int v_cube_location = glGetUniformLocation(cubeMapShaderID, "V");
	glDepthMask(GL_FALSE);
	glUseProgram(cubeMapShaderID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texCube);
	glBindVertexArray(cubeMapVao);
	glUniformMatrix4fv(p_cube_location, 1, GL_FALSE, bunny_mesh.projection.m);
	glUniformMatrix4fv(v_cube_location, 1, GL_FALSE, bunny_mesh.view.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);

	// Uniforms
	int model_location = glGetUniformLocation(reflection_program_id, "model");
	int view_mat_location = glGetUniformLocation(reflection_program_id, "view");
	int proj_mat_location = glGetUniformLocation(reflection_program_id, "proj");
	int ortho_mat_location = glGetUniformLocation(reflection_program_id, "ortho");
	int texture_location = glGetUniformLocation(reflection_program_id, "texture_map");
	int normal_location = glGetUniformLocation(reflection_program_id, "normal_map");
	int height_location = glGetUniformLocation(reflection_program_id, "height_map");
	int lit_outline_thickness_location = glGetUniformLocation(reflection_program_id, "lit_outline_thickness");
	int unlit_outline_thickness_location = glGetUniformLocation(reflection_program_id, "ulit_outline_thickness");
	int wobble_distortion_location = glGetUniformLocation(reflection_program_id, "wobble_distortion");
	int paper_roughness_location = glGetUniformLocation(reflection_program_id, "paper_roughness");
	int diffuse_factor_location = glGetUniformLocation(reflection_program_id, "diffuse_factor");
	int dry_brush_granulation_location = glGetUniformLocation(reflection_program_id, "dry_brush_granulation");
	int dry_brush_density_location = glGetUniformLocation(reflection_program_id, "dry_brush_density");
	// Using Program
	glUseProgram(reflection_program_id);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, paper_texture_id);
	glUniform1i(texture_location, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, paper_normal_id);
	glUniform1i(normal_location, 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, paper_height_id);
	glUniform1i(height_location, 2);
	
	glBindVertexArray(sphereVao);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, bunny_mesh.projection.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, bunny_mesh.view.m);
	glUniformMatrix4fv(model_location, 1, GL_FALSE, bunny_mesh.model.m);
	glUniformMatrix4fv(ortho_mat_location, 1, GL_FALSE, bunny_mesh.ortho.m);
	glDrawArrays(GL_TRIANGLES, 0, sphereMesh.getNumVertices());

	// Uniform Variables
	glUniform3f(reflection_locations.light_position_location, lightPosition.v[0], lightPosition.v[1], lightPosition.v[2]);
	glUniform3f(reflection_locations.view_position_location, camera_pos.v[0], camera_pos.v[1], camera_pos.v[2]);
	glUniform1f(reflection_locations.lit_outline_thickness_location, lit_outline_thickness);
	glUniform1f(reflection_locations.unlit_outline_thickness_location, unlit_outline_thickness);
	glUniform1f(reflection_locations.wobble_distortion_location, wobble_distortion);
	glUniform1f(reflection_locations.paper_roughness_location, paper_roughness);
	glUniform1f(reflection_locations.diffuse_factor_location, diffuse_factor);
	glUniform1f(reflection_locations.dry_brush_granulation_location, dry_brush_granulation);
	glUniform1f(reflection_locations.dry_brush_granulation_location, dry_brush_granulation);
	glUniform1f(reflection_locations.dry_brush_density_location, dry_brush_density);
	// Draw tweakbar
	TwDraw();
	// Swap Buffers
    glutSwapBuffers();
}

void updateScene() {	

	// Wait until at least 16ms passed since start of last frame (Effectively caps framerate at ~60fps)
	static DWORD  last_time = 0;
	DWORD  curr_time = timeGetTime();
	float  delta = (curr_time - last_time) * 0.001f;
	if (delta > 0.03f)
		delta = 0.03f;
	last_time = curr_time;
	// Model Matrix
	rotation_deg += 0.05;
	bunny_mesh.model = rotate_y_deg(identity_mat4(), rotation_deg);
	bunny_mesh.model = scale(bunny_mesh.model, vec3(12.0f, 12.0f, 12.0f));
	bunny_mesh.model = translate(bunny_mesh.model, bunny_position);

	// View matrix
	bunny_mesh.view = look_at(camera_pos,
								camera_front,
								camera_up);
	// Light position
	lightPositionUpdate(lightPositionDirection, lightPosition, lightPositionToggle);
	glutPostRedisplay();
}


void init()
{
	// Set up the shaders
	CompileShaders(reflection_program_id, "../Shaders/vertexShader.glsl", "../Shaders/sumiEShader.glsl");
	CompileShaders(cubeMapShaderID, "../Shaders/cubeMapVertexShader.glsl", "../Shaders/cubeMapFragmentShader.glsl");

	bind_texture(paper_texture_id, "../textures/brush_pattern.jpg");
	bind_mipmap(paper_normal_id, "../textures/watercolor_normal.jpg");
	bind_mipmap(paper_height_id, "../textures/watercolor_normal.jpg");
	// load mesh into a vertex buffer array
	generateObjectBuffer(sphereVao, sphereMesh, reflection_program_id, reflection_locations);
	compile_cube_map(cubeMapVao, cubeMapShaderID, cubemap_shader_locations);
	create_cube_map("../Canvas/negz.jpg",
		"../Canvas/posz.jpg",
		"../Canvas/posy.jpg",
		"../Canvas/negy.jpg",
		"../Canvas/negx.jpg",
		"../Canvas/posx.jpg",
		&texCube);
}

int main(int argc, char** argv){
	// Creating model initial values
	// Model 1 View
	bunny_mesh.projection = perspective(90.0, (float)(width) / (float)height, 0.1, 100.0);
	bunny_mesh.model = scale(identity_mat4(), vec3(100.0f, 100.0f, 100.0f));
	bunny_mesh.ortho = identity_mat4();

	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Assignment 5 - Non Photorrealistic Rendering");

	// Ant Tweak Bar Init
	TwInit(TW_OPENGL, NULL);
	// or
	//TwInit(TW_OPENGL_CORE, NULL); // for core profile
	TwWindowSize(width, height);
	shader_settings = TwNewBar("Shader Settings");
	TwAddVarRW(shader_settings, "Unlit Outline Thickness", TW_TYPE_FLOAT, &unlit_outline_thickness, "label='Unlit Outline Thickness' min=-1 max=1 step=0.05 help='Unlit Outline Thickness'");
	TwAddVarRW(shader_settings, "Lit Outline Thickness", TW_TYPE_FLOAT, &lit_outline_thickness, "label='Lit Outline Thickness' min=-1 max=1 step=0.05 help='Lit Outline Thickness'");
	TwAddVarRW(shader_settings, "Wobble Distortion", TW_TYPE_FLOAT, &wobble_distortion, "label='Wobble Distortion' min=1 max=10 step=0.05 help='Wobble Distortion'");
	TwAddVarRW(shader_settings, "Paper Roughness", TW_TYPE_FLOAT, &paper_roughness, "label='Paper Roughness' min=-1 max=1 step=0.05 help='Paper Roughness'");
	TwAddVarRW(shader_settings, "Diffuse Factor", TW_TYPE_FLOAT, &diffuse_factor, "label='Diffuse Factor' min=0 max=5 step=0.05 help='Diffuse Factor'");
	TwAddVarRW(shader_settings, "Application Parameter", TW_TYPE_FLOAT, &dry_brush_granulation, "label='Application Parameter' min=-1 max=1 step=0.1 help='Application Parameter'");
	TwAddVarRW(shader_settings, "Dry Brush Density", TW_TYPE_FLOAT, &dry_brush_density, "label='Dry Brush Density' min=0.5 max=10 step=0.5 help='Dry Brush Density'");


	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	// glut and tweakbar callbacks
	glutKeyboardFunc(key_press);
	glutSpecialFunc(special_keypress);
	glutMouseFunc(mouse_click);
	glutKeyboardFunc(key_press);
	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT); // same as MouseMotion


	 // A call to glewInit() must be done after glut is initialized!
	glewExperimental = GL_TRUE; //for non-lab machines, this line gives better modern GL support
    GLenum res = glewInit();
	// Check for any errors
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }

	// Set up your objects and shaders
	init();

	// Begin infinite event loop
	glutMainLoop();
	TwTerminate();
	return 0;
    
	return 0;
}
