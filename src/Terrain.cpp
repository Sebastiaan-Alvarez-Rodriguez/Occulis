#include "Terrain.h"

#include "util.h"
#include "shader.hpp"
#include "error.hpp"

Terrain::Terrain(const Camera* cam, const Wind* wind, const Sun* sun): cam(cam), wind(wind), sun(sun), ground(cam) {
    program_id_grass=LoadShaders("shaders/grass_vertex.glsl", "shaders/grass_frag.glsl");
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
    grasses.push_back(Grass(cam, wind, sun, &ground, pos, radius, amt));
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

void Terrain::setLightSpaceMatrix(glm::mat4 matrix) {
    glUseProgram(program_id_grass);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id_grass, "lightSpaceMatrix"),
        1,
        GL_FALSE,
        &matrix[0][0]
    );
}