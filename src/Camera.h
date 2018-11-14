#ifndef CAMERA
#define CAMERA

#include <string>
#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    enum class rotdir {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    enum class movedir {
        LEFT,
        RIGHT,
        UP,
        DOWN,
        FORWARD,
        BACKWARD
    };

    void init(GLuint program_id, const char* viewname);
    void rotate(rotdir d, float amt);
    void move(movedir d, float amt);
private:
    GLuint program_id;
    std::string viewname;

    glm::vec3 cam_pos = {200, 200, 200};
    float cam_theta = M_PI /4*3, cam_phi = 0;
    float cam_speed = 50.0f;

    void cam_update();
};
#endif