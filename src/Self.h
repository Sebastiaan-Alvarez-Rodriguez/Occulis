#ifndef SELF
#define SELF

#include <GL/glew.h>
#include <GL/glu.h>

#include "Camera.h"
#include "Atmosphere.h"
#include "Terrain.h"
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
    Atmosphere atmosphere;
    Terrain ter;

    GLuint program_id_main, program_id_atmos, program_id_depth;

    GLuint frame_buffer_id, depth_texture_id;
    size_t screen_width = 800, screen_height = 600;
    bool wireframe_toggle = false;

    void cameraInit();
    void depthStuffInit();  
    void lightCamSetup();
};
#endif