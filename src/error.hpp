#ifndef ERROR
#define ERROR

#include <iostream>
#include <stdexcept>

static inline void errCheck() {
    auto err = glGetError();
    bool errorOccured = false;
    while (err != 0) {
        errorOccured = true;
        std::cerr<<'('<<err<<")- "<< gluErrorString(err) << std::endl;
        err = glGetError();
    }
    if (errorOccured)
        throw std::runtime_error("gl_exception");
}
#endif