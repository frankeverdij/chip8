#ifndef CHIP8_H
#define CHIP8_H

#include <queue>

class chip8
{
    unsigned char v_[16];
    unsigned short i_;
    unsigned short pc_;
    std::queue<unsigned short> stack;
    unsigned char delay;
    unsigned char sound;
    unsigned char memory[4096];
    unsigned char gfx[64 * 32];

    inline unsigned short get_opcode_address(unsigned short);
    inline unsigned char get_opcode_X(unsigned short);
    inline unsigned char get_opcode_Y(unsigned short);
    inline unsigned char get_opcode_val(unsigned short);

    public :

};

#endif /* CHIP*_H */
