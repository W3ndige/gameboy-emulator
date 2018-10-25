#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "../gpu/gpu.hpp"
#include "../cpu/cpu.hpp"

class Gameboy {
    public:
        Gameboy();
        void Emulate();

    private:
        Memory memory;
        GPU gpu;
        CPU cpu;
};

#endif