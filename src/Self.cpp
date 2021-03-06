#include "Self.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "error.hpp"

Self::Self(inputstate& i, size_t init_width, size_t init_height, size_t grass_amt): in(i), screen_width(init_width), screen_height(init_height), atmosphere(&cam), ter(&cam, &wind, atmosphere.getSun()) {
    program_id_depth= LoadShaders("shaders/depth_vertex.glsl", "shaders/depth_frag.glsl");
    program_id_main = LoadShaders("shaders/main_vertex.glsl", "shaders/main_frag.glsl");
    glDepthFunc(GL_LESS);
    frameBufferInit();
    setProjections();
    ter.addGrass({255, 255}, 255, grass_amt);
    errCheck();
}
 
void Self::setProjections() {
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) screen_width / (float) screen_height, 0.1f, 100000.0f);
    glUseProgram(program_id_main);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id_main, "projection"),
        1,
        GL_FALSE,
        &Projection[0][0]
    );
    atmosphere.setProjection(Projection);
    ter.setProjection(Projection);
}

void Self::frameBufferInit() {
    const size_t SHADOW_WIDTH = screen_width, SHADOW_HEIGHT = screen_height;
    frame_buffer_id = 0;
    glGenFramebuffers(1, &frame_buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);

    depth_texture_id = 0;
    glGenTextures(1, &depth_texture_id);
    glBindTexture(GL_TEXTURE_2D, depth_texture_id);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,SHADOW_WIDTH,SHADOW_HEIGHT,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 

    glBindFramebuffer(GL_FRAMEBUFFER, depth_texture_id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture_id, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Framebuffer is troubled");
}


void Self::update(int width, int height, double deltatime) {
    screen_width = width;
    screen_height = height;

    if (in.press[SDLK_t])
        wireframe_toggle = !wireframe_toggle;

    if (in.down[SDLK_j])
       cam.rotate(Camera::rotdir::LEFT, M_PI/2 * deltatime);
    if (in.down[SDLK_l])
        cam.rotate(Camera::rotdir::RIGHT, M_PI/2 * deltatime);
    if (in.down[SDLK_i])
        cam.rotate(Camera::rotdir::UP, M_PI/2 * deltatime);
    if (in.down[SDLK_k])
        cam.rotate(Camera::rotdir::DOWN, M_PI/2 * deltatime);

    if (in.down[SDLK_w])
        cam.move(Camera::movedir::FORWARD, cam_speed*deltatime);
    if (in.down[SDLK_s])
        cam.move(Camera::movedir::BACKWARD, cam_speed*deltatime);
    if (in.down[SDLK_a])
        cam.move(Camera::movedir::LEFT, cam_speed*deltatime);
    if (in.down[SDLK_d])
        cam.move(Camera::movedir::RIGHT, cam_speed*deltatime);
    if (in.down[SDLK_r])
        cam.move(Camera::movedir::UP, cam_speed*deltatime);
    if (in.down[SDLK_f])
        cam.move(Camera::movedir::DOWN, cam_speed*deltatime);
    
    if (in.press[SDLK_g])
        grass_toggle = !grass_toggle;

    if (in.down[SDLK_y])
        atmosphere.update(deltatime);
    if (in.down[SDLK_h])
        atmosphere.update(-deltatime);

    if (in.down[SDLK_o])
        wind.updateStrength(deltatime*4);
    if (in.down[SDLK_p])
        wind.updateStrength(-deltatime*4);

    if (in.press[SDLK_z])
        suncycle_toggle = !suncycle_toggle;

    if (suncycle_toggle)
        atmosphere.update(deltatime/8);
    wind.update(deltatime);
}

void Self::computeLightSpace(GLuint p) {
    glm::mat4 View = glm::lookAt(
        atmosphere.getSunPosition(),            //camera is at sun position
        {ter.getWidth()/2,0,ter.getHeight()/2}, //and looks at middle of terrain
        {0,1,0}                                 // up vector is up
    );
    glm::mat4 Projection = glm::ortho(-(float)ter.getWidth()/2, (float)ter.getWidth()/2, -(float)ter.getHeight()/2, (float)ter.getHeight()/2, 1.0f, 10000.0f);

    glm::mat4 lightSpaceMatrix = Projection * View;

    glUniformMatrix4fv(
        glGetUniformLocation(p, "lightSpaceMatrix"),
        1,
        GL_FALSE,
        &lightSpaceMatrix[0][0]
    );
    ter.setLightSpaceMatrix(lightSpaceMatrix);
    glUseProgram(p);
}

void Self::render() {
    errCheck();
    GLenum drawMode = wireframe_toggle ? GL_LINES : GL_TRIANGLES;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1,1,1,1);

    //first pass (draw quad to find depth)
    glUseProgram(program_id_depth);
    //bind texture, to make output go to texture instead of screen
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);
    glActiveTexture(GL_TEXTURE0);
    glClear(GL_DEPTH_BUFFER_BIT);

    //compute lightspace matrix (camera at sun position)
    computeLightSpace(program_id_depth);
    //render the terrain
    
    ter.render(GL_TRIANGLES, program_id_depth);

    //second pass
    glUseProgram(program_id_main);
    //bind buffer '0' -> draw to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    atmosphere.render(drawMode, program_id_main);

    glUseProgram(program_id_main);
    computeLightSpace(program_id_main);
    ter.render(drawMode, program_id_main);
    if (grass_toggle)
        ter.renderGrass(drawMode);
    errCheck();
}