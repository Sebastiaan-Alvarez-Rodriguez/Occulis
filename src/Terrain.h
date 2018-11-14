#ifndef TERRAIN
#define TERRAIN

#include <utility>

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Terrain {
public:
    void init(GLuint program_id);
    void render();
private:
    GLuint heightmapVertexID, heightmapNormalID, heightmapColorID;
    void loadHeights();
    void loadNormals();
    void loadColors();
    std::vector<glm::vec4> loadPNG(const char* png_filename);
};
#endif