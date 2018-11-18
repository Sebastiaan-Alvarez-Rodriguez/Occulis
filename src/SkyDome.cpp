#include "SkyDome.h"
#include <vector>
#include "util.h"
#include "error.hpp"

void SkyDome::init(GLuint program_id, const Sun* sun, const Camera* cam) {
    this->program_id = program_id;
    this->sun = sun;
    this->cam = cam;
    glUseProgram(program_id);

    createSkyDome(rings, sectors);
    position = {256, 256, 256};
    recalculateModel();
    recalculateSkyData();
    errCheck();
}

void SkyDome::render(GLenum drawMode) {
    setModel();
    setSkyData();
    
    glDisable(GL_DEPTH_TEST);
    //enable vertices
    glBindBuffer(GL_ARRAY_BUFFER, domeVertexID);
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

void SkyDome::setModel() {
    glUniformMatrix4fv(
        glGetUniformLocation(program_id, "model"), 
        1,
        GL_FALSE,
        &model[0][0]
    );
}

void SkyDome::setSkyData() {
    recalculateSkyData();

    
    glUniform3f(glGetUniformLocation(program_id, "A"), skyData.A.x, skyData.A.y, skyData.A.z);
    glUniform3f(glGetUniformLocation(program_id, "B"), skyData.B.x, skyData.B.y, skyData.B.z);
    glUniform3f(glGetUniformLocation(program_id, "C"), skyData.C.x, skyData.C.y, skyData.C.z);
    glUniform3f(glGetUniformLocation(program_id, "D"), skyData.D.x, skyData.D.y, skyData.D.z);
    glUniform3f(glGetUniformLocation(program_id, "E"), skyData.E.x, skyData.E.y, skyData.E.z);
    glUniform3f(glGetUniformLocation(program_id, "Z"), skyData.Z.x, skyData.Z.y, skyData.Z.z);
    glUniform3f(glGetUniformLocation(program_id, "sunDirection"), sunDirection.x, sunDirection.y, sunDirection.z);
}

void SkyDome::recalculateSkyData() {
    //params for below: glm::vec3 sun_direction, float turbidity, float normalizedSunY
    sunDirection = sun->getDirection();
    skyData = computeSkyData(sunDirection, 2, (glm::normalize(sun->getPosition())).y);
}

void SkyDome::recalculateModel() {
    model = glm::translate(glm::mat4(1.0f), position);
}

void SkyDome::createSkyDome(size_t r, size_t s) {
    std::vector<Data> domeData = createSphere(r, s, radius);
    for (size_t i = 0; i < domeData.size(); ++i)
        domeData[i].color = {135,206,235,255};
    domeVertexID = 0;
    glGenBuffers(1, &domeVertexID);
    glBindBuffer(GL_ARRAY_BUFFER, domeVertexID);
    glBufferData(
        GL_ARRAY_BUFFER,
        domeData.size()*sizeof(Data),
        &domeData[0],
        GL_STATIC_DRAW
    );
}
