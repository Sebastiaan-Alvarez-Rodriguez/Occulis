#version 330 core

layout(location = 0) in vec4 _vertex;
layout(location = 1) in vec4 _normal;
layout(location = 2) in vec4 _color;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main() {
    gl_Position = lightSpaceMatrix * model * _vertex;
}