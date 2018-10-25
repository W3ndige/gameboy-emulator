#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "../gpu/gpu.hpp"
#include "../cpu/cpu.hpp"

class Gameboy {
    public:
        Gameboy(bool debugger);
        void Debugger();
        void Emulate();

    private:
        bool debugger;
        Memory memory;
        GPU gpu;
        CPU cpu;
};

#endif