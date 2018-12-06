#version 330 core

in vec4 point;
in vec4 position;
in vec2 rotation;
in float scale;
in vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightSpaceMatrix;

uniform vec4 direction;
uniform float strength;
uniform float baseAngle;

out vec4 inColor;
out vec4 fragPosLightSpace;

// Compute all wind-based movements of grass here
vec4 computeWindStuff() {
    if (point.y * scale < 0.1)
        return vec4(0,0,0,0);
    return scale * direction * strength * sin(baseAngle+0.01f*position.x) * point.y* point.y* point.y* point.y;
}

void main() {
    mat4 posRotMatrix = mat4(1.0f);
    posRotMatrix[0][0] = rotation[0];
    posRotMatrix[0][2] = rotation[1];
    posRotMatrix[2][2] = rotation[0];
    posRotMatrix[2][0] = -rotation[1];
    posRotMatrix[3][0] = position.x;
    posRotMatrix[3][1] = position.y;
    posRotMatrix[3][2] = position.z;
    /*
        Visualization of above matrix:
        rot[0]  0 -rot[1]  posX
        0       1  0       posY
        rot[1]  0  rot[0]  posZ
        0       0  0       1
        This matrix applies rotation and translation simultaniously, which is efficient
    */
    vec4 _vertex =  (posRotMatrix *(vec4(1, scale*8, 1, 1) * point) + computeWindStuff());
    gl_Position = projection * view * model * _vertex;
    inColor = color * 0.7 * (point.y + 0.8);
    fragPosLightSpace = lightSpaceMatrix * model * _vertex;
}