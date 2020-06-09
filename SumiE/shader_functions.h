//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>

// C++ standard libraries
#include <iostream>
#include <string> 
#include <fstream>
#include <iostream>
#include <sstream>

// OpenGL Libraries
#include <GL/glew.h>
#include <GL/freeglut.h>

std::string readShaderSource(const std::string& fileName);

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

GLuint CompileShaders();