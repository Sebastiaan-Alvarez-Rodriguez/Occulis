#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aOffset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D heightMap;

out vec3 fColor;

float getY(float x, float z) {
    return texture(heightMap, vec2(x,z)).r * 255.0f;
}

void main() {
    float x = aPos.x + aOffset.x;
    float y = aPos.y;//getY(aOffset.x, aOffset.y);
    float z = aPos.z + aOffset.y;
    vec4 instancePos = vec4(x,y,z,1.0);
    gl_Position = projection * view * model * instancePos;
    fColor = aColor.xyz/255.0f;
} 