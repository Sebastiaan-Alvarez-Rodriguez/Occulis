#ifndef WIND
#define WIND

#include <glm/glm.hpp>

#include <array>
#include <chrono>

class Wind {
public:
    struct WData {
        glm::vec4 direction = {1, 0, 0, 0};
        float strength = 0.4f;
        float baseAngle = 0.0f;
    };

    void update(double deltatime);
    void updateDirection(double deltatime);
    void updateStrength(float t);
    void changeDirection(const glm::vec4& dir);
    Wind::WData getWData() const;

private:
    WData data;
    glm::vec4 futureDirection = {1, 0, 0, 0};

    glm::mat4 rotation(float ang);
};
#endif