#include "Sun.h"

#include <vector>
#include <stdexcept>
#include "util.h"
#include "error.hpp"

Sun::Sun(const Camera* cam): cam(cam) {
    createSun(rings, sectors);
    position = {0, -200, 0};
    recalculateModel();
    errCheck();
}

void Sun::render(GLenum drawMode, GLuint program_id) {
    setModelView(program_id);
    setSunPosition(program_id);
    glDisable(GL_DEPTH_TEST);
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
    glEnable(GL_DEPTH_TEST);
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

void Sun::setModelView(GLuint p) {
    glUniformMatrix4fv(
        glGetUniformLocation(p, "model"), 
        1,
        GL_FALSE,
        &model[0][0]
    );
    glm::mat4 view = cam->getView();
    glUniformMatrix4fv(
        glGetUniformLocation(p, "view"), 
        1,
        GL_FALSE,
        &view[0][0]
    );
}

void Sun::recalculateModel() {
    model = glm::translate(glm::mat4(1.0f), position);
}

void Sun::setSunPosition(GLuint p) {
    glUniform3f(glGetUniformLocation(p, "sunLoc"), position.x, position.y, position.z);
}

void Sun::createSun(size_t r, size_t s) {
    std::vector<Data> sunData = createSphere(r, s, radius);
    for (size_t i = 0; i < sunData.size(); ++i)
        sunData[i].color = {253,184,19,255};//make sun yellow/orange-ish
    
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
