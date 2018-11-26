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

GLuint Ground::genHeightmap() {
    size_t size;
    std::vector<float> data = ImageReader::readHeightmap(
        Image("terrain/terrain_heightmap.png"),
        size
    );
    GLuint genID = 0;
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &genID);
    glBindTexture(GL_TEXTURE_2D, genID);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width,height,0,GL_RED,GL_FLOAT,&data[0]);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return genID;
}

void Ground::render(GLenum drawMode, GLuint program_id) {
    setModelView(program_id);

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