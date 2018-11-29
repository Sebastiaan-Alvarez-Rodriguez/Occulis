#ifndef UTIL
#define UTIL

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include<vector>
#include <stdexcept>

struct Data {
    glm::vec4 vertex, normal, color;
};

inline size_t randuint(size_t min, size_t max) {
    return std::rand()%(max-min + 1) + min;
}

inline std::vector<Data> createSphere(size_t rings, size_t sectors, float radius) {
    if (rings < 1 || sectors < 1)
        throw std::runtime_error("Sphere created with invalid parameters");
    const float R = 1.0f/(float)(rings-1);
    const float S = 1.0f/(float)(sectors-1);

    std::vector<Data> ret;
    ret.reserve(rings*sectors *6);

    std::vector<glm::vec4> vertices(rings * sectors * 6, {0,0,0,1});
    std::vector<glm::vec4> normals(rings * sectors * 6, {0,0,0,1});
    std::vector<glm::vec4> colors(rings * sectors * 6, {255, 255, 255, 255});
    for(size_t r = 0; r < rings; ++r) 
        for(size_t s = 0; s < sectors; ++s) {
            const float x_0=cos(2*M_PI*s*S)*sin(M_PI*r*R);
            const float y_0=sin(-M_PI_2+M_PI*r*R);
            const float z_0=sin(2*M_PI*s*S)*sin(M_PI*r*R);

            const float x_1=cos(2*M_PI*(s+1)*S)*sin(M_PI*r*R);
            const float y_1=y_0;
            const float z_1=sin(2*M_PI*(s+1)*S)*sin(M_PI*r*R);

            const float x_2=cos(2*M_PI*(s+1)*S)*sin(M_PI*(r+1)*R);
            const float y_2=sin(-M_PI_2+M_PI*(r+1)*R);
            const float z_2=sin(2*M_PI*(s+1)*S)*sin(M_PI*(r+1)*R);

            const float x_3=cos(2*M_PI*s*S)*sin(M_PI*(r+1)*R);
            const float y_3=sin(-M_PI_2+M_PI*(r+1)*R);
            const float z_3=sin(2*M_PI*s*S)*sin(M_PI*(r+1)*R);

            ret.push_back({
                {x_0*radius,y_0*radius,z_0*radius,1},
                {x_0,y_0,z_0,1},
                {255, 255, 255, 255}
            });
            ret.push_back({
                {x_1*radius,y_1*radius,z_1*radius,1},
                {x_1,y_1,z_1,1},
                {255, 255, 255, 255}
            });
            ret.push_back({
                {x_2*radius,y_2*radius,z_2*radius,1},
                {x_2,y_2,z_2,1},
                {255, 255, 255, 255}
            });

            ret.push_back({
                {x_2*radius,y_2*radius,z_2*radius,1},
                {x_2,y_2,z_2,1},
                {255, 255, 255, 255}
            });

            ret.push_back({
                {x_3*radius,y_3*radius,z_3*radius,1},
                {x_3,y_3,z_3,1},
                {255, 255, 255, 255}
            });
            ret.push_back({
                {x_0*radius,y_0*radius,z_0*radius,1},
                {x_0,y_0,z_0,1},
                {255, 255, 255, 255}
            });
    }
    return ret;
}


#endif