//New intel: Use shadowmapping for terrain like this
// https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
// http://ogldev.atspace.co.uk/www/tutorial23/tutorial23.html
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/

//PCF for softer shadows (first link has info)

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

    position = _vertex;
    normal = _normal;
    color = _color;
}