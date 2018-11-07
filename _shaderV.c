#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor;

void main(){
    // mat4 mvp = projection * view * model;
    vec4 pos = vec4(vertexPosition_modelspace, 1.0);
    gl_Position = pos;
    vertexColor = color;
}