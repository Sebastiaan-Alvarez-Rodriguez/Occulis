#include "Wind.h"

bool almostEqual(float a, float b) {
    return (a > b-0.01f && a < b+0.01f);
}


void Wind::update(double deltatime) {
    data.baseAngle += deltatime;
    if (data.baseAngle > 2*M_PI)
        data.baseAngle = 0;
}

void Wind::updateStrength(float t) {
    data.strength += t;
}

Wind::WData Wind::getWData() const {
    return data;
}