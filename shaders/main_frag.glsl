#version 330 core

in vec4 position;
in vec4 normal;
in vec4 color;
in vec4 fragPosLightSpace;

uniform sampler2D shadowMap;
uniform vec3 sunLoc;
uniform vec3 sunColor;


out vec4 out_color;
const vec4 sunStrength = vec4(0.5f, 0.5f, 0.5f, 1.0f);
const vec4 ambient_color = vec4(0.3f, 0.3f, 0.3f, 1.0f);


float calculateLight(vec4 fragPosLightSpace) {
    if (sunLoc.y < 0)
        return 0.3f;
    const float bias = 0.0005f;
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5f + 0.5f;

    if(projCoords.z > 1.0f)
        return 1.0f;

    float light = 0.0f;
    float currentDepth = projCoords.z;
    vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            light += currentDepth > pcfDepth+bias ? 0.3f : 1.0f;
        }
    light /= 9.0f;
    return light;
}

void main() {
    float visibility = calculateLight(fragPosLightSpace);

    vec4 regular_color = color/255.0f;
    out_color = 
        ambient_color * regular_color + 
        visibility * regular_color * vec4(sunColor/255.0f, 1.0f) * sunStrength;
    ;
    // out_color = vec4(
    //     ambient_color + 
    //     visibility * regular_color.xyz *
    //     light_color * light_power +//*0.4 +  //0.4 was cosTheta
    //     visibility * specular_color * 
    //     light_color * light_power, //* 0.05, //0.05 was pow(cosAlpha,5)
    //     1
    // );

}