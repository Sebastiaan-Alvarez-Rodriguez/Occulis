#ifndef SELF
#define SELF
// https://github.com/hmazhar/moderngl_camera/blob/master/camera.cpp
// https://learnopengl.com/Getting-started/Camera
// http://in2gpu.com/2016/03/14/opengl-fps-camera-quaternion/

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "inputstate.h"
class Self {
public:
    Self(inputstate& i);
    //called before each render, deltatime is in seconds
    void update(int width, int height, double deltatime);
    //render the scene
    void render();

private:
    inputstate& in;
    GLuint programID, heightmapVertexID, squareColorID;
    size_t screen_width = 800, screen_height = 600;
    glm::vec3 cam_pos = {200, 200, 200};
    float cam_theta = M_PI /4*3, cam_phi = 0;
    float cam_speed = 10.0f;
    void cameraInit(glm::vec3 look_pos);
    void cam_update();
};
#endif