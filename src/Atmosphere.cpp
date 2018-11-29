#include "Atmosphere.h"

#include "shader.hpp"
#include "error.hpp"

Atmosphere::Atmosphere(const Camera* cam): sun(cam), dome(&sun, cam) {
    program_id_dome= LoadShaders("shaders/preetham_vertex.glsl", "shaders/preetham_frag.glsl");
    errCheck();
}

void Atmosphere::render(GLenum drawMode, GLuint p_sun) {
    sun.render(drawMode, p_sun);

    glUseProgram(program_id_dome);
    dome.render(drawMode, program_id_dome);
errCheck();
}

void Atmosphere::update(double deltatime) {
    sun.update(deltatime);
}

const Sun* Atmosphere::getSun() const {
    return &sun;
}

glm::vec3 Atmosphere::getSunPosition() const {
    return sun.getPosition();
}

void Atmosphere::setProjection(glm::mat4 projection) {
    glUseProgram(program_id_dome);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id_dome, "projection"),
        1,
        GL_FALSE,
        &projection[0][0]
    );
}