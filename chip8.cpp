#include "chip8.h"

unsigned char chip8::fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

chip8::chip8() : window_(NULL),
                 renderer_(NULL),
                 width_(640),
                 height_(320),
                 running_(true),
                 memory_(4096),
                 pc_(0x200),
                 i_(0)
{
    clearDisplay();

    for (int j = 0; j < 80; ++j)
        memory_[j] = fontset[j];

    for (int j = 0; j < 16; ++j)
    {
        v_[j] = 0;
        keys_[j] = false;
    }
}

unsigned short chip8::fetch()
{
    unsigned short opcode = memory_[pc_] << 8 | memory_[pc_+1];

//    cout << "memory " << showbase << internal << hex << setw(2) << (int) memory_[pc_] << " : " << (int) memory_[pc_ + 1] << endl;
//    cout << "opcode " << opcode << " pc " << (int)pc_ << endl;
    pc_ += 2;
    return opcode;
}

void chip8::clearDisplay()
{
    std::fill_n(gfx_, 64 * 32, 0);
    draw_ = true;
}

inline void chip8::handle_0(const unsigned short& opcode)
{
    switch (opcode)
    {
        case 0x00E0 :
            clearDisplay();
            break;
        case 0x00EE :
            if (!stack_.empty())
            {
                pc_ = stack_.top();
                stack_.pop();
            }
            else
            {
                cerr << "Stack is empty!" << endl;
                exit(-1);
            } 
            break;
        default : // RCA 1802 call, but that's illegal
            print_illegal(0x0, opcode);
            exit(0);
            break;
    }
}

void chip8::print_illegal(unsigned char handle, const unsigned short& opcode)
{
    cout << "Illegal " << showbase << internal << setfill('0') << hex << setw(2) << (int) handle << " opcode " << setw(4) << opcode << " , quitting..." << endl;
}

inline void chip8::handle_8(const unsigned short& opcode)
{
    unsigned char lsb = opcode & 0x0f;
    unsigned char x = v_[get_opcode_X(opcode)];
    unsigned char y = v_[get_opcode_Y(opcode)];
    unsigned short add;

    switch (lsb)
    {
        case 0x00 :
            v_[get_opcode_X(opcode)] = y;
            break;
        case 0x01 :
            v_[get_opcode_X(opcode)] |= y;
            break;
        case 0x02 :
            v_[get_opcode_X(opcode)] &= y;
            break;
        case 0x03 :
            v_[get_opcode_X(opcode)] ^= y;
            break;
        case 0x04 :
            add = x;
            add += y;
            v_[get_opcode_X(opcode)] = add & 0xff;
            v_[15] = (add > 255) ? 1 : 0;
            break;
        case 0x05 :
            v_[15] = (x < y) ? 0 : 1;
            v_[get_opcode_X(opcode)] -= y;
            break;
        case 0x06 :
            v_[15] = (x & 0x01);
            v_[get_opcode_X(opcode)] >>= 1;
            break;
        case 0x07 :
            v_[15] = (y < x) ? 0 : 1;
            v_[get_opcode_X(opcode)] = y - x;
            break;
        case 0x0E :
            v_[15] = x >> 7;
            v_[get_opcode_X(opcode)] <<= 1;            
            break;
        default : // illegal 8XY. call
            print_illegal(0x08, opcode);
            exit(0);
            break;
    }
}

inline void chip8::handle_d(const unsigned short& opcode)
{
    unsigned char x = v_[get_opcode_X(opcode)];
    unsigned char y = v_[get_opcode_Y(opcode)];
    unsigned char n = opcode & 0x0f;
    unsigned char bits, lines, pixel;
    unsigned char j, k;
    unsigned char mask = 0x80;

    v_[15] = 0;

    if ((x > 63) || (y > 31))
    {
        return;
    }

    bits = (x + 8 <= 64) ? 8 : 64 - x;
    lines = (y + n <= 32) ? n : 32 - y;

    for (j = 0; j < lines; j++)
    {
        for (k = 0; k < bits; k++)
        {
            if (memory_[i_ + j] & (mask >> k))
            {
                pixel = gfx_[64 * (y + j) + x + k];
                v_[15] |= pixel;
                gfx_[64 * (y + j) + x + k] = pixel ^ 0xff;
            }
        }
    }
    v_[15] &= 0x01;
    draw_ = true;
}

inline void chip8::handle_e(const unsigned short& opcode)
{
    unsigned char lsb = opcode & 0xff;

    switch (lsb)
    {
        case 0x9E :
            pc_ += (keys_[v_[get_opcode_X(opcode)]]) ? 2 : 0;
            break;
        case 0xA1 :
            pc_ += !(keys_[v_[get_opcode_X(opcode)]]) ? 2 : 0;
            break;
        default : // illegal eX.. call
            print_illegal(0x0e, opcode);
            exit(0);
            break;
    }
}

