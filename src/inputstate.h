#ifndef INPUTSTATE
#define INPUTSTATE

#include <array>
#include <SDL/SDL.h>

struct inputstate {
    //values for keys are in the form SDLK_a or SDLK_UP
    //keys that were pressed this frame
    std::array<bool, SDLK_LAST> press = {};
    //keys that are being held down
    std::array<bool, SDLK_LAST> down ={};
}; 
#endif