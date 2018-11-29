#include "Ground.h"

#include <glm/glm.hpp>
#include "util.h"
#include "shader.hpp"
#include "error.hpp"

Ground::Ground(const Camera* cam): cam(cam) {    
    std::vector<Data> data = ImageReader::read(
        Image("terrain/terrain_heightmap.png"),
        Image("terrain/color_flat.png"),
        triangles,
        width,
        height
    );
    genHeightmap();
    glGenBuffers(1, &dataID);
    glBindBuffer(GL_ARRAY_BUFFER, dataID);
    glBufferData(
        GL_ARRAY_BUFFER,
        data.size()*sizeof(Data),
        &data[0],
        GL_STATIC_DRAW
    );
    errCheck();
}

void Ground::render(GLenum drawMode, GLuint program_id) {
    setModelView(program_id);
    errCheck();
    //enable vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, dataID);
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

size_t Ground::getWidth() {
    return width;
}

size_t Ground::getHeight() {
    return height;
}

float Ground::getHeightAt(size_t x, size_t z) const {
    return terrain_heightmap[z*(width-1) + x];
}

void Ground::genHeightmap() {
    size_t size;
    terrain_heightmap = ImageReader::readHeightmap(
        Image("terrain/terrain_heightmap.png"),
        size
    );
}

void Ground::setModelView(GLuint p) {
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