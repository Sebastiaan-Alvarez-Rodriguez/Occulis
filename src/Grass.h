#ifndef GRASS
#define GRASS

#include <GL/glew.h>
#include <GL/glu.h>

#include <glm/glm.hpp>

#include <vector>

#include "Camera.h"
#include "Wind.h"
#include "Sun.h"
#include "Ground.h"

/*
 * A class representing a grass patch
 */
class Grass {
public:
    // All these parameters are needed to control this grass patch
    Grass(const Camera* cam, const Wind* wind, const Sun* sun, const Ground* ground, glm::vec2 pos, float radius, size_t amt);

    void render(GLenum drawMode, GLuint program_id);

private:
    // All data of this patch, in one struct
    struct GData {
        std::vector<glm::vec4> positions;
        std::vector<glm::vec2> rotations;
        std::vector<float> scales;
        std::vector<glm::vec4> colors;
    };

    // Required pointers to get data from
    const Camera* cam;
    const Wind* wind;
    const Sun* sun;
    const Ground* ground;

    // Stored data
    GData data;

    glm::vec4 position;
    float radius;
    size_t instances;
    
    GLuint pointsBuffer, grassPositions, grassRotations, grassScales, grassColor;

    std::vector<glm::vec4> genMesh();

    // Generate functions to generate required data
    void generatePositions();
    void generateRotations();
    void generateScales();
    void generateColors();

    // Setters for the shader
    void setModelView(GLuint program_id);
    void setWData(GLuint program_id);
    void setSunData(GLuint p);
};
#endif