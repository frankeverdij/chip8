#ifndef CHIP8_H
#define CHIP8_H

#include <stack>
#include <vector>
#include <map>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <iomanip>
#include <string>

using namespace std;

class chip8
{
    private :

        bool running_;
        bool draw_;

        // Registers
        unsigned char v_[16];
        unsigned short i_;
        unsigned short pc_;
        stack<unsigned short> stack_;
        unsigned char delay_;
        unsigned char sound_;

        // Data blocks
        vector<unsigned char> memory_;
        unsigned char gfx_[64 * 32];
        unsigned char keys_[16];
        static unsigned char fontset[80];

        // Inline opcode handling
        inline unsigned short get_opcode_address(const unsigned short&);
        inline unsigned char get_opcode_X(const unsigned short&);
        inline unsigned char get_opcode_Y(const unsigned short&);
        inline unsigned char get_opcode_val(const unsigned short&);
        inline void handle_0(const unsigned short&);
        inline void handle_8(const unsigned short&);
        inline void handle_d(const unsigned short&);
        inline void handle_e(const unsigned short&);
        inline void handle_f(const unsigned short&);

        void clearDisplay();
        void handleCounters();

        unsigned short fetch();
        void decode_and_execute(const unsigned short&);
        void print_illegal(unsigned char, const unsigned short&);

    public :

        chip8();
        void setRunning(bool);
        void setDraw(bool);
        void setKey(unsigned char, bool);
        bool loadRom(const char *);
        int loop();

        // Render functions handling backend
        virtual bool initRender() = 0;
        virtual void draw(unsigned char *) = 0;
        virtual void cleanupRender() = 0;

        // Keyboard handling backend
        virtual bool pollEvent() = 0;
        virtual void handleEvent() = 0;
        virtual unsigned char waitForKeyPress() = 0;

        // sound and delay handling backend
        virtual void delay(unsigned int) = 0;

};

#endif /* CHIP8_H */
