#ifndef ERROR
#define ERROR

#include <iostream>

static inline bool errCheck() {
    auto err = glGetError();
    bool errorOccured = false;
    while (err != 0) {
        errorOccured = true;
        std::cerr<<'('<<err<<")- "<< gluErrorString(err) << std::endl;
    }
    return errorOccured;
}
#endif