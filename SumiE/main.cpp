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
vec3 camera_pos(0.0f, 0.0f, 2.0f);
vec3 camera_front(0.0f, 0.0f, -10.0f); // front z -10.0f;
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
float unlit_outline_thickness = 0.25f; // specular_strength_location
float lit_outline_thickness = 0.15f; // ambient_strength_location
float solid_outline_color = 0.0f;
bool outline_selection = false;
float wobble_distortion = 0.0f;
bool cel_shader_selection = true;
bool texture_selection = false;
float texture_luminance = 1.0f;
float paper_alpha_thresh = 1.0f;
float paper_alpha_div = 0.0f;

// Model Load Variables & VAO Variables
//BlenderObj sphereMesh("../meshes/airplane_incomplete_smooth.obj");
BlenderObj sphereMesh("../meshes/bunny.obj");
//BlenderObj sphereMesh("../meshes/bamboo.obj");
GLuint sphereVao;
GLuint cubeMapVao;
GLuint texCube;

// Additional mesh VAOS for airplane
BlenderObj propellerMesh("../meshes/bunny.obj");
GLuint propellerVao;
BlenderObj wheelMesh("../meshes/bunny.obj");
GLuint wheelVao;

// Mesh Projection Matrices
ProjectionMatrices bunny_mesh;
//vec3 bunny_position = vec3(0.0f, -1.0f, -25.0f); // airplane
vec3 bunny_position = vec3(0.0f, -1.0f, -3.0f);
//vec3 bunny_position = vec3(0.0f, -1.0f, -3.0f); // ball
//vec3 bunny_position = vec3(0.5f, -1.9f, -1.5f); //Sumi-E bamboo
//vec3 bunny_position = vec3(0.5f, -1.9f, -1.0f); //bamboo right side
ProjectionMatrices propeller_mesh;
ProjectionMatrices wheelR_mesh;
ProjectionMatrices wheelL_mesh;
ProjectionMatrices wheelC_mesh;
float rotation_deg = 0;
float rotation_deg_wheel_prop = 0;
bool bunny_rotation_toggle = false;

// Texture/Normal map ID
GLuint brush_texture_id;
GLuint mesh_texture_id;
GLuint tone_texture_id;

// Tweak Bar
TwBar* shader_settings;

