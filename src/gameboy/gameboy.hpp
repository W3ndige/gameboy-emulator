#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <thread>
#include "debugger.hpp"
#include "../gpu/gpu.hpp"
#include "../cpu/cpu.hpp"

const float FPS         = 59.73f;
const int   MAX_CYCLES  = 70224;
const float DELAY_TIME  = 1000.0f / FPS;

class Gameboy {
    public:
        Gameboy(bool debug, bool without_boot, bool exit_on_inifite, std::string rom_file);
        void Loop();

    private:
        Memory memory;
        CPU cpu;
        GPU gpu;
        
        bool debugging;
        friend class Debugger;
        Debugger debugger;

        void Emulate();
        void SetKeyPressed(int key);
        void SetKeyReleased(int key);

};

#endif