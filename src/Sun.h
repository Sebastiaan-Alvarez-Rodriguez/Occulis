#ifndef SUN
#define SUN

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

class Sun {
public:
    void init(GLuint program_id, const Camera* cam);
    void render(GLenum drawMode);
    void update(double deltatime);
    glm::vec3 getDirection() const;
    glm::vec3 getPosition() const;
private:
    GLuint program_id;
    GLuint sunVertexID;
    
    glm::vec3 position;
    glm::mat4 model;
    
    float radius = 200.0f;
    const size_t rings = 48, sectors = 48;

    float angle = 0;
    float orbit_radius = 3000.0f;

    const Camera* cam;
    void setModel();
    void recalculateModel();
    void createSun(size_t r, size_t s);
};
#endif