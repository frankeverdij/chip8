#include "chip8.h"

void chip8::handleEvent(SDL_Event * Event)
{
    unsigned char keypress;

    switch (Event->type)
    {
        case SDL_QUIT :
            running_ = false;
            break;
        case SDL_KEYDOWN :
            keypress = key_pressed(&(Event->key));
            if (keypress != 255)
            {
                keys_[keypress] = true;
            }
            break;
        case SDL_KEYUP :
            keypress = key_pressed(&(Event->key));
            if (keypress != 255)
            {
                keys_[keypress] = false;
            }
            break;
        default :
            break;
    }
}

unsigned char chip8::waitForKeyPress()
{
    SDL_Event Event;
    unsigned char key;

    while (true)
    {
        SDL_WaitEvent(&Event);

        if (Event.type == SDL_KEYDOWN)
        {
            key = key_pressed(&(Event.key));
            if (key != 255)
            {
                return key;
            }
        }
    }
}

map<SDL_Scancode, unsigned char> chip8::keymap = {
    {SDL_SCANCODE_1, 0x01},
    {SDL_SCANCODE_2, 0x02},
    {SDL_SCANCODE_3, 0x03},
    {SDL_SCANCODE_4, 0x0c},
    {SDL_SCANCODE_Q, 0x04},
    {SDL_SCANCODE_W, 0x05},
    {SDL_SCANCODE_E, 0x06},
    {SDL_SCANCODE_R, 0x0d},
    {SDL_SCANCODE_A, 0x07},
    {SDL_SCANCODE_S, 0x08},
    {SDL_SCANCODE_D, 0x09},
    {SDL_SCANCODE_F, 0x0e},
    {SDL_SCANCODE_Z, 0x0a},
    {SDL_SCANCODE_X, 0x00},
    {SDL_SCANCODE_C, 0x0b},
    {SDL_SCANCODE_V, 0x0f}
};

unsigned char chip8::key_pressed(SDL_KeyboardEvent * kbe)
{
    unsigned char key;

    try
    {
        key = keymap.at((kbe->keysym).scancode);
    }
    catch (const std::out_of_range& oor)
    {
        key = 255;
    }

    return key;
}
