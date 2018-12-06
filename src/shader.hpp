#ifndef SHADER
#define SHADER

#include <GL/glew.h>
#include <GL/glu.h>
#include <string>
#include <vector>

/*
 * Basic shader-compiler helper-class
 */

std::string readFile(const char* file_path);

void compile(const char* file_path, GLuint shaderID);

GLuint createShader(const char* file_path, GLenum shadertype);

GLuint createProgram(std::vector<GLuint> ids);

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
#endif