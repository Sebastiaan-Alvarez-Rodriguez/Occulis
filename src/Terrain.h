#ifndef TERRAIN
#define TERRAIN

#include <utility>

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <vector>

#include "Camera.h"
#include "Wind.h"
#include "Sun.h"
#include "Ground.h"
#include "Grass.h"

class Terrain {
public:
    Terrain(const Camera* cam, const Wind* wind, const Sun* sun);
    void render(GLenum drawMode, GLuint program_id);
    void renderGrass(GLenum drawMode);
    size_t getWidth();
    size_t getHeight();
    void addGrass(glm::vec2 pos, float radius, size_t amt);
    void setProjection(glm::mat4 projection);
    void setLightSpaceMatrix(glm::mat4 matrix);
private:
    const Camera* cam;
    const Wind* wind;
    const Sun* sun;
    Ground ground;
    GLuint program_id_grass;
    std::vector<Grass> grasses;
    
};
#endif