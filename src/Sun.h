#ifndef SUN
#define SUN

#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Sun {
public:
    void init(GLuint program_id);
    void createSphere(size_t rings, size_t sectors);
    void render();
private:
    GLuint sunVertexID, sunNormalID;
    glm::vec3 position;
    float radius = 200.0f;
    
};
#endif