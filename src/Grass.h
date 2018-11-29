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

//https://github.com/dbaranchuk/grass-simulator-opengl/blob/master/GrassSimulator/Utility/include/Wind.h
//https://github.com/rsteiger/realtime-grass/tree/master/shaders
//instanced:: https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/10.1.instancing_quads/instancing_quads.cpp

struct GData {
    std::vector<glm::vec4> positions;
    std::vector<glm::vec2> rotations;
    std::vector<float> scales;
    std::vector<glm::vec4> colors;
};

class Grass {
public:
    Grass(const Camera* cam, const Wind* wind, const Sun* sun, const Ground* ground, glm::vec2 pos, float radius, size_t amt);
    void render(GLenum drawMode, GLuint program_id);
private:
    const Camera* cam;
    const Wind* wind;
    const Sun* sun;
    const Ground* ground;
    GData data;

    glm::vec4 position;
    float radius;
    size_t instances;
    
    GLuint pointsBuffer, grassPositions, grassRotations, grassScales, grassColor;

    std::vector<glm::vec4> genMesh();


    void generatePositions();
    void generateRotations();
    void generateScales();
    void generateColors();
    void setModelView(GLuint program_id);
    void setWData(GLuint program_id);
    void setSunData(GLuint p);
};
#endif