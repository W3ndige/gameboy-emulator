#include "gameboy.hpp"

Gameboy::Gameboy() : gpu(&memory), cpu(&memory) {}

void Gameboy::Emulate() {

    /*if (cpu.Breakpoint(0x100) || debug) {
        cpu.Diagnostics();
        debug = 1;
        q = getchar();
    }
    if (q == 'q') {
        end = true;
    }*/

    unsigned int current_cycle = cpu.GetLastOpcodeTime();
    cpu.FetchAndDispatch();
    unsigned int cycles = cpu.GetLastOpcodeTime() - current_cycle;
    gpu.UpdateGraphics(cycles);
}