#ifndef ATMOSPHERE
#define ATMOSPHERE

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "SkyDome.h"
#include "Sun.h"

class Atmosphere {
public:
    Atmosphere(const Camera* cam);
    void render(GLenum drawMode, GLuint p_sun);
    void update(double deltatime);
    const Sun* getSun() const;
    
    glm::vec3 getSunPosition() const;
    // glm::vec3 getSunLightColor() const;
    void setProjection(glm::mat4 projection);
private:
    Sun sun;
    SkyDome dome;
    GLuint program_id_dome;
};
#endif