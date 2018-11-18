#include "Terrain.h"

#include "util.h"
#include "error.hpp"

void Terrain::init(GLuint program_id) {
    this->program_id = program_id;
    glUseProgram(program_id);
    
    std::vector<Data> data = ImageReader::read(
        Image("terrain/terrain_heightmap.png"),
        Image("terrain/color_flat.png")
    );
    //returns 1572864 (512*512*6) elements

    glGenBuffers(1, &heightmapVertexID);
    glBindBuffer(GL_ARRAY_BUFFER, heightmapVertexID);
    glBufferData(
        GL_ARRAY_BUFFER,
        data.size()*sizeof(Data),
        &data[0],
        GL_STATIC_DRAW
    );
    errCheck();
}

void Terrain::setModel() {
    glm::mat4 Model = glm::mat4(1.0f);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id, "model"), 
        1,
        GL_FALSE,
        &Model[0][0]
    );
}

void Terrain::render(GLenum drawMode) {
    setModel();
    glBindBuffer(GL_ARRAY_BUFFER, heightmapVertexID);
    //enable vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Data), 0);
    //enable normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Data), (void*)(sizeof(glm::vec4)));
    //enable colors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Data), (void*)(2* sizeof(glm::vec4)));

    glDrawArrays(drawMode, 0, 512*512*6);

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}