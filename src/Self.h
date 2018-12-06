#ifndef SELF
#define SELF

#include <GL/glew.h>
#include <GL/glu.h>

#include "Camera.h"
#include "Atmosphere.h"
#include "Terrain.h"
#include "Wind.h"
#include "inputstate.h"

/*
 * Main drawing handler. Sets up all required objects, and provides a lot of
 * required data, such as Projection matrices.
 */
class Self {
public:
    Self(inputstate& i, size_t init_width, size_t init_height, size_t grass_amt);
    // Updates atmosphere and wind, and checks for user button input
    void update(int width, int height, double deltatime);

    void render();

private:
    inputstate& in;
    size_t screen_width = 1920, screen_height = 1080;
    const float cam_speed = 100.0f;

    Camera cam;
    Atmosphere atmosphere;
    Terrain ter;
    Wind wind;

    GLuint program_id_depth, program_id_main;

    GLuint frame_buffer_id, depth_texture_id;
    bool wireframe_toggle = false, grass_toggle = true, suncycle_toggle = true;

    // Sets projection matrices for a few shaders (These projections never change)
    void setProjections();
    // Initialize framebuffer, for creating a depth-texture
    void frameBufferInit();
    // Computes projection times view matrix for depth texture
    void computeLightSpace(GLuint p);
};
#endif