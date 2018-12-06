#ifndef CAMERA
#define CAMERA

#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/* 
 * Camera controller class. The View matrix is created and updated here
 */
class Camera {
public:
    // enum classes to represent rotation/movement directions
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

    // Rotate and move directions
    void rotate(rotdir d, float amt);
    void move(movedir d, float amt);

    glm::mat4 getView() const;
private:
    // Cam position and angles
    glm::vec3 cam_pos = {240, 120, 240};
    float cam_theta = M_PI /4*3, cam_phi = 0;

    // View matrix this class is controlling/updating
    glm::mat4 view;

    // Function that updates View matrix, according to current globals
    void cam_update();
};
#endif