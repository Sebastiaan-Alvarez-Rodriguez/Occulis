#include "Wind.h"

bool almostEqual(float a, float b) {
    return (a > b-0.01f && a < b+0.01f);
}


void Wind::update(double deltatime) {
    data.baseAngle += deltatime;
    if (data.baseAngle > 2*M_PI)
        data.baseAngle = 0;
    updateDirection(deltatime/16);
}

void Wind::updateDirection(double deltatime) {
    if (!almostEqual(data.direction.x, futureDirection.x)) {
        if (data.direction.x < futureDirection.x)
            data.direction.x += deltatime;
        else
            data.direction.x -= deltatime;
    }
 
    if (!almostEqual(data.direction.z, futureDirection.z)) {
        if (data.direction.z < futureDirection.z)
            data.direction.z += deltatime;
        else
            data.direction.z -= deltatime;
    }
}

void Wind::updateStrength(float t) {
    data.strength += t;
}

void Wind::changeDirection(const glm::vec4& dir) {
    futureDirection = glm::normalize(dir);
}

Wind::WData Wind::getWData() const {
    return data;
}

glm::mat4 Wind::rotation(float ang) {
       glm::mat4 ret(1.0f);
        ret[0][0] = std::cos(ang);
        ret[0][1] = -std::sin(ang);
        ret[1][0] = std::sin(ang);
        ret[1][1] = std::cos(ang);
        return ret;
}