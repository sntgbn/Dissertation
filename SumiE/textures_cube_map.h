#ifndef _TEXTURES_CUBE_MAP_H
#define _TEXTURES_CUBE_MAP_H

// OpenGL Libraries
#include <GL/glew.h>
#include <GL/freeglut.h>

// C++ Standard libraries
#include <fstream>

// Custom Libraries
#include <shaders_buffers.h>

void bind_texture(GLuint& texture_id, const char* file_name);

void bind_mipmap(GLuint& texture_id, const char* file_name);

void compile_cube_map(GLuint& cubeMapVao, GLuint& cubeMapShaderID, shaderLocations& shaderVariableLocations);

bool load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name);

void create_cube_map(
    const char* front,
    const char* back,
    const char* top,
    const char* bottom,
    const char* left,
    const char* right,
    GLuint* tex_cube);

#endif //_TEXTURES_CUBE_MAP_H