#ifndef _SHADERS_BUFFERS_H
#define _SHADERS_BUFFERS_H

// OpenGL Libraries
#include <GL/glew.h>
#include <GL/freeglut.h>

// C++ Standard libraries
#include <fstream>
#include <iostream>
#include <sstream>
#include <string> 

// Library dependencies
#include <blender_loader.h>

class shaderLocations {
    public:
		GLuint vertex_position_location;
		GLuint vertex_normals_location;
		GLuint texture_coordinates_location;
		GLuint tangents_location;
		GLuint bitangents_location;
		// Uniform variables in shader
		GLuint light_position_location;
		GLuint view_position_location;
		GLuint ambient_strength_location;
		GLuint specular_strength_location;
		GLuint lit_outline_thickness_location;
		GLuint unlit_outline_thickness_location;
		GLuint wobble_distortion_location;
		GLuint paper_roughness_location;
		GLuint diffuse_factor_location;
		GLuint dry_brush_granulation_location;
		GLuint dry_brush_density_location;
};

void generateObjectBuffer(GLuint& objectVao, BlenderObj& blenderObject, GLuint& shaderProgramID, shaderLocations& shaderVariableLocations);

std::string readShaderSource(const std::string& fileName);

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

GLuint CompileShaders(GLuint& shaderProgramID, std::string vertex_shader, std::string fragment_shader);

#endif //_SHADERS_BUFFERS_H