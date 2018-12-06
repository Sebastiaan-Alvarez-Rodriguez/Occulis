#ifndef WIND
#define WIND

#include <glm/glm.hpp>

#include <array>
#include <chrono>

/*
 * Class to simulate wind
 */
class Wind {
public:
    // Struct to store information about wind. To be passed as main information struct
    struct WData {
        glm::vec4 direction = {1, 0, 0, 0};
        float strength = 0.4f;
        float baseAngle = 0.0f;
    };

    // Update wind baseangle
    void update(double deltatime);

    // Update strength
    void updateStrength(float t);

    // Basic getter
    Wind::WData getWData() const;

private:
    WData data;
};
#endif