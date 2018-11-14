#define _USE_MATH_DEFINES
#include "Self.h"
#include <cmath>
#include "Camera.h"
#include "inputstate.h"
#include "shader.hpp"
#include "picopng.hpp"
#include "error.hpp"

struct rgba {
    unsigned char r, g, b, a;
};

Self::Self(inputstate& i): in(i) {
    program_id = LoadShaders("_shaderV.c", "_shaderF.c");
    glUseProgram(program_id);
    cam.init(program_id, "view");
    
    size_t w, h;
    std::vector<unsigned char> buffer, heightmap;
    loadFile(buffer, "terrain_heightmap.png");
    if (decodePNG(heightmap, w, h, &buffer[0], buffer.size(), true)!=0)
        throw std::runtime_error("picopng exception");
    std::vector<float> hallo(512*512*4*6, 1);

    for (size_t z = 0; z < 512; ++z)
        for (size_t x = 0; x < 512; ++x) {
            hallo[(z * 512 + x) * 24 + 0] = x;
            hallo[(z * 512 + x) * 24 + 1] = (float) heightmap[(z * 512 + x)*4];
            hallo[(z * 512 + x) * 24 + 2] = z;
            // hallo[(z * 512 + x) * 12 + 3] = 1;
            hallo[(z * 512 + x) * 24 + 4] = x+1;
            hallo[(z * 512 + x) * 24 + 5] = (float) heightmap[(z * 512 + (x+1))*4];
            hallo[(z * 512 + x) * 24 + 6] = z;
            // hallo[(z * 512 + x) * 12 + 7] = 1;
            hallo[(z * 512 + x) * 24 + 8] = x+1;
            hallo[(z * 512 + x) * 24 + 9] = (float) heightmap[((z+1) * 512 + (x+1))*4];
            hallo[(z * 512 + x) * 24 + 10] = z+1;
            // hallo[(z * 512 + x) * 12 + 11] = 1;
            hallo[(z * 512 + x) * 24 + 12] = x+1;
            hallo[(z * 512 + x) * 24+ 13] = (float) heightmap[((z+1) * 512 + (x+1))*4];
            hallo[(z * 512 + x) * 24 + 14] = z+1;
            // hallo[(z * 512 + x) * 12 + 15] = 1;
            hallo[(z * 512 + x) * 24 + 16] = x;
            hallo[(z * 512 + x) * 24 + 17] = (float) heightmap[((z+1) * 512 + x)*4];
            hallo[(z * 512 + x) * 24 + 18] = z+1;
            // hallo[(z * 512 + x) * 12 + 19] = 1;
            hallo[(z * 512 + x) * 24 + 20] = x;
            hallo[(z * 512 + x) * 24 + 21] = (float) heightmap[(z * 512 + x)*4];
            hallo[(z * 512 + x) * 24 + 22] = z;
            // hallo[(z * 512 + x) * 12 + 23] = 1;
        }

    glGenBuffers(1, &heightmapVertexID);
    glBindBuffer(GL_ARRAY_BUFFER, heightmapVertexID);
    glBufferData(
        GL_ARRAY_BUFFER,
        hallo.size()*sizeof(float),
        &hallo[0],
        GL_STATIC_DRAW
    );
    cameraInit();
    if (errCheck())
        throw std::runtime_error("gl_exception");
}
 
void Self::cameraInit() {
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id, "model"), 
        1,
        GL_FALSE,
        &Model[0][0]
    );

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) screen_width / (float) screen_height, 0.1f, 1000.0f);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id, "projection"),
        1,
        GL_FALSE,
        &Projection[0][0]
    );
}

void Self::update(int width, int height, double deltatime) {
    screen_width = width;
    screen_height = height;

    if (in.down[SDLK_j])
       cam.rotate(Camera::rotdir::LEFT, M_PI/2 * deltatime);

    if (in.down[SDLK_l])
        cam.rotate(Camera::rotdir::RIGHT, M_PI/2 * deltatime);

    if (in.down[SDLK_i])
        cam.rotate(Camera::rotdir::UP, M_PI/2 * deltatime);

    if (in.down[SDLK_k])
        cam.rotate(Camera::rotdir::DOWN, M_PI/2 * deltatime);

    if (in.down[SDLK_t])
        cam.move(Camera::movedir::FORWARD, cam_speed*deltatime);
    
    if (in.down[SDLK_g]){
        cam.move(Camera::movedir::BACKWARD, cam_speed*deltatime);
    }
    if (in.down[SDLK_f]){
        cam.move(Camera::movedir::LEFT, cam_speed*deltatime);
    }
    if (in.down[SDLK_h]){
        cam.move(Camera::movedir::RIGHT, cam_speed*deltatime);
    }
}

void Self::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, heightmapVertexID);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    // glBindBuffer(GL_ARRAY_BUFFER, squareColorID);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 512*512*6);

    // glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    if (errCheck())
        throw std::runtime_error("gl_exception");
}