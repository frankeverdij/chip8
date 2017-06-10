#include "chip8.h"

chip8::chip8(): window(NULL),renderer(NULL),running(true)
{}

bool chip8::initRender(const short width, const short height)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Can't init" << std::endl;
        return false;
    }

    if ((window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height ,SDL_WINDOW_OPENGL)) == NULL) {
        std::cout << "Can't create window" << SDL_GetError() << std::endl;
        return false;
    }

    if ((renderer = SDL_CreateRenderer(window, -1, 0)) == NULL) {
        std::cout << "Can't create renderer" << SDL_GetError() << std::endl;
        return false;
    }

    glClearColor(0.2f, 0.2f, 0.5f, 0);

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, height, 0, 1, -1);

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_TEXTURE_2D);

    glLoadIdentity();

    return true;
    
}

void chip8::cleanupRender()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int chip8::loop()
{
    if (initRender(640,320) == false) {
        std::cout << "Render init failed" << std::endl;
        return -1;
    }

    while (running)
    {
        
    }

    cleanupRender();

    return 0;
}
