#include "Terrain.h"

#include "util.h"
#include "shader.hpp"
#include "error.hpp"

Terrain::Terrain(const Camera* cam): cam(cam), ground(cam) {
    program_id_grass=LoadShaders("shaders/grass_vertex.glsl", "shaders/grass_frag.glsl");
    heightTexID = 0;
    errCheck();
}

void Terrain::render(GLenum drawMode, GLuint program_id) {
    ground.render(drawMode, program_id);
}

void Terrain::renderGrass(GLenum drawMode) {
    glUseProgram(program_id_grass);
    for (Grass& g : grasses)
        g.render(drawMode, program_id_grass);
}

size_t Terrain::getWidth() {
    return ground.getWidth();
}

size_t Terrain::getHeight() {
    return ground.getHeight();
}

void Terrain::addGrass(glm::vec2 pos, float radius, size_t amt) {
    if (heightTexID == 0)
        heightTexID = ground.genHeightmap();
    grasses.push_back(Grass(cam, heightTexID, pos, radius, amt));
}
void Terrain::setProjection(glm::mat4 projection) {
    glUseProgram(program_id_grass);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id_grass, "projection"),
        1,
        GL_FALSE,
        &projection[0][0]
    );
}