#ifndef CHIP8_H
#define CHIP8_H

#include <stack>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

class chip8
{
    private :
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        bool running_;
        GLuint texhandle_;

        unsigned char v_[16];
        unsigned short i_;
        unsigned short pc_;
        std::stack<unsigned short> stack_;
        unsigned char delay_;
        unsigned char sound_;
        unsigned char memory_[4096];
        unsigned char gfx_[64 * 32];

        inline unsigned short get_opcode_address(const unsigned short&);
        inline unsigned char get_opcode_X(const unsigned short&);
        inline unsigned char get_opcode_Y(const unsigned short&);
        inline unsigned char get_opcode_val(const unsigned short&);

    public :
        static unsigned char fontset[80];
        chip8();
        bool loadRom(const string&);
        bool initRender(const short, const short);
        void draw(const short, const short);
        void clearDisplay();
        void cleanupRender();
        unsigned short fetch();
        unsigned short keypress();
        void decode_and_execute(const unsigned short&);
        inline void handle_0(const unsigned short&);
        inline void handle_8(const unsigned short&);
        inline void handle_e(const unsigned short&);
        inline void handle_f(const unsigned short&);
        int loop();
};

#endif /* CHIP*_H */
