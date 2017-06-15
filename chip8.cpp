#include "chip8.h"

chip8::chip8(): window_(NULL),renderer_(NULL),running_(true)
{}

unsigned short chip8::fetch()
{
    unsigned short opcode = memory_[pc_] << 8 || memory_[pc_ + 1];
    pc_ += 2;
    return opcode;
}

unsigned short chip8::keypress()
{
    return 0;
}


void chip8::handle_0(const unsigned short& opcode)
{
    switch (opcode)
    {
        case 0x00E0 :
            clearDisplay();
            break;
        case 0x00EE :
            pc_ = stack_.top();
            stack_.pop();
            break;
        default : // RCA 1802 call
            stack_.push(pc_);
            pc_ = get_opcode_address(opcode);
    }
}

void chip8::handle_8(const unsigned short& opcode)
{
    unsigned char lsb = opcode & 0x0f;
    switch (lsb)
    {
        case 0x00 :
            v_[get_opcode_X(opcode)] = v_[get_opcode_Y(opcode)];
            break;
        case 0x01 :
            v_[get_opcode_X(opcode)] |= v_[get_opcode_Y(opcode)];
            break;
        case 0x02 :
            v_[get_opcode_X(opcode)] &= v_[get_opcode_Y(opcode)];
            break;
        case 0x03 :
            v_[get_opcode_X(opcode)] ^= v_[get_opcode_Y(opcode)];
            break;
        case 0x04 :
            
            break;
        case 0x05 :
            
            break;
        case 0x06 :
            v_[15] = (v_[get_opcode_X(opcode)] & 0x01);
            v_[get_opcode_X(opcode)] >> 1;
            break;
        case 0x07 :
            
            break;
        case 0x0E :
            v_[15] = (v_[get_opcode_X(opcode)] & 0x80) >> 7;
            v_[get_opcode_X(opcode)] << 1;            
            break;
        default : // illegal 8XY. call
            break;
    }
}

void chip8::handle_e(const unsigned short& opcode)
{
    unsigned char lsb = opcode & 0xff;
    switch (lsb)
    {
        case 0x9E :
            pc_ += (v_[get_opcode_X(opcode)] == keypress()) ? 2 : 0;
            break;
        case 0xA1 :
            pc_ += (v_[get_opcode_X(opcode)] != keypress()) ? 2 : 0;
            break;
        default : // illegal 9X.. call
            break;
    }
}

void chip8::handle_f(const unsigned short& opcode)
{
}

unsigned short chip8::get_opcode_address(const unsigned short& opcode)
{
    return opcode & 0x0fff;
}

unsigned char chip8::get_opcode_X(const unsigned short& opcode)
{
    return (opcode >> 8) & 0x0f;
}

unsigned char chip8::get_opcode_Y(const unsigned short& opcode)
{
    return (opcode >> 4) & 0x0f;
}

unsigned char chip8::get_opcode_val(const unsigned short& opcode)
{
    return opcode & 0xff;
}

void chip8::decode_and_execute(const unsigned short& opcode)
{
    unsigned char msb = (opcode >> 8) & 0xf0;
    switch (msb)
    {
        case 0x00 :
            handle_0(opcode);
            break;
        case 0x01 :
            pc_ = get_opcode_address(opcode);
            break;
        case 0x02 :
            stack_.push(pc_);
            pc_ = get_opcode_address(opcode);
            break;
        case 0x03 :
            pc_ += (v_[get_opcode_X(opcode)] == get_opcode_val(opcode)) ? 2 : 0;
            break;
        case 0x04 :
            pc_ += (v_[get_opcode_X(opcode)] != get_opcode_val(opcode)) ? 2 : 0;
            break;
        case 0x05 :
            pc_ += (v_[get_opcode_X(opcode)] == v_[get_opcode_Y(opcode)]) ? 2 : 0;
            break;
        case 0x06 :
            v_[get_opcode_X(opcode)] = get_opcode_val(opcode);
            break;
        case 0x07 :
            v_[get_opcode_X(opcode)] += get_opcode_val(opcode);
            break;
        case 0x08 :
            handle_8(opcode);
            break;
        case 0x09 :
            pc_ += (v_[get_opcode_X(opcode)] != v_[get_opcode_Y(opcode)]) ? 2 : 0;
            break;
        case 0x0a :
            i_ = get_opcode_address(opcode);
            break;
        case 0x0b :
            pc_ = v_[0] + get_opcode_address(opcode);
            break;
        case 0x0c :
            v_[get_opcode_X(opcode)] = get_opcode_val(opcode);
            break;
        case 0x0d :
            // sprite
            break;
        case 0x0e :
            handle_e(opcode);
            break;
        case 0x0f :
            handle_f(opcode);
            break;
        default :
            break;
    }
}

int chip8::loop()
{
    if (initRender(640,320) == false) {
        std::cout << "Render init failed" << std::endl;
        return -1;
    }

    while (running_)
    {
        unsigned short opcode = fetch();
        decode_and_execute(opcode);
    }

    cleanupRender();

    return 0;
}
