#version 330 core

in vec4 position;

out vec4 _color;

uniform vec3 A, B, C, D, E, Z;
uniform vec3 sunDirection;

vec3 perez(float cos_theta, float gamma, float cos_gamma, vec3 A, vec3 B, vec3 C, vec3 D, vec3 E) {
    return (1 + A * exp(B / (cos_theta + 0.01))) * (1 + C * exp(D * gamma) + E * cos_gamma * cos_gamma);
}

void main() {
    vec3 direction = normalize(position.xyz);

    vec3 V = normalize(direction);
    float cos_theta = clamp(V.y, 0, 1);
    float cos_gamma = dot(V, sunDirection);
    float gamma = acos(cos_gamma);
    
    vec3 R_xyY = Z * perez(cos_theta, gamma, cos_gamma, A, B, C, D, E);
    
    vec3 R_XYZ = (vec3(R_xyY.x, R_xyY.y, 1 - R_xyY.x - R_xyY.y) * R_xyY.z) / R_xyY.y;
    
    // Radiance
    float r = dot(vec3( 3.240479, -1.537150, -0.498535), R_XYZ);
    float g = dot(vec3(-0.969256,  1.875992,  0.041556), R_XYZ);
    float b = dot(vec3( 0.055648, -0.204043,  1.057311), R_XYZ);

    _color = vec4(r, g, b, 1);
}
