#ifndef PREETHAM
#define PREETHAM

//  https://github.com/ddiakopoulos/sandbox/blob/master/gl/gl-procedural-sky.hpp
#define _USE_MATH_DEFINES
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>

struct PreethamSkyData {
    glm::vec3 A, B, C, D, E, Z;
};

inline float perez(float theta, float gamma, float A, float B, float C, float D, float E) {
    return (1.f + A * std::exp(B / (std::cos(theta) + 0.01))) * (1.f + C * std::exp(D * gamma) + E * std::cos(gamma) * std::cos(gamma));
}

inline float zenith_luminance(float sunTheta, float turbidity) {
    float chi = (4.f / 9.f - turbidity / 120) * (M_PI - 2 * sunTheta);
    return (4.0453 * turbidity - 4.9710) * std::tan(chi) - 0.2155 * turbidity + 2.4192;
}
    
inline float zenith_chromacity(const glm::vec4 & c0, const glm::vec4 & c1, const glm::vec4 & c2, float sunTheta, float turbidity) {
    glm::vec4 thetav = glm::vec4(sunTheta * sunTheta * sunTheta, sunTheta * sunTheta, sunTheta, 1);
    return glm::dot(glm::vec3(turbidity * turbidity, turbidity, 1), glm::vec3(glm::dot(thetav, c0), glm::dot(thetav, c1), glm::dot(thetav, c2)));
}

inline PreethamSkyData computeSkyData(glm::vec3 sun_direction, float turbidity, float normalizedSunY) {
    if (turbidity <= 1.0f) {
        throw std::runtime_error("Attempting to compute Preetham skydata for turbidity <= 1");
    }
    
    const float sunTheta = std::acos(glm::clamp(sun_direction.y, 0.f, 1.f));

    // A.2 Skylight Distribution Coefficients and Zenith Values: compute Perez distribution coefficients
    glm::vec3 A = glm::vec3(-0.0193, -0.0167,  0.1787) * turbidity + glm::vec3(-0.2592, -0.2608, -1.4630);
    glm::vec3 B = glm::vec3(-0.0665, -0.0950, -0.3554) * turbidity + glm::vec3( 0.0008,  0.0092,  0.4275);
    glm::vec3 C = glm::vec3(-0.0004, -0.0079, -0.0227) * turbidity + glm::vec3( 0.2125,  0.2102,  5.3251);
    glm::vec3 D = glm::vec3(-0.0641, -0.0441,  0.1206) * turbidity + glm::vec3(-0.8989, -1.6537, -2.5771);
    glm::vec3 E = glm::vec3(-0.0033, -0.0109, -0.0670) * turbidity + glm::vec3( 0.0452,  0.0529,  0.3703);
    
    // A.2 Skylight Distribution Coefficients and Zenith Values: compute zenith color
    glm::vec3 Z = {
        zenith_chromacity(glm::vec4(0.00166, -0.00375, 0.00209, 0), glm::vec4(-0.02903, 0.06377, -0.03202, 0.00394), glm::vec4(0.11693, -0.21196, 0.06052, 0.25886), sunTheta, turbidity),
        zenith_chromacity(glm::vec4(0.00275, -0.00610, 0.00317, 0), glm::vec4(-0.04214, 0.08970, -0.04153, 0.00516), glm::vec4(0.15346, -0.26756, 0.06670, 0.26688), sunTheta, turbidity),
        zenith_luminance(sunTheta, turbidity) * 1000// conversion from kcd/m^2 to cd/m^2
    };
    
    // 3.2 Skylight Model: pre-divide zenith color by distribution denominator
    Z.x /= perez(0, sunTheta, A.x, B.x, C.x, D.x, E.x);
    Z.y /= perez(0, sunTheta, A.y, B.y, C.y, D.y, E.y);
    Z.z /= perez(0, sunTheta, A.z, B.z, C.z, D.z, E.z);
    
    // For low dynamic range simulation, normalize luminance to have a fixed value for sun
    if (normalizedSunY) 
        Z.z = normalizedSunY / perez(sunTheta, 0, A.z, B.z, C.z, D.z, E.z);
    
    return {A, B, C, D, E, Z};
}

#endif