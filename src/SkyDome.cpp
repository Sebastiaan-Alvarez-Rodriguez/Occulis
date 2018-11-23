#include "SkyDome.h"
#include <vector>
#include "util.h"
#include "error.hpp"

SkyDome::SkyDome(const Sun* sun, const Camera* cam): sun(sun), cam(cam) {
    createSkyDome(rings, sectors);
    recalculateSkyData();
    errCheck();
}

void SkyDome::render(GLenum drawMode, GLuint program_id) {
    setModelView(program_id);
    setSkyData(program_id);
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
errCheck();
}

void SkyDome::setModelView(GLuint p) {
    glm::mat4 model = glm::mat4(1.0f);
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

void SkyDome::setSkyData(GLuint p) {
    recalculateSkyData();
    glUniform3f(glGetUniformLocation(p, "A"), skyData.A.x, skyData.A.y, skyData.A.z);
    glUniform3f(glGetUniformLocation(p, "B"), skyData.B.x, skyData.B.y, skyData.B.z);
    glUniform3f(glGetUniformLocation(p, "C"), skyData.C.x, skyData.C.y, skyData.C.z);
    glUniform3f(glGetUniformLocation(p, "D"), skyData.D.x, skyData.D.y, skyData.D.z);
    glUniform3f(glGetUniformLocation(p, "E"), skyData.E.x, skyData.E.y, skyData.E.z);
    glUniform3f(glGetUniformLocation(p, "Z"), skyData.Z.x, skyData.Z.y, skyData.Z.z);
    glUniform3f(glGetUniformLocation(p, "sunDirection"), sunDirection.x, sunDirection.y, sunDirection.z);
}

void SkyDome::recalculateSkyData() {
    //params for below: glm::vec3 sun_direction, float turbidity, float normalizedSunY
    sunDirection = sun->getDirection();
    auto tmp = glm::normalize(sun->getPosition());
    // auto tmp = sunDirection.x;
    // sunDirection.x = sunDirection.y;
    // sunDirection.y =tmp;
    skyData = computeSkyData(tmp, 2, tmp.y);
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
