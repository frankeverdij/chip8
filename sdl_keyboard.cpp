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
            if (keypress < 16)
            {
                keys_[keypress] = true;
            }
            break;
        case SDL_KEYUP :
            keypress = key_pressed(&(Event->key));
            if (keypress < 16)
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
            if (key < 16)
            {
                return key;
            }
        }
    }
}

unsigned char chip8::key_pressed(SDL_KeyboardEvent * ke)
{
    switch ((ke->keysym).sym)
    {
        case SDL_SCANCODE_1 :
            return 0;
        case SDL_SCANCODE_2 :
            return 1;
        case SDL_SCANCODE_3 :
            return 2;
        case SDL_SCANCODE_4 :
            return 3;
        case SDL_SCANCODE_Q :
            return 4;
        case SDL_SCANCODE_W :
            return 5;
        case SDL_SCANCODE_E :
            return 6;
        case SDL_SCANCODE_R :
            return 7;
        case SDL_SCANCODE_A :
            return 8;
        case SDL_SCANCODE_S :
            return 9;
        case SDL_SCANCODE_D :
            return 10;
        case SDL_SCANCODE_F :
            return 11;
        case SDL_SCANCODE_Z :
            return 12;
        case SDL_SCANCODE_X :
            return 13;
        case SDL_SCANCODE_C :
            return 14;
        case SDL_SCANCODE_V :
            return 15;
        default :
            return 16;
    }
   
}
