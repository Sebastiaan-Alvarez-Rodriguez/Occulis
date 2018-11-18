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
    program_id_depth= LoadShaders("shaders/depth_vertex.glsl", "shaders/depth_frag.glsl");

    cam.init(program_id_main, program_id_atmos);

    atmosphere.init(program_id_atmos, program_id_main);

    ter.init(program_id_main);
    cameraInit();
    depthStuffInit();
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

    glUseProgram(program_id_depth);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id_depth, "cam_projection"),
        1,
        GL_FALSE,
        &Projection[0][0]
    );
    glUseProgram(program_id_main);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id_main, "cam_projection"),
        1,
        GL_FALSE,
        &Projection[0][0]
    );
}

void Self::depthStuffInit() {
    frame_buffer_id = 0;
    glGenFramebuffers(1, &frame_buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);

    depth_texture_id = 0;
    glGenTextures(1, &depth_texture_id);
    glBindTexture(GL_TEXTURE_2D, depth_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture_id, 0);

    glDrawBuffer(GL_NONE); //No color buffer is drawn to.

    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Framebuffer is troubled");
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
    
    if (in.down[SDLK_g])
        cam.move(Camera::movedir::BACKWARD, cam_speed*deltatime);

    if (in.down[SDLK_f])
        cam.move(Camera::movedir::LEFT, cam_speed*deltatime);

    if (in.down[SDLK_h])
        cam.move(Camera::movedir::RIGHT, cam_speed*deltatime);

    if (in.down[SDLK_a])
        atmosphere.update(deltatime);
    if (in.down[SDLK_z])
        atmosphere.update(-deltatime);
    if (in.press[SDLK_x])
        atmosphere.printSunDir();
}

void Self::lightCamSetup() {
    //guaranteed program_id_depth in use here
    glm::vec3 sunLoc = atmosphere.getSunPosition();
    glm::mat4 camView = glm::lookAt(
        sunLoc,            //camera is at sun location
        {256,0,256},       //and looks at mid-point of image
        {0,1,0}           // Head is up
    );
    glm::mat4 camModel = glm::translate(glm::mat4(1.0f), sunLoc);
    // glUseProgram(program_id_depth);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id_depth, "cam_view"),
        1,
        GL_FALSE,
        &camView[0][0]
    );
    glUniformMatrix4fv(
        glGetUniformLocation(program_id_depth, "cam_model"),
        1,
        GL_FALSE,
        &camModel[0][0]
    );
    glUseProgram(program_id_main);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id_main, "cam_view"),
        1,
        GL_FALSE,
        &camView[0][0]
    );
    glUniformMatrix4fv(
        glGetUniformLocation(program_id_main, "cam_model"),
        1,
        GL_FALSE,
        &camModel[0][0]
    );
}

void Self::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1,0,0,0);

    //1e pass
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id); //bind depth buffer
    glUseProgram(program_id_depth);
    lightCamSetup();                                    //setup camera
    ter.render(GL_TRIANGLES);                           //render terrain

    //2e pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);               //bind default buffer
    // glActiveTexture(depth_texture_id);                  //zet texture als actief

    GLenum drawMode = wireframe_toggle ? GL_LINES : GL_TRIANGLES;

    atmosphere.render(drawMode);
    ter.render(drawMode);

    if (errCheck())
        throw std::runtime_error("gl_exception");
}