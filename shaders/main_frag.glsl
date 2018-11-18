#version 330 core

in vec4 position;
in vec4 normal;
in vec4 color;
in vec4 ShadowCoord;

uniform sampler2D shadowMap;//no more sampler2Dshadow

out vec4 out_color;

// Ambient : simulates indirect lighting
const vec3 ambient_color = vec3(0.1f, 0.1f, 0.1f);
// Diffuse : "color" of the object

// Specular : reflective highlight, like a mirror
const vec3 specular_color = vec3(0.1f, 0.1f, 0.1f);
const vec3 light_color = vec3(0.8f, 0.6f, 0.6f);
const float light_power = 1;

void main() {
    vec2 UVCoords;

    float visibility = 1.0;
    float depth = texture(shadowMap, ShadowCoord.xy).x;
    if(depth < ShadowCoord.z)
        visibility = 0.5;

    out_color = vec4(
        ambient_color + 
        visibility * vec3(color.x/255.0f, color.y/255.0f, color.z/255.0f) *
        light_color * light_power +//*0.4 +  //0.4 was cosTheta
        visibility * specular_color * 
        light_color * light_power, //* 0.05, //0.05 was pow(cosAlpha,5)
        1
    );

}