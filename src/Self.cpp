#include "Self.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "error.hpp"

struct rgba {
    unsigned char r, g, b, a;
};

Self::Self(inputstate& i): in(i) {
    program_id_main = LoadShaders("shaders/main_vertex.glsl", "shaders/main_frag.glsl");
    program_id_atmos= LoadShaders("shaders/main_vertex.glsl", "shaders/preetham_frag.glsl");

    cam.init(program_id_main, "view", program_id_atmos, "view");

    atmosphere.init(program_id_atmos, program_id_main, &cam);

    ter.init(program_id_main);
    cameraInit();
    if (errCheck())
        throw std::runtime_error("gl_exception");
}
 
void Self::cameraInit() {
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glUseProgram(program_id_main);
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) screen_width / (float) screen_height, 0.1f, 100000.0f);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id_main, "projection"),
        1,
        GL_FALSE,
        &Projection[0][0]
    );
    glUseProgram(program_id_atmos);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id_atmos, "projection"),
        1,
        GL_FALSE,
        &Projection[0][0]
    );
}

void Self::update(int width, int height, double deltatime) {
    screen_width = width;
    screen_height = height;

    if (in.press[SDLK_w])
        wireframe_toggle = !wireframe_toggle;

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
    
    atmosphere.update(deltatime);
}

void Self::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,1,0);
    GLenum drawMode = wireframe_toggle ? GL_LINES : GL_TRIANGLES;

    atmosphere.render(drawMode);

    ter.render(drawMode);

    if (errCheck())
        throw std::runtime_error("gl_exception");
}