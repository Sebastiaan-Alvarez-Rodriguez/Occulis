#include "shader.hpp"

#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

std::string readFile(const char* file_path) {
    std::string shaderCode;
    std::ifstream shaderStream(file_path, std::ios::in);
    if(shaderStream.is_open()){
        std::stringstream sstr;
        sstr << shaderStream.rdbuf();
        shaderCode = sstr.str();
        shaderStream.close();
    }else{
        throw std::runtime_error("Impossible to open "+std::string(file_path)+". Are you in the right directory?");
    }
    return shaderCode;
}

void compile(const char* file_path, GLuint shaderID) {
    // Read the shader code from the file
    printf("Reading shader : %s\n", file_path);

    std::string shaderCode = readFile(file_path);

    printf("Compiling shader : %s\n", file_path);
    
    const char* sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer , NULL);
    glCompileShader(shaderID);

    // Check shader
    int InfoLogLength;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0) {
        GLchar* shaderErrorMessage = (GLchar*) malloc(InfoLogLength);
        glGetShaderInfoLog(shaderID, InfoLogLength, NULL, shaderErrorMessage);
        std::cerr << shaderErrorMessage << std::endl;
        free(shaderErrorMessage);
        throw std::runtime_error("Shader compile error(s) detected");
    }
}

GLuint createShader(const char* file_path, GLenum shadertype) {
    // Create the shaders
    GLuint id = glCreateShader(shadertype);
    compile(file_path, id);
    return id;
}

GLuint createProgram(std::vector<GLuint> ids) {
    // Link the program
    printf("Linking program\n");
    GLuint program_id = glCreateProgram();
    for (GLuint id : ids)
        glAttachShader(program_id, id);
    glLinkProgram(program_id);

    // Check the program
    printf("Checking program\n");

    int InfoLogLength;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0) {
        GLchar* shaderErrorMessage = (GLchar*) malloc(InfoLogLength);
        glGetProgramInfoLog(program_id, InfoLogLength, NULL, shaderErrorMessage);
        std::cerr << shaderErrorMessage << std::endl;
        free(shaderErrorMessage);
        throw std::runtime_error("Shader linking error(s) detected");
    }
    for(GLuint id : ids)
        glDetachShader(program_id, id);
    for(GLuint id : ids)
        glDeleteShader(id);
    printf("Program complete\n");
    return program_id;
}

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {
    GLuint vertexShaderID = createShader(vertex_file_path, GL_VERTEX_SHADER);
    GLuint fragmentShaderID = createShader(fragment_file_path, GL_FRAGMENT_SHADER);
    return createProgram({vertexShaderID, fragmentShaderID});
}