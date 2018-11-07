#include <iostream>

static inline bool errCheck() {
    auto err = glGetError(); 
    if (err)
        std::cerr<<'('<<err<<")- "<< gluErrorString(err);
    return err != 0;
}