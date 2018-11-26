#include "Grass.h"

#include <vector>
//https://github.com/dbaranchuk/grass-simulator-opengl/blob/master/GrassSimulator/Utility/include/Wind.h
//https://github.com/rsteiger/realtime-grass/tree/master/shaders
//instanced:: https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/10.1.instancing_quads/instancing_quads.cpp
Grass::Grass(const Camera* cam, GLuint heightTexture, glm::vec2 position, float radius, size_t amt): 
    cam(cam), heightTextureID(heightTexture), position(position), radius(radius), amt(amt) {
    generateBlade();
    generatePositions();
    recalculateModel();
}

void Grass::render(GLenum drawMode, GLuint program_id) {
    glDisable(GL_CULL_FACE);
    setModelView(program_id);
    // setTexture(program_id);
    //enable vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, bladeID);//bladeID->1 triangle ('grass')
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec4), 0);
    //enable colors
    glEnableVertexAttribArray(1);// -> green color for all 3 points of grass triangle
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec4), (void*)(sizeof(glm::vec4)));
    //enable offsets
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, offsetID);//offsetID->amt offsets, for amt instances
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
    glVertexAttribDivisor(2, 1);
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, heightTextureID);

    glDrawArrays(drawMode, 0, 6);

    // glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glEnable(GL_CULL_FACE);
}

void Grass::generateBlade() {
    std::vector<glm::vec4> data = {
        {0,0,0,1},
        {96, 128, 56, 255},
        {2,0,0,1},
        {96, 128, 56, 255},
        {1,100,0,1},
        {96, 128, 56, 255},

        {0,0,0,1},
        {96, 128, 56, 255},
        {2,0,0,1},
        {96, 128, 56, 255},
        {1,100,0,1},
        {96, 128, 56, 255}
    };
    glGenBuffers(1, &bladeID);
    glBindBuffer(GL_ARRAY_BUFFER, bladeID);
    glBufferData(
        GL_ARRAY_BUFFER,
        data.size()*sizeof(glm::vec4),
        &data[0],
        GL_STATIC_DRAW
    );
}

void Grass::generatePositions() {
    //radius, amt, position
    amt = 2;
    std::vector<glm::vec2> data;
    // data.reserve(amt);
    for(size_t i =0; i < amt; ++i) {
        float angle = 2 * M_PI * i / amt;
        data.push_back({std::cos(angle), std::sin(angle)});
    }
    glGenBuffers(1, &offsetID);
    glBindBuffer(GL_ARRAY_BUFFER, offsetID);
    glBufferData(
        GL_ARRAY_BUFFER,
        data.size()*sizeof(glm::vec2),
        &data[0],
        GL_STATIC_DRAW
    );
}
void Grass::setModelView(GLuint p) {
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
void Grass::recalculateModel() {
    model = glm::translate(glm::mat4(1.0f), {position.x, 1.0f, position.y});
}

void Grass::setTexture(GLuint p) {
    glUniform1ui(
        glGetUniformLocation(p, "heightMap"),
        heightTextureID
    );
}