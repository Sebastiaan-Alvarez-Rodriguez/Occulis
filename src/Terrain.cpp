#include "Terrain.h"

#include "util.h"
#include "error.hpp"

Terrain::Terrain(const Camera* cam): cam(cam) {    
    size_t triangles;
    std::vector<Data> data = ImageReader::read(
        Image("terrain/terrain_heightmap.png"),
        Image("terrain/color_flat.png"),
        triangles
    );
    //returns 1572864 (512*512*6) elements
    this->triangles = triangles;

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

void Terrain::render(GLenum drawMode, GLuint program_id) {
    setModelView(program_id);

    //enable vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, heightmapVertexID);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Data), 0);
    //enable normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Data), (void*)(sizeof(glm::vec4)));
    //enable colors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Data), (void*)(2* sizeof(glm::vec4)));

    glDrawArrays(drawMode, 0, triangles);

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void Terrain::setModelView(GLuint p) {
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