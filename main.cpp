#include "sdl2/backend.h"

int main(int argc, char* argv[])
{
    sdl2_back myChip8;

    if (argc > 1)
    {
        if (myChip8.loadRom(argv[1]) == false)
        {
            cout << "can't load " << argv[1] << endl;
            exit(-1);
        }
    }
    else
    {
        cout << "usage: chip8 <file>" << endl;
        exit(0);
    }

    return myChip8.loop();
}
