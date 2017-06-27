#include "chip8.h"

bool chip8::initRender()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Can't init" << std::endl;
        return false;
    }

    if ((window_ = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_ ,SDL_WINDOW_OPENGL)) == NULL) {
        std::cout << "Can't create window" << SDL_GetError() << std::endl;
        return false;
    }

    if ((renderer_ = SDL_CreateRenderer(window_, -1, 0)) == NULL) {
        std::cout << "Can't create renderer" << SDL_GetError() << std::endl;
        return false;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0);

    glViewport(0, 0, width_, height_);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width_, 0, height_, -1, 1);

    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_LIGHTING);

    return true;
    
}

void chip8::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glGenTextures(1, &texhandle_);

    glBindTexture(GL_TEXTURE_2D, texhandle_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 64, 32, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, gfx_);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex2i(0, 0);
    glTexCoord2i(0, 0); glVertex2i(0, height_);
    glTexCoord2i(1, 0); glVertex2i(width_, height_);
    glTexCoord2i(1, 1); glVertex2i(width_, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    glFlush();
    SDL_GL_SwapWindow(window_);
    draw_ = false;
}

void chip8::cleanupRender()
{
    SDL_DestroyWindow(window_);
    SDL_Quit();
}


