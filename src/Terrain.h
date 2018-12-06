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

/*
 * A class representing a terrain: Bare ground, grass drawn on it
 */
class Terrain {
public:
    Terrain(const Camera* cam, const Wind* wind, const Sun* sun);

    void render(GLenum drawMode, GLuint program_id);

    // Seperate render function for grass patches
    void renderGrass(GLenum drawMode);

    // Basic getters
    size_t getWidth();
    size_t getHeight();

    // Add-grass function, to add grass at specified (X, Z) location with given radius
    void addGrass(glm::vec2 pos, float radius, size_t amt);

    // Basic setters for shaders
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