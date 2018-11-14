#ifndef SELF
#define SELF

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Terrain.h"
#include "Sun.h"
#include "inputstate.h"

class Self {
public:
    Self(inputstate& i);
    //called before each render, deltatime is in seconds
    void update(int width, int height, double deltatime);
    //render the scene
    void render();

private:
    constexpr static float cam_speed = 100.0f;
    inputstate& in;
    Camera cam;
    Terrain ter;
    Sun sun;
    GLuint program_id, heightmapVertexID, squareColorID;
    size_t screen_width = 800, screen_height = 600;
    void cameraInit();
};
#endif