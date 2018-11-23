#version 330 core

layout(location = 0) in vec4 _vertex;
layout(location = 1) in vec4 _normal;
layout(location = 2) in vec4 _color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 position;

void main() {
    gl_Position = projection * view * model * _vertex;
    position = model*_vertex;
}