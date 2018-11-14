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
    void init(GLuint program_id, const char* heightmap_filename);
    void render();
private:
    GLuint heightmapVertexID, heightmapNormalID;
    void loadNormals(std::vector<glm::vec4> vertices);
};
#endif