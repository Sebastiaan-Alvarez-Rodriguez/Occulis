#ifndef SUN
#define SUN

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

/*
 * Class representing a sun. 
 * (As Preetham model already simulates sun, rendering this is not required/recommended)
 */
class Sun {
public:
    Sun(const Camera* cam);

    void render(GLenum drawMode, GLuint program_id);

    // Update sun location
    void update(double deltatime);

    //basic getters
    glm::vec3 getDirection() const;
    glm::vec3 getPosition() const;
    glm::vec3 getLightColor() const;
private:
    GLuint sunVertexID;
    
    glm::vec3 position;
    glm::vec3 lightColor;
    glm::mat4 model;
    
    // Sun positional, rotational information
    float radius = 200.0f;
    const size_t rings = 48, sectors = 48;

    float angle = M_PI/4 * 3;
    float orbit_radius = 3000.0f;

    const Camera* cam;

    // Update methods for model and lightcolor
    void recalculateModel();
    void recalculateLightColor();

    // Basic setters for shaders
    void setModelView(GLuint p);
    void setSunPosition(GLuint p);
    void setSunlightColor(GLuint p);

    // Create big yellow orb
    void createSun(size_t r, size_t s);
};
#endif