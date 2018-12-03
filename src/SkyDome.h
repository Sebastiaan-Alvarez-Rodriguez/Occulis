#ifndef SKYDOME
#define SKYDOME

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Sun.h"
#include "Camera.h"
#include "preetham.h"

class SkyDome {
public:
    SkyDome(const Sun* sun, const Camera* cam);
    void render(GLenum drawMode, GLuint program_id);
private:
    const Sun* sun;
    const Camera* cam;
    
    glm::vec3 sunDirection;

    GLuint domeVertexID;

    float radius = 2000.0f;
    const size_t rings = 48, sectors = 48;

    PreethamSkyData skyData;


    void setModelView(GLuint p);
    void setSkyData(GLuint p);
    void recalculateSkyData();
    void createSkyDome(size_t r, size_t s);
};
#endif