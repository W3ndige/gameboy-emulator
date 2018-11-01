#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "debugger.hpp"
#include "../gpu/gpu.hpp"
#include "../cpu/cpu.hpp"

class Gameboy {
    public:
        Gameboy(bool debug);
        void Emulate();

    private:
        Memory memory;
        CPU cpu;
        GPU gpu;
        
        bool debugging;
        friend class Debugger;
        Debugger debugger;

};

#endif