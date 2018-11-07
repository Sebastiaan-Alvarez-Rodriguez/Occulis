#ifndef SELF
#define SELF
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
    GLuint programID, squareVertexID, squareColorID;
    
};
#endif