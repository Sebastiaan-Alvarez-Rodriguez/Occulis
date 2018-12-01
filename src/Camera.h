#ifndef CAMERA
#define CAMERA

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

    Camera();
    void rotate(rotdir d, float amt);
    void move(movedir d, float amt);
    glm::mat4 getView() const;
private:
    glm::vec3 cam_pos = {240, 120, 240};
    glm::mat4 view;
    float cam_theta = M_PI /4*3, cam_phi = 0;
    float cam_speed = 50.0f;

    void cam_update();
};
#endif