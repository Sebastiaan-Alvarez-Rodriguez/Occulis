#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 _vertex;
layout(location = 1) in vec4 _normal;
layout(location = 2) in vec4 _color;

// Values that stay constant for the whole mesh.
uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main() {
    gl_Position = lightSpaceMatrix * model * _vertex;
}