void display() {
	// tell GL to only draw onto a pixel if the shape is closer to the viewer	
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// View Port
	glViewport(0, 0, width, height);

	// Cube Map
	int p_cube_location = glGetUniformLocation(cubeMapShaderID, "P");
	int v_cube_location = glGetUniformLocation(cubeMapShaderID, "V");
	glDepthMask(GL_FALSE);
	// Uncomment to draw cubemap
	glUseProgram(cubeMapShaderID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texCube);
	glBindVertexArray(cubeMapVao);	
	glUniformMatrix4fv(p_cube_location, 1, GL_FALSE, bunny_mesh.projection.m);
	glUniformMatrix4fv(v_cube_location, 1, GL_FALSE, bunny_mesh.view.m);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// Cubemap part ends here
	glDepthMask(GL_TRUE);

	// Blending part goes here
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);     // Cull back facing polygons
	glCullFace(GL_BACK);

	// Uniforms
	int model_location = glGetUniformLocation(reflection_program_id, "model");
	int view_mat_location = glGetUniformLocation(reflection_program_id, "view");
	int proj_mat_location = glGetUniformLocation(reflection_program_id, "proj");
	int ortho_mat_location = glGetUniformLocation(reflection_program_id, "ortho");
	int brush_texture_location = glGetUniformLocation(reflection_program_id, "brush_texture_map");
	int mesh_texture_location = glGetUniformLocation(reflection_program_id, "mesh_texture_map");
	int tone_texture_location = glGetUniformLocation(reflection_program_id, "tone_texture_map");

	int lit_outline_thickness_location = glGetUniformLocation(reflection_program_id, "lit_outline_thickness");
	int unlit_outline_thickness_location = glGetUniformLocation(reflection_program_id, "ulit_outline_thickness");
	int solid_outline_color_location = glGetUniformLocation(reflection_program_id, "solid_outline_color_location");
	int outline_selection_location = glGetUniformLocation(reflection_program_id, "outline_selection_location");
	int wobble_distortion_location = glGetUniformLocation(reflection_program_id, "wobble_distortion");
	int cel_shader_selection_location = glGetUniformLocation(reflection_program_id, "cel_shader_selection");
	int texture_selection_location = glGetUniformLocation(reflection_program_id, "texture_selection");
	int texture_luminance_location = glGetUniformLocation(reflection_program_id, "texture_luminance");
	int paper_alpha_threshold_location = glGetUniformLocation(reflection_program_id, "paper_alpha_threshold");
	int paper_alpha_div_location = glGetUniformLocation(reflection_program_id, "paper_alpha_div");
	// Using Program
	glUseProgram(reflection_program_id);

	// 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brush_texture_id);
	glUniform1i(brush_texture_location, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mesh_texture_id);
	glUniform1i(mesh_texture_location, 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tone_texture_id);
	glUniform1i(tone_texture_location, 2);

	// main model
	glBindVertexArray(sphereVao);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, bunny_mesh.projection.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, bunny_mesh.view.m);
	glUniformMatrix4fv(model_location, 1, GL_FALSE, bunny_mesh.model.m);
	glUniformMatrix4fv(ortho_mat_location, 1, GL_FALSE, bunny_mesh.ortho.m);
	glDrawArrays(GL_TRIANGLES, 0, sphereMesh.getNumVertices());

	//// propeller & wheel model
	//glBindVertexArray(propellerVao);
	//glUniformMatrix4fv(model_location, 1, GL_FALSE, propeller_mesh.model.m);
	//glDrawArrays(GL_TRIANGLES, 0, propellerMesh.getNumVertices());
	//glBindVertexArray(wheelVao);
	//glUniformMatrix4fv(model_location, 1, GL_FALSE, wheelC_mesh.model.m);	
	//glDrawArrays(GL_TRIANGLES, 0, wheelMesh.getNumVertices());
	//glUniformMatrix4fv(model_location, 1, GL_FALSE, wheelR_mesh.model.m);
	//glDrawArrays(GL_TRIANGLES, 0, wheelMesh.getNumVertices());
	//glUniformMatrix4fv(model_location, 1, GL_FALSE, wheelL_mesh.model.m);
	//glDrawArrays(GL_TRIANGLES, 0, wheelMesh.getNumVertices());

	// Uniform Variables
	glUniform3f(reflection_locations.light_position_location, lightPosition.v[0], lightPosition.v[1], lightPosition.v[2]);
	glUniform3f(reflection_locations.view_position_location, camera_pos.v[0], camera_pos.v[1], camera_pos.v[2]);
	glUniform1f(reflection_locations.lit_outline_thickness_location, lit_outline_thickness);
	glUniform1f(reflection_locations.unlit_outline_thickness_location, unlit_outline_thickness);
	glUniform1f(reflection_locations.solid_outline_color_location, solid_outline_color);
	glUniform1f(reflection_locations.outline_selection_location, outline_selection);
	glUniform1f(reflection_locations.cel_shader_selection_location, cel_shader_selection);
	glUniform1f(reflection_locations.texture_selection_location, texture_selection);
	glUniform1f(reflection_locations.texture_luminance_location, texture_luminance);
	glUniform1f(reflection_locations.wobble_distortion_location, wobble_distortion);
	glUniform1f(reflection_locations.paper_alpha_threshold_location, paper_alpha_thresh);
	glUniform1f(reflection_locations.paper_alpha_div_location, paper_alpha_div);
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

	// camera update
	camera_front = camera_pos;
	camera_front.v[2] -= 10.0f;
	//std::cout << "Camera  Pos --> X: " << camera_pos.v[0] << ", Y: "<<camera_pos.v[1] << ", Z:" <<camera_pos.v[2] << std::endl;
	//std::cout << "Camera Frnt --> X: " << camera_front.v[0] << ", Y: " << camera_front.v[1] << ", Z:" << camera_front.v[2] << std::endl;
	// Model Matrix
	if (bunny_rotation_toggle == true) {
		rotation_deg += 0.075;
	}
	rotation_deg_wheel_prop += 0.5;
	

	bunny_mesh.model = scale(identity_mat4(), vec3(12, 12, 12));
	bunny_mesh.model = rotate_y_deg(bunny_mesh.model, rotation_deg);
	//bunny_mesh.model = scale(bunny_mesh.model, vec3(0.008f, 0.008f, 0.008f)); // Scale bamboo tree
	//bunny_mesh.model = scale(bunny_mesh.model, vec3(0.025f, 0.025f, 0.025f)); // Plane scale
	bunny_mesh.model = translate(bunny_mesh.model, bunny_position);
	

	// View matrix
	bunny_mesh.view = look_at(camera_pos,
								camera_front,
								camera_up);


	//Hierarchy models
	// Propeller
	propeller_mesh = bunny_mesh;
	propeller_mesh.model = translate(bunny_mesh.model, bunny_position);
	//propeller_mesh.model = rotate_z_deg(identity_mat4(), rotation_deg_wheel_prop);
	//propeller_mesh.model = translate(propeller_mesh.model, vec3(0, -45, -400));
	//propeller_mesh.model = bunny_mesh.model * propeller_mesh.model;
	// Wheel Back
	wheelC_mesh.model = rotate_x_deg(identity_mat4(), rotation_deg_wheel_prop);
	wheelC_mesh.model = translate(wheelC_mesh.model, vec3(0, -125, 265));
	wheelC_mesh.model = bunny_mesh.model * wheelC_mesh.model;
	// Wheel Right
	wheelR_mesh.model = rotate_x_deg(identity_mat4(), rotation_deg_wheel_prop);
	wheelR_mesh.model = translate(wheelR_mesh.model, vec3(-105, -155, -230));
	wheelR_mesh.model = bunny_mesh.model * wheelR_mesh.model;
	// Wheel Left
	wheelL_mesh.model = rotate_x_deg(identity_mat4(), rotation_deg_wheel_prop);
	wheelL_mesh.model = translate(wheelL_mesh.model, vec3(105, -155, -230));
	wheelL_mesh.model = bunny_mesh.model * wheelL_mesh.model;

	// Light position
	lightPositionUpdate(lightPositionDirection, lightPosition, lightPositionToggle);

	glutPostRedisplay();

	//// String Selection
	//std::ostringstream texture_path_stream;
	//texture_path_stream << "../textures/brush_pattern_" << texture_selection << ".jpg";
	//bind_texture(paper_texture_id, texture_path_stream.str().c_str());
}


