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
    void render(GLenum drawMode, GLuint program_id);
    size_t getWidth();
    size_t getHeight();
    float getHeightAt(size_t x, size_t z) const;
private:
    const Camera* cam;

    GLuint dataID;
    std::vector<float> terrain_heightmap;
    size_t triangles, width, height;

    void genHeightmap();
    void setModelView(GLuint p);
};
#endif