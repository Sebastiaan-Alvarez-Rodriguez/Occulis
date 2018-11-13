#define _USE_MATH_DEFINES
#include "Self.h"
#include <cmath>
#include "inputstate.h"
#include "shader.hpp"
#include "picopng.hpp"
#include "error.hpp"

struct rgba {
    unsigned char r, g, b, a;
};

Self::Self(inputstate& i) : in(i) {
    programID = LoadShaders("_shaderV.c", "_shaderF.c");
    glUseProgram(programID);

    size_t w, h;
    std::vector<unsigned char> buffer, heightmap;
    loadFile(buffer, "terrain_heightmap.png");
    if (decodePNG(heightmap, w, h, &buffer[0], buffer.size(), true)!=0)
        throw std::runtime_error("picopng exception");
    std::vector<float> hallo(512*512*4, 1);

    for (size_t z = 0; z < 512; ++z)
        for (size_t x = 0; x < 512; ++x) {
            hallo[(z * 512 + x) * 4 + 0] = x;
            hallo[(z * 512 + x) * 4 + 1] = (float) heightmap[(z * 512 + x)*4];
            hallo[(z * 512 + x) * 4 + 2] = z;
            // hallo[(z * 512 + x) * 4 + 0] = 1;
        }

    glGenBuffers(1, &heightmapVertexID);
    glBindBuffer(GL_ARRAY_BUFFER, heightmapVertexID);
    glBufferData(
        GL_ARRAY_BUFFER,
        hallo.size()*sizeof(float),
        &hallo[0],
        GL_STATIC_DRAW
    );
    cameraInit(glm::vec3(256, 100, 256));
    if (errCheck())
        throw std::runtime_error("gl_exception");
}
 
void Self::cameraInit(glm::vec3 look_pos) {
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) screen_width / (float) screen_height, 0.1f, 1000.0f);

    cam_update();

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);

    glUniformMatrix4fv(
        glGetUniformLocation(programID, "model"), 
        1,
        GL_FALSE,
        &Model[0][0]
    );

    glUniformMatrix4fv(
        glGetUniformLocation(programID, "projection"),
        1,
        GL_FALSE,
        &Projection[0][0]
    );
}


void Self::cam_update() {
    if(cam_theta < 0.001f) 
        cam_theta = 0.001f;
    else if(cam_theta > M_PI - 0.001f) 
        cam_theta = M_PI - 0.001f;

    glm::mat4 View = glm::lookAt(
        cam_pos,  // Camera is at (256,300,256), in World Space
        cam_pos + glm::vec3(cos(cam_phi)*sin(cam_theta), cos(cam_theta), sin(cam_phi)*sin(cam_theta) ),
        {0,1,0}  // Head is up (set to 0,-1,0 to look upside-down)
        );
        glUniformMatrix4fv(
            glGetUniformLocation(programID, "view"),
            1,
            GL_FALSE,
            &View[0][0]
        );
}

void Self::update(int width, int height, double deltatime) {
    screen_width = width;
    screen_height = height;

    if (in.down[SDLK_j]) {
        cam_phi -= M_PI * deltatime / 2;
        cam_update();
    }
    if (in.down[SDLK_l])  {
        cam_phi += M_PI * deltatime / 2;
        cam_update();
    }
    if (in.down[SDLK_i]) {
        cam_theta += M_PI * deltatime / 2;
        cam_update();
    }
    if (in.down[SDLK_k]) {
        cam_theta -= M_PI * deltatime / 2;
        cam_update();
    }
    if (in.down[SDLK_t]){
        cam_pos.x -= cam_speed * sin(cam_phi) * deltatime;
        cam_pos.y += cam_speed * cos(cam_phi) * deltatime;
        cam_update();
    }
    if (in.down[SDLK_g]){
        cam_pos.x += cam_speed * sin(cam_phi) * deltatime;
        cam_pos.y -= cam_speed * cos(cam_phi) * deltatime;
        cam_update();
    }
    if (in.down[SDLK_f]){
            cam_pos.x += cam_speed * cos(cam_phi) * sin(cam_theta) * deltatime;
        cam_pos.y += cam_speed * sin(cam_phi) * sin(cam_theta) * deltatime;
        cam_pos.z += cam_speed * cos(cam_theta) * deltatime;
        cam_update();
    }
    if (in.down[SDLK_h]){
        cam_pos.x -= cam_speed * cos(cam_phi) * sin(cam_theta) * deltatime;
        cam_pos.y -= cam_speed * sin(cam_phi) * sin(cam_theta) * deltatime;
        cam_pos.z -= cam_speed * cos(cam_theta) * deltatime;
        cam_update();
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

    glDrawArrays(GL_POINTS, 0, 512*512);

    // glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    if (errCheck())
        throw std::runtime_error("gl_exception");
}