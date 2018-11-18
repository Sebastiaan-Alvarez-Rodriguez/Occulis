#ifndef ATMOSPHERE
#define ATMOSPHERE

#include <GL/glew.h>
#include <GL/glu.h>

#include "Camera.h"
#include "SkyDome.h"
#include "Sun.h"

class Atmosphere {
public:
    void init(GLuint skydome_pID, GLuint sun_pID, const Camera* cam);
    void render(GLenum drawMode);
    void update(double deltatime);
private:
    GLuint skydome_pID, sun_pID;
    SkyDome dome;
    Sun sun;
};
#endif