void init()
{
	// Set up the shaders
	CompileShaders(reflection_program_id, "../Shaders/vertexShader.glsl", "../Shaders/sumiEShader.glsl");
	CompileShaders(cubeMapShaderID, "../Shaders/cubeMapVertexShader.glsl", "../Shaders/cubeMapFragmentShader.glsl");

	bind_texture(brush_texture_id, "../textures/brush_pattern_3.jpg");
	bind_texture(mesh_texture_id, "../textures/orange_texture.jpg");
	bind_texture(tone_texture_id, "../textures/tone_texture.jpg");

	// load mesh into a vertex buffer array
	generateObjectBuffer(sphereVao, sphereMesh, reflection_program_id, reflection_locations);
	generateObjectBuffer(propellerVao, propellerMesh, reflection_program_id, reflection_locations);
	generateObjectBuffer(wheelVao, wheelMesh, reflection_program_id, reflection_locations);
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
    glutCreateWindow("Dissertation - SumiE Shader");

	// Ant Tweak Bar Init
	TwInit(TW_OPENGL, NULL);
	// or
	//TwInit(TW_OPENGL_CORE, NULL); // for core profile
	TwWindowSize(width, height);
	shader_settings = TwNewBar("Shader Settings");
	TwAddVarRW(shader_settings, "Unlit Outline", TW_TYPE_FLOAT, &unlit_outline_thickness, "label='Unlit Outline Thickness' min=-10 max=10 step=0.05 help='Unlit Outline Thickness'");
	TwAddVarRW(shader_settings, "Lit Outline", TW_TYPE_FLOAT, &lit_outline_thickness, "label='Lit Outline Thickness' min=-10 max=10 step=0.01 help='Lit Outline Thickness'");
	TwAddVarRW(shader_settings, "Outline Selection", TW_TYPE_BOOLCPP, &outline_selection, "label='Outline Selection' help='Outline Selection'");
	TwAddVarRW(shader_settings, "Solid Outline Color", TW_TYPE_FLOAT, &solid_outline_color, "label='Solid Outline Color' min=0 max=1.0 step=0.01 help='Solid Outline Color'");
	TwAddVarRW(shader_settings, "Wobble Distortion", TW_TYPE_FLOAT, &wobble_distortion, "label='Wobble Distortion' min=-5 max=5 step=0.01 help='Wobble Distortion'");
	TwAddVarRW(shader_settings, "Cel Shader Selection", TW_TYPE_BOOLCPP, &cel_shader_selection, "label='Cel Shader Selection' help='Cel Shader Selection'");
	TwAddVarRW(shader_settings, "Texture Selection", TW_TYPE_BOOLCPP, &texture_selection, "label='Texture Selection' help='Texture Selection'");
	TwAddVarRW(shader_settings, "Texture Luminance", TW_TYPE_FLOAT, &texture_luminance, "label='Texture Luminance' min=1 max=2.0 step=0.01 help='Texture Luminance'");
	TwAddVarRW(shader_settings, "Paper A Thresh ", TW_TYPE_FLOAT, &paper_alpha_thresh, "label='Paper A Thresh' min=0 max=1 step=0.001 help='Paper A Thresh'");
	TwAddVarRW(shader_settings, "Paper A Thresh Skip", TW_TYPE_FLOAT, &paper_alpha_div, "label='Paper A Thresh Skip' min=0 max=1.0 step=0.001 help='Paper A Thresh Skip'");
	
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	// glut and tweakbar callbacks
	glutKeyboardFunc(key_press);
	glutSpecialFunc(special_keypress);
	glutKeyboardFunc(key_press);
	glutMouseFunc(mouse_wheel_click);
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
	