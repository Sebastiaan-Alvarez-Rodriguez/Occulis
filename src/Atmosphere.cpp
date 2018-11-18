#include "Atmosphere.h"

#include "shader.hpp"
#include "error.hpp"

void Atmosphere::init(GLuint skydome_pID, GLuint sun_pID) {
    this->skydome_pID = skydome_pID;
    this->sun_pID = sun_pID;
    sun.init(sun_pID);
    dome.init(skydome_pID, &sun);
    errCheck();
}

void Atmosphere::render(GLenum drawMode) {
    glUseProgram(skydome_pID);
    dome.render(drawMode);
    
    glUseProgram(sun_pID);
    // sun.render(drawMode);
}

void Atmosphere::update(double deltatime) {
    sun.update(deltatime);
}
#include <iostream>
void Atmosphere::printSunDir() {
    auto position = sun.getDirection();
    std::cerr << "Dir x: "<<position.x<< "   y:"<<position.y<<"   z:"<<position.z << std::endl;
}