//New intel: Use shadowmapping for terrain like this
// https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
// Download git link for above thing
//      https://github.com/opengl-tutorials/ogl/blob/master/tutorial16_shadowmaps/ShadowMapping.fragmentshader
// http://ogldev.atspace.co.uk/www/tutorial23/tutorial23.html
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/



// https://en.wikipedia.org/wiki/Shadow_mapping
//PCF for softer shadows (first link has info)

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

//om de een of andere reden lijkt shader last te hebben van de view matrix van camera
void main(){
    gl_Position = projection * view * model * _vertex;

    position = model*_vertex;
    normal = _normal;//transpose(inverse(model))*_normal
    color = _color;
    //Dit kan ook zonder model, en dan zelf een test4/ shade doen
    fragPosLightSpace = lightSpaceMatrix * model * _vertex;
}