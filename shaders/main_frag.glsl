#version 330 core

in vec4 position;
in vec4 normal;
in vec4 color;
in vec4 fragPosLightSpace;

uniform sampler2D shadowMap;

out vec4 out_color;

const vec3 ambient_color = vec3(0.1f, 0.1f, 0.1f);
const vec3 specular_color = vec3(0.1f, 0.1f, 0.1f);

const vec3 light_color = vec3(1.0f,1.0f,1.0f);
const float light_power = 1;

float calculateLight(vec4 fragPosLightSpace) {
    const float bias = 0.0005;
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)
        return 1.0;

    float shadow = 0.0;
    float currentDepth = projCoords.z;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth > pcfDepth+bias ? 0.3 : 1.0;
        }
    shadow /= 9.0;
    return shadow;
    // // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    // float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // // get depth of current fragment from light's perspective
    // float currentDepth = projCoords.z;
    // // check whether current frag pos is in shadow
    // //+0.005 of + 0.0005
    // float shadow = currentDepth > closestDepth+0.0005  ? 0.3 : 1.0;

    // return shadow;
}

void main() {
    float visibility = calculateLight(fragPosLightSpace);

    vec3 regular_color = vec3(color.x/255.0f, color.y/255.0f, color.z/255.0f);
    out_color = vec4(ambient_color + visibility * regular_color, 1);
    // out_color = vec4(
    //     ambient_color + 
    //     visibility * regular_color.xyz *
    //     light_color * light_power +//*0.4 +  //0.4 was cosTheta
    //     visibility * specular_color * 
    //     light_color * light_power, //* 0.05, //0.05 was pow(cosAlpha,5)
    //     1
    // );

}