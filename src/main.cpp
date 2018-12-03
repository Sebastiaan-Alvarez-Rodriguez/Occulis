#include <thread>
#include <chrono>
#include <stdexcept>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <iostream>
#include <sstream>
#include <getopt.h>
#include "Self.h"
#include "inputstate.h"

size_t width = 1920, height = 1080;
float fps_lim = 60;
size_t grass_amt = 500000;

const SDL_Surface* screensurface;

static inline void fpsCounter(size_t& frames, std::chrono::high_resolution_clock::time_point& program_start) {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur = now - program_start;
    ++frames;
    if (dur.count() > 1) {
        std::stringstream ss;
        ss << "Landscape | fps: " << (frames / dur.count());
        SDL_WM_SetCaption(ss.str().c_str(), NULL);
        program_start = now;
        frames = 0;
    }
}

static inline void frameLimiter(int lim, std::chrono::high_resolution_clock::time_point& frame_start) {
    auto now = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::duration<float>(1 / (float)lim) - (now - frame_start));
    frame_start = std::chrono::high_resolution_clock::now();    
}

void printControls() {
    std::cout 
        << "------------------------------------CONTROLS------------------------------------"<<std::endl
        << "Camera" << std::endl
        << "\tRotations" << std::endl
        << "\t\t up:            I" << std::endl
        << "\t\t down:          K" << std::endl
        << "\t\t left:          J" << std::endl
        << "\t\t right:         L" << std::endl
        << "\t Movements" << std::endl
        << "\t\t up:            R" << std::endl
        << "\t\t down:          F" << std::endl
        << "\t\t forward:       W" << std::endl
        << "\t\t backward:      S" << std::endl
        << "\t\t left:          A" << std::endl
        << "\t\t right:         D" << std::endl
        << "Atmosphere" << std::endl
        << "\t increase time: Y" << std::endl
        << "\t decrease time: H" << std::endl
        << "Wind" <<std::endl
        << "\t increase wind: O" << std::endl
        << "\t decrease wind: P" << std::endl
        << "Toggles" << std::endl
        << "\t wireframe:     T" << std::endl
        << "\t grass:         G" << std::endl
        << "\t daycycles:     Z" << std::endl
        << "--------------------------------------------------------------------------------" << std::endl;
}
const SDL_Surface* createWindow() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("SDL init failed");

    //set depth buffer to 24 bits for better precision
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
    SDL_WM_SetCaption("Landscape", NULL);

    const SDL_Surface* const screensurf = 
    SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL 
        | SDL_RESIZABLE);
    if(screensurf == NULL)
        throw std::runtime_error("SDL videomode init failed");
    if (glewInit() != GLEW_OK)
        throw std::runtime_error("GLEW init failed");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    return screensurf;
}

static void showHelp(const char *progName) {
    std::cout << progName << " [-f <fps>] [-h]" << std::endl;
    std::cout << R"HERE(
    --fps
    -f       fps rate      Maximum allowed fps rate
    --grass
    -g       grass amt     Amount of grass blades per patch
    --help
    -h                     Show this panel
)HERE";
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

int main(int argc, char** argv) {
    static struct option long_options[] = {
        {"fps", required_argument, NULL, 'f'},
        {"grass", required_argument, NULL, 'g'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    const char* progName = argv[0];
    int c;
    while ((c = getopt_long(argc, argv, "f:g:h", long_options, NULL)) != -1){
        int x = 0;
        try {
            size_t pos;
            if (optarg != NULL) {
                x = std::stoi(optarg, &pos);
                if (pos < std::strlen(optarg))
                    std::cerr << "Ignoring trailing characters after number: " << optarg << '\n';
            }
        } catch (std::invalid_argument const &ex) {
            std::cerr << "Invalid number: " << optarg << '\n';
            showHelp(progName);
            return 0;
        } catch (std::out_of_range const &ex) {
            std::cerr << "Number out of range: " << optarg << '\n';
            showHelp(progName);
            return 1;
        }
        switch (c) {
            case 'f':
                if (x == 0) {
                    std::cerr << "0 fps not allowed" << std::endl;
                    showHelp(progName);
                    return 1;
                } else {
                    fps_lim = ((x >= 0) ? x : -x);
                }
                break;
            case 'g':
                if (x < 0) {
                    std::cerr <<"<0 grass blades not allowed" << std::endl;
                    showHelp(progName);
                    return 1;
                } else {
                    grass_amt = (size_t) x;
                }
                break;
            case 'h':
            default:
                showHelp(progName);
                return 0;
            
        }
    }//while
    argc -= optind;
    argv += optind;

    try {
        screensurface = createWindow();
    } catch(std::runtime_error e) {
        std::cerr << "main failed to create window: " << e.what() << std::endl;
        return 1;
    }



    printControls();
    auto lasttime = SDL_GetTicks(), 
         newtime = lasttime;
    size_t frames = 0;

    inputstate in;
    Self program(in, width, height, grass_amt);

    auto program_start = std::chrono::high_resolution_clock::now();
    auto frame_start = std::chrono::high_resolution_clock::now();
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