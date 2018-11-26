#ifndef GROUND
#define GROUND

#include <GL/glew.h>
#include <GL/glu.h>
#include <vector>
#include "Image.h"
#include "ImageReader.h"
#include "Camera.h"

//Ideas: Implement grass (with instanced drawArrays call!!!!
    //  https://learnopengl.com/Advanced-OpenGL/Instancing
class Ground {
public:
    Ground(const Camera* cam);
    GLuint genHeightmap();
    void render(GLenum drawMode, GLuint program_id);
    size_t getWidth();
    size_t getHeight();
private:
    const Camera* cam;

    GLuint dataID;

    size_t triangles, width, height;

    void setModelView(GLuint p);
};
#endif