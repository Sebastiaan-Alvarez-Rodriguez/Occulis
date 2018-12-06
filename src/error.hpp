#ifndef ERROR
#define ERROR

#include <iostream>
#include <stdexcept>

/*
 * Simple function to check whether any openGL errors occur
 */
static inline void errCheck() {
    auto err = glGetError();
    if (err != 0) {
        std::cerr<<'('<<err<<")- "<< gluErrorString(err) << std::endl;
        throw std::runtime_error("gl_exception");
    }
}
#endif