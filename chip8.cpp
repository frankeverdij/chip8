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

unsigned short chip8::fetch()
{
    return memory_[pc_] << 8 || memory_[pc_ + 1];
}

void chip8::clearDisplay()
{
    
}

void chip8::handle_0(const unsigned short& opcode)
{
    switch (opcode)
    {
        case 0x00E0 :
            clearDisplay();
            pc_ += 2;
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
        default : // illegal 8XY. call
            break;
    }
    pc_ += 2;
}

void chip8::handle_e(const unsigned short& opcode)
{
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
            pc_ += (v_[get_opcode_X(opcode)] == get_opcode_val(opcode)) ? 4 : 2;
            break;
        case 0x04 :
            pc_ += (v_[get_opcode_X(opcode)] != get_opcode_val(opcode)) ? 4 : 2;
            break;
        case 0x05 :
            pc_ += (v_[get_opcode_X(opcode)] == v_[get_opcode_Y(opcode)]) ? 4 : 2;
            break;
        case 0x06 :
            v_[get_opcode_X(opcode)] = get_opcode_val(opcode);
            pc_ += 2;
            break;
        case 0x07 :
            v_[get_opcode_X(opcode)] += get_opcode_val(opcode);
            pc_ += 2;
            break;
        case 0x08 :
            handle_8(opcode);
            break;
        case 0x09 :
            pc_ += (v_[get_opcode_X(opcode)] != v_[get_opcode_Y(opcode)]) ? 4 : 2;
            break;
        case 0x0a :
            i_ = get_opcode_address(opcode);
            pc_ += 2;
            break;
        case 0x0b :
            pc_ = v_[0] + get_opcode_address(opcode);
            break;
        case 0x0c :
            v_[get_opcode_X(opcode)] = get_opcode_val(opcode);
            pc_ += 2;
            break;
        case 0x0d :
            // sprite
            pc_ += 2;
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
        unsigned short opcode = fetch();
        decode_and_execute(opcode);
    }

    cleanupRender();

    return 0;
}
