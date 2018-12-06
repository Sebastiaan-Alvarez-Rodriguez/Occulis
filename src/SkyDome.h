#ifndef SKYDOME
#define SKYDOME

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>

#include "Sun.h"
#include "Camera.h"
#include "preetham.h"

/*
 * Representing a Skydome, upon which the sky is drawn
 */
class SkyDome {
public:
    SkyDome(const Sun* sun, const Camera* cam);
    void render(GLenum drawMode, GLuint program_id);
private:
    // Required pointers to get additional data
    const Sun* sun;
    const Camera* cam;
    
    glm::vec3 sunDirection;

    GLuint domeVertexID;

    // Information about dome
    float radius = 2000.0f;
    const size_t rings = 48, sectors = 48;

    PreethamSkyData skyData;


    // Setters for shaders
    void setModelView(GLuint p);
    void setSkyData(GLuint p);
    // Update function for skydata
    void recalculateSkyData();

    // Create a giant sphere
    void createSkyDome(size_t r, size_t s);
};
#endif