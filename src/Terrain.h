#ifndef TERRAIN
#define TERRAIN

#include <utility>

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Image.h"
#include "ImageReader.h"

class Terrain {
public:
    void init(GLuint program_id);
    void render();
private:
    GLuint heightmapVertexID, heightmapNormalID, heightmapColorID;
};
#endif