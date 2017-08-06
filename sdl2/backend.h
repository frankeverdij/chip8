#ifndef SDL2_BACKEND_H
#define SDL2_BACKEND_H

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "chip8.h"

using namespace std;

class sdl2_back : public chip8 {

    private:
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        unsigned short width_;
        unsigned short height_;
        GLuint texhandle_;

        static map<SDL_Scancode, unsigned char> keymap;
        unsigned char key_pressed(SDL_KeyboardEvent *);

        SDL_Event Event;

    public:
        sdl2_back();
        bool initRender();
        void draw(unsigned char *);
        void cleanupRender();

        bool pollEvent();
        void handleEvent();
        unsigned char waitForKeyPress();
        void delay(unsigned int);
};

#endif /* SDL2_BACKEND_H */
