#ifndef TERRAIN
#define TERRAIN

#include <utility>

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <vector>

#include "Camera.h"
#include "Ground.h"
#include "Grass.h"
//Ideas: Implement grass (with instanced drawArrays call!!!!
    //  https://learnopengl.com/Advanced-OpenGL/Instancing
class Terrain {
public:
    Terrain(const Camera* cam);
    void render(GLenum drawMode, GLuint program_id);
    void renderGrass(GLenum drawMode);
    size_t getWidth();
    size_t getHeight();
    void addGrass(glm::vec2 pos, float radius, size_t amt);
    void setProjection(glm::mat4 projection);
private:
    const Camera* cam;
    Ground ground;
    GLuint heightTexID;
    GLuint program_id_grass;
    std::vector<Grass> grasses;
    
};
#endif