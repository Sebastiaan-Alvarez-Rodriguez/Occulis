#ifndef TERRAIN
#define TERRAIN

#include <utility>

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Image.h"
#include "ImageReader.h"

#include "Camera.h"
//Ideas: Implement grass (with instanced drawArrays call!!!!
    //  https://learnopengl.com/Advanced-OpenGL/Instancing
class Terrain {
public:
    Terrain(const Camera* cam);
    void render(GLenum drawMode, GLuint program_id);
private:
    const Camera* cam;
    GLuint heightmapVertexID;

    size_t triangles;
    void setModelView(GLuint p);
};
#endif