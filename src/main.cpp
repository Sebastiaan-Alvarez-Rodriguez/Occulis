#include <thread>
#include <chrono>
#include <stdexcept>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <iostream>
#include "Self.h"
#include "inputstate.h"

size_t width = 800, height = 600;
float fps_lim = 60;
const SDL_Surface* screensurface;

static inline void fpsCounter(size_t& frames, std::chrono::high_resolution_clock::time_point& program_start) {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur = now - program_start;
    ++frames;
    if (dur.count() > 1) {
        std::cout << "fps: " << (frames / dur.count()) << std::endl;
        program_start = now;
        frames = 0;
    }
}

static inline void frameLimiter(int lim, std::chrono::high_resolution_clock::time_point& frame_start) {
    auto now = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::duration<float>(1 / (float)lim) - (now - frame_start));
    frame_start = std::chrono::high_resolution_clock::now();    
}

const SDL_Surface* createWindow() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL init failed");

    //set depth buffer to 24 bits for better precision
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
    SDL_WM_SetCaption("Self2", NULL);

    const SDL_Surface* const screensurf = 
    SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL 
        | SDL_RESIZABLE);
    if(screensurf == NULL)
        throw std::runtime_error("SDL videomode init failed");
    if (glewInit() != GLEW_OK)
        throw std::runtime_error("GLEW init failed");
    return screensurf;
}

void mainEventHandler(inputstate* in, bool* running) {
    SDL_Event sdlevent;

    in->press.fill(false);
    
    while(SDL_PollEvent(&sdlevent)) {
        switch(sdlevent.type) {
            case SDL_KEYDOWN:
                if (sdlevent.key.keysym.sym == SDLK_ESCAPE
                    || sdlevent.key.keysym.sym == SDLK_q) {
                    *running = false;
                    return;
                } else {
                    in->press[sdlevent.key.keysym.sym] = true;
                    in->down[sdlevent.key.keysym.sym] = true;
                }
                break;
            case SDL_KEYUP:
                in->down[sdlevent.key.keysym.sym] = false;
                break;
            case SDL_QUIT:
                *running = false;
                return;
            case SDL_VIDEORESIZE:
                width = sdlevent.resize.w;
                height = sdlevent.resize.h;
                break;
        }
    }
}

int main() {
    try {
        screensurface = createWindow();
    } catch(std::runtime_error e) {
        std::cerr << "main failed to create window: " << e.what() << std::endl;
        return 1;
    }

    auto lasttime = SDL_GetTicks(), 
         newtime = lasttime;
    auto program_start = std::chrono::high_resolution_clock::now();
    auto frame_start = std::chrono::high_resolution_clock::now();
    size_t frames = 0;

    inputstate in;
    Self program(in);

    bool running = true;
    while (running) {
        fpsCounter(frames, program_start);//count fps
        frameLimiter(fps_lim, frame_start); //limit frames
        mainEventHandler(&in, &running);

        newtime = SDL_GetTicks();
        program.update(width, height, (newtime - lasttime) * 0.001);
        lasttime = newtime;

        program.render();
        SDL_GL_SwapBuffers();
    }
    SDL_Quit();
    return 0;
}