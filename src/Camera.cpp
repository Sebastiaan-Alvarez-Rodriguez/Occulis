#include "Camera.h"
#include "error.hpp"

void Camera::init(GLuint program_id_main, const char* viewname_main, GLuint program_id_atmos, const char* viewname_atmos) {
    this->program_id_main = program_id_main;
    this->program_id_atmos = program_id_atmos;
    this->viewname_main = std::string(viewname_main);
    this->viewname_atmos = std::string(viewname_atmos);
    cam_update();
    errCheck();
}

void Camera::rotate(rotdir d, float amt) {
    switch (d) {
        case rotdir::LEFT: 
            cam_phi -= amt;
            break;
        case rotdir::RIGHT: 
            cam_phi += amt;
            break;
        case rotdir::UP: 
            cam_theta -= amt; 
            break;
        case rotdir::DOWN: 
            cam_theta += amt; 
            break;
    }
    cam_update();
}

void Camera::move(movedir d, float amt) {
    switch (d) {
        case movedir::LEFT: 
            cam_pos.x += std::sin(cam_phi) * amt;
            cam_pos.z -= std::cos(cam_phi) * amt;
            break;
        case movedir::RIGHT: 
            cam_pos.x -= std::sin(cam_phi) * amt;
            cam_pos.z += std::cos(cam_phi) * amt;
            break;
        case movedir::UP: 
            break;
        case movedir::DOWN: 
            break;
        case movedir::FORWARD: 
            cam_pos.x += std::cos(cam_phi) * std::sin(cam_theta) * amt;
            cam_pos.y += std::cos(cam_theta) * amt;
            cam_pos.z += std::sin(cam_phi) * std::sin(cam_theta) * amt;
            break;
        case movedir::BACKWARD: 
            cam_pos.x -= std::cos(cam_phi) * std::sin(cam_theta) * amt;
            cam_pos.y -= std::cos(cam_theta) * amt;
            cam_pos.z -= std::sin(cam_phi) * std::sin(cam_theta) * amt;
            break;
    }
    cam_update();
}

glm::mat4 Camera::getView() const{
    return view;
}

void Camera::cam_update() {
    if(cam_phi < -M_PI) 
        cam_phi = M_PI;
    else if(cam_phi > M_PI) 
        cam_phi = -M_PI;

    if(cam_theta < 0.0001f) 
        cam_theta = 0.0001f;
    else if(cam_theta > M_PI - 0.0001f) 
        cam_theta = M_PI - 0.0001f;

    view = glm::lookAt(
        cam_pos,  // Camera is at (256,300,256), in World Space
        cam_pos + glm::vec3(std::cos(cam_phi)*std::sin(cam_theta), std::cos(cam_theta), std::sin(cam_phi)*std::sin(cam_theta)),
        {0,1,0}  // Head is up (set to 0,-1,0 to look upside-down)
    );
    glUseProgram(program_id_main);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id_main, "view"), 
        1,
        GL_FALSE,
        &view[0][0]
    );
    glUseProgram(program_id_atmos);
    glUniformMatrix4fv(
        glGetUniformLocation(program_id_atmos, "view"), 
        1,
        GL_FALSE,
        &view[0][0]
    );
}