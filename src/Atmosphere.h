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
    void init(GLuint skydome_pID, GLuint sun_pID);
    void render(GLenum drawMode);
    void update(double deltatime);
    void printSunDir();
    glm::vec3 getSunPosition() const;
private:
    GLuint skydome_pID, sun_pID;
    SkyDome dome;
    Sun sun;
};
#endif