#include "Self.h"
#include "shader.hpp"
#include "picopng.hpp"
#include "error.hpp"

Self::Self(inputstate& i) : in(i) {
    programID = LoadShaders("_shaderV.c", "_shaderF.c");
    glUseProgram(programID);

    size_t w, h;
    std::vector<unsigned char> buffer, heightmap;
    loadFile(buffer, "terrain_heightmap.png");
    if (decodePNG(heightmap, w, h, &buffer[0], buffer.size(), false)!=0)
        throw std::runtime_error("picopng exception");
    std::vector<glm::vec3> positions(512 * 512 * 6);
    size_t x, z;
    for (z = 0; z < 512; z++)
        for (x = 0; x < 512; x++) {
            positions[(z * 512 + x)*6+0] = {x, (float)heightmap[(z * 512 + x)], z};
            positions[(z * 512 + x)*6+1] = {x+1, (float)heightmap[(z * 512 + x+1)], z};
            positions[(z * 512 + x)*6+2] = {x+1, (float)heightmap[((z+1) * 512 + x+1)], z+1};
            positions[(z * 512 + x)*6+3] = {x+1, (float)heightmap[((z+1) * 512 + x+1)], z+1};
            positions[(z * 512 + x)*6+4] = {x, (float)heightmap[((z+1) * 512 + x)], z+1};
            positions[(z * 512 + x)*6+5] = {x, (float)heightmap[(z * 512 + x)], z};
        }

    glGenBuffers(1, &heightmapVertexID);
    //then bind it, so that the next operation is performed on this object
    glBindBuffer(GL_ARRAY_BUFFER, heightmapVertexID);
    //send the data to the GPU
    glBufferData(
        GL_ARRAY_BUFFER,
        512*512*6*3*sizeof(float),        //size of data in bytes
        &positions[0],                    //pointer to start of data
        GL_STATIC_DRAW                    //expected method of updating the data
    );

    cameraInit(glm::vec3(256,300,256), glm::vec3(512,100,512), glm::vec3(0,1,0));
    if (errCheck())
        throw std::runtime_error("gl_exception");
}
 
void Self::cameraInit(glm::vec3 cam_pos, glm::vec3 look_pos, glm::vec3 head_rot) {
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) screen_width / (float) screen_height, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View = glm::lookAt(
        cam_pos,  // Camera is at (256,300,256), in World Space
        look_pos, // and looks at the origin
        head_rot  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);

    glUniformMatrix4fv(
        glGetUniformLocation(programID, "model"), 
        1,
        GL_FALSE,
        &Model[0][0]
    );
    glUniformMatrix4fv(
        glGetUniformLocation(programID, "view"),
        1,
        GL_FALSE,
        &View[0][0]
    );

    glUniformMatrix4fv(
        glGetUniformLocation(programID, "projection"),
        1,
        GL_FALSE,
        &Projection[0][0]
    );
}

void Self::update(int width, int height, double deltatime) {
    screen_width = width;
    screen_height = height;
}

void Self::render() {
    glBindBuffer(GL_ARRAY_BUFFER, heightmapVertexID);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // glBindBuffer(GL_ARRAY_BUFFER, squareColorID);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 512*512*6);

    // glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    if (errCheck())
        throw std::runtime_error("gl_exception");
}