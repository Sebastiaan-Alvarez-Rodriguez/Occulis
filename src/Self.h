#ifndef SELF
#define SELF
// https://github.com/hmazhar/moderngl_camera/blob/master/camera.cpp
// https://learnopengl.com/Getting-started/Camera
// http://in2gpu.com/2016/03/14/opengl-fps-camera-quaternion/

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "inputstate.h"
class Self {
public:
    Self(inputstate& i);
    //called before each render, deltatime is in seconds
    void update(int width, int height, double deltatime);
    //render the scene
    void render();

private:
    constexpr static float cam_speed = 50.0f;
    inputstate& in;
    Camera cam;
    GLuint program_id, heightmapVertexID, squareColorID;
    size_t screen_width = 800, screen_height = 600;
    void cameraInit();
};
#endif