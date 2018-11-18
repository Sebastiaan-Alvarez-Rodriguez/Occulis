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

    void init(GLuint program_id_main, const char* viewname_main, GLuint program_id_atmos, const char* viewname_atmos);
    void rotate(rotdir d, float amt);
    void move(movedir d, float amt);
    glm::mat4 getView() const;
private:
    GLuint program_id_main, program_id_atmos;
    std::string viewname_main, viewname_atmos;

    glm::vec3 cam_pos = {200, 100, 200};
    glm::mat4 view = glm::mat4(1.0f);
    float cam_theta = M_PI /4*3, cam_phi = 0;
    float cam_speed = 50.0f;

    void cam_update();
};
#endif