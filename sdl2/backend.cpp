#include "sdl2/backend.h"

sdl2_back::sdl2_back() : window_(NULL),
                         renderer_(NULL),
                         width_(640),
                         height_(320)
{}

void sdl2_back::delay(unsigned int seconds)
{
    SDL_Delay(seconds);
}

bool sdl2_back::pollEvent()
{
    return SDL_PollEvent(&Event);
}