inline void chip8::handle_f(const unsigned short& opcode)
{
    unsigned char lsb = opcode & 0xff;
    unsigned char bcd, j;

    switch (lsb)
    {
        case 0x07 :
            v_[get_opcode_X(opcode)] = delay_;
            break;
        case 0x0a :
            v_[get_opcode_X(opcode)] = waitForKeyPress();
            break;
        case 0x15 :
            delay_ = v_[get_opcode_X(opcode)];
            break;
        case 0x18 :
            sound_ = v_[get_opcode_X(opcode)];
            break;
        case 0x1e :
            i_ += v_[get_opcode_X(opcode)];
            if (i_ > 0x0fff)  // undocumented feature
            {
                v_[15] = 1;
            }
            else
            {
                v_[15] = 0;
            }
            break;
        case 0x29 :
            i_ = 5 * v_[get_opcode_X(opcode)];
            break;
        case 0x33 :
            bcd = v_[get_opcode_X(opcode)];
            memory_[i_ + 0] = bcd / 100;
            memory_[i_ + 1] = (bcd % 100) / 10;
            memory_[i_ + 2] = (bcd % 10);
            break;
        case 0x55 :
            for (j = 0; j <= get_opcode_X(opcode) ; j++)
            {
                memory_[i_ + j] = v_[j];
            }
            break;
        case 0x65 :
            for (j = 0; j <= get_opcode_X(opcode) ; j++)
            {
                v_[j] = memory_[i_ + j];
            }
            break;
        default : // illegal fX.. call
            print_illegal(0x0f, opcode);
            exit(0);
            break;
    }
}

inline unsigned short chip8::get_opcode_address(const unsigned short& opcode)
{
    return opcode & 0x0fff;
}

inline unsigned char chip8::get_opcode_X(const unsigned short& opcode)
{
    return (opcode >> 8) & 0x0f;
}

inline unsigned char chip8::get_opcode_Y(const unsigned short& opcode)
{
    return (opcode >> 4) & 0x0f;
}

inline unsigned char chip8::get_opcode_val(const unsigned short& opcode)
{
    return opcode & 0xff;
}

void chip8::decode_and_execute(const unsigned short& opcode)
{
    unsigned char msb = (opcode >> 12) & 0x0f;
    switch (msb)
    {
        case 0x0 :
            handle_0(opcode);
            break;
        case 0x1 :
            pc_ = get_opcode_address(opcode);
            break;
        case 0x2 :
            stack_.push(pc_);
            pc_ = get_opcode_address(opcode);
            break;
        case 0x3 :
            pc_ += (v_[get_opcode_X(opcode)] == get_opcode_val(opcode)) ? 2 : 0;
            break;
        case 0x4 :
            pc_ += (v_[get_opcode_X(opcode)] != get_opcode_val(opcode)) ? 2 : 0;
            break;
        case 0x5 :
            pc_ += (v_[get_opcode_X(opcode)] == v_[get_opcode_Y(opcode)]) ? 2 : 0;
            break;
        case 0x6 :
            v_[get_opcode_X(opcode)] = get_opcode_val(opcode);
            break;
        case 0x7 :
            v_[get_opcode_X(opcode)] += get_opcode_val(opcode);
            break;
        case 0x8 :
            handle_8(opcode);
            break;
        case 0x9 :
            pc_ += (v_[get_opcode_X(opcode)] != v_[get_opcode_Y(opcode)]) ? 2 : 0;
            break;
        case 0xa :
            i_ = get_opcode_address(opcode);
            break;
        case 0xb :
            pc_ = v_[0] + get_opcode_address(opcode);
            break;
        case 0xc :
            v_[get_opcode_X(opcode)] = (rand() % 256) & get_opcode_val(opcode);
            break;
        case 0xd :
            handle_d(opcode);
            break;
        case 0xe :
            handle_e(opcode);
            break;
        case 0xf :
            handle_f(opcode);
            break;
        default :
            print_illegal(0xff, opcode);
            exit(0);
            break;
    }
}

void chip8::handleCounters()
{
    if (sound_ != 0)
    {
        cout << '\a' << flush;
    }
    else
    {
        sound_--;
    }
    
    if (delay_ != 0)
    {
        delay_--;
    }
}

bool chip8::loadRom(const char *filename)
{
    streampos fileSize;

    ifstream file(filename, ios::binary);
    if (file.fail())
    {
        cout << "Open file failed" << endl;
        return false;
    }

    file.unsetf(ios::skipws);

    file.seekg(0, ios::end);
    fileSize = file.tellg();
    file.seekg(0, ios::beg);

    if (fileSize > 4096 - 0x200)
    {
        cout << "Rom too large" << endl;
        return false;
    }

    memory_.insert(memory_.begin() + 0x200,
               std::istream_iterator<unsigned char>(file),
               std::istream_iterator<unsigned char>());
    return true;
}

int chip8::loop()
{
    if (initRender() == false) {
        cout << "Render init failed" << endl;
        return -1;
    }

    SDL_Event Event;

    while (running_)
    {
        unsigned short opcode = fetch();
        decode_and_execute(opcode);
        if (draw_)
        {
            draw();
        }
        handleCounters();
        while(SDL_PollEvent(&Event)) {
            handleEvent(&Event);
        }
        SDL_Delay(2);
    }

    cleanupRender();

    return 0;
}
