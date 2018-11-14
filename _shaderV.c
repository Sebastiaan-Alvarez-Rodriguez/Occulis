#version 330 core
layout(location = 0) in vec4 _vertex;
layout(location = 1) in vec4 _normal;
layout(location = 2) in vec4 _color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 position;
out vec4 normal;
out vec4 color;

void main(){
    mat4 mvp = projection * view * model;
    vec4 pos = mvp * _vertex;
    gl_Position = pos;

    position = _vertex;//pos?
    normal = _normal;
    color = _color;
    // color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}