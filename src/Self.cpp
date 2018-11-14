#define _USE_MATH_DEFINES
#include "Self.h"
#include <cmath>
#include "Camera.h"
#include "Terrain.h"
#include "inputstate.h"
#include "shader.hpp"
#include "error.hpp"

struct rgba {
    unsigned char r, g, b, a;
};

Self::Self(inputstate& i): in(i) {
    program_id = LoadShaders("shaders/_shaderV.c", "shaders/_shaderF.c");
    glUseProgram(program_id);
    cam.init(program_id, "view");
    ter.init(program_id);
    sun.init(program_id);
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
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) screen_width / (float) screen_height, 0.1f, 10000.0f);
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
    ter.render();
    sun.render();
    if (errCheck())
        throw std::runtime_error("gl_exception");
}