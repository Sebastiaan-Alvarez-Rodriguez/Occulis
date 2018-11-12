#version 330 core
layout(location = 0) in vec4 berg;
// layout(location = 1) in vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor;

void main(){
    mat4 mvp = projection * view * model;
    vec4 pos = mvp * berg;
    gl_Position = pos;
    vertexColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}