#version 330 core

in vec4 position;
in vec4 normal;
in vec4 color;

out vec4 out_color;

void main() {
    out_color = vec4(color.x/255.0f, color.y/255.0f, color.z/255.0f, color.w/255.0f);
}