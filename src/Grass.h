#ifndef GRASS
#define GRASS

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>

#include "Camera.h"

class Grass {
public:
    Grass(const Camera* cam, GLuint heightTexture, glm::vec2 position, float radius, size_t amt);
    void render(GLenum drawMode, GLuint program_id);
private:
    const Camera* cam;
    GLuint bladeID, offsetID, heightTextureID;

    glm::mat4 model;
    glm::vec2 position;
    float radius;
    size_t amt;
    void generateBlade();
    void generatePositions();
    void setModelView(GLuint p);
    void recalculateModel();
    void setTexture(GLuint p);
};
#endif