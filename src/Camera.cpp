#include "Camera.h"

void Camera::init(GLuint program_id, const char* viewname) {
    this->program_id = program_id;
    this->viewname = std::string(viewname);
    cam_update();
}

void Camera::cam_update() {
    if(cam_phi < -M_PI) 
        cam_phi = M_PI;
    else if(cam_phi > M_PI) 
        cam_phi = -M_PI;

    if(cam_theta < 0.001f) 
        cam_theta = 0.001f;
    else if(cam_theta > M_PI - 0.001f) 
        cam_theta = M_PI - 0.001f;

    glm::mat4 view = glm::lookAt(
        cam_pos,  // Camera is at (256,300,256), in World Space
        cam_pos + glm::vec3(cos(cam_phi)*sin(cam_theta), cos(cam_theta), sin(cam_phi)*sin(cam_theta) ),
        {0,1,0}  // Head is up (set to 0,-1,0 to look upside-down)
    );
    glUniformMatrix4fv(
        glGetUniformLocation(program_id, viewname.c_str()), 
        1,
        GL_FALSE,
        &view[0][0]
    );
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
            cam_pos.x += sin(cam_phi) * amt;
            cam_pos.z -= cos(cam_phi) * amt;
            break;
        case movedir::RIGHT: 
            cam_pos.x -= sin(cam_phi) * amt;
            cam_pos.z += cos(cam_phi) * amt;
            break;
        case movedir::UP: 
            break;
        case movedir::DOWN: 
            break;
        case movedir::FORWARD: 
            cam_pos.x += cos(cam_phi) * sin(cam_theta) * amt;
            cam_pos.y += cos(cam_theta) * amt;
            cam_pos.z += sin(cam_phi) * sin(cam_theta) * amt;
            break;
        case movedir::BACKWARD: 
            cam_pos.x -= cos(cam_phi) * sin(cam_theta) * amt;
            cam_pos.y -= cos(cam_theta) * amt;
            cam_pos.z -= sin(cam_phi) * sin(cam_theta) * amt;
            break;
    }
    cam_update();
}