#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "debugger.hpp"
#include "../gpu/gpu.hpp"
#include "../cpu/cpu.hpp"

class Gameboy {
    public:
        Gameboy(bool debug);
        void Loop();

    private:
        Memory memory;
        CPU cpu;
        GPU gpu;
        
        bool debugging;
        friend class Debugger;
        Debugger debugger;

        void Emulate();

};

#endif