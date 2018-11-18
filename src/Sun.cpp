#include "Sun.h"

#include <vector>
#include <stdexcept>
#include "util.h"
#include "error.hpp"

void Sun::init(GLuint program_id) {
    this->program_id = program_id;

    glUseProgram(program_id);
    //createSun(rings, sectors);
    position = {0, -200, 0};
    // recalculateModel();
    // setModel();
    errCheck();
}

void Sun::render(GLenum drawMode) {
    setModel();
    //enable vertices
    glBindBuffer(GL_ARRAY_BUFFER, sunVertexID);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Data), 0);
    //enable normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Data), (void*)(sizeof(glm::vec4)));
    //enable colors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Data), (void*)(2* sizeof(glm::vec4)));

    glDrawArrays(drawMode, 0, rings*sectors*6);

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void Sun::update(double deltatime) {
    angle += deltatime;
    if (angle > 2*M_PI)
        angle = 0;
    position.x = std::sin(angle) * orbit_radius;
    position.y = std::cos(angle) * orbit_radius;
    recalculateModel();
}

glm::vec3 Sun::getDirection() const {
    return glm::normalize(position);
}

glm::vec3 Sun::getPosition() const {
    return position;
}

void Sun::setModel() {
    glUniformMatrix4fv(
        glGetUniformLocation(program_id, "model"), 
        1,
        GL_FALSE,
        &model[0][0]
    );
}

void Sun::recalculateModel() {
    model = glm::translate(glm::mat4(1.0f), position);
}

void Sun::createSun(size_t r, size_t s) {
    std::vector<Data> sunData = createSphere(r, s, radius);
    for (size_t i = 0; i < sunData.size(); ++i)
        sunData[i].color = {253,184,19,255};
    sunVertexID = 0;
    glGenBuffers(1, &sunVertexID);
    glBindBuffer(GL_ARRAY_BUFFER, sunVertexID);
    glBufferData(
        GL_ARRAY_BUFFER,
        sunData.size()*sizeof(Data),
        &sunData[0],
        GL_STATIC_DRAW
    );
}
