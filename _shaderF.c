#version 330 core

in vec4 vertexColor;

out vec4 color;

void main(){
    color.xyzw = vertexColor.xyzw;
}