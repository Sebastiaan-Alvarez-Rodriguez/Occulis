#ifndef SELF
#define SELF

#include <GL/glew.h>
#include <GL/glu.h>

#include "Camera.h"
#include "Atmosphere.h"
#include "Terrain.h"
#include "Wind.h"
#include "inputstate.h"

class Self {
public:
    Self(inputstate& i, size_t init_width, size_t init_height, size_t grass_amt);
    //called before each render, deltatime is in seconds
    void update(int width, int height, double deltatime);
    //render the scene
    void render();

private:
    inputstate& in;
    size_t screen_width = 800, screen_height = 600;
    const float cam_speed = 100.0f;

    Camera cam;
    Atmosphere atmosphere;
    Terrain ter;
    Wind wind;

    GLuint program_id_depth, program_id_main;

    GLuint frame_buffer_id, depth_texture_id;
    bool wireframe_toggle = false, grass_toggle = true;

    void setProjections();
    void frameBufferInit();
    void generateTextures();
    void computeLightSpace(GLuint p);
};
#endif