#version 330 core
layout(location = 0) in vec4 _vertex;
layout(location = 1) in vec4 _normal;
layout(location = 2) in vec4 _color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightSpaceMatrix;

out vec4 position;
out vec4 normal;
out vec4 color;

out vec4 fragPosLightSpace;

void main(){
    gl_Position = projection * view * model * _vertex;

    position = model*_vertex;
    normal = _normal;
    color = _color;

    fragPosLightSpace = lightSpaceMatrix * model * _vertex;
}