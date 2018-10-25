#include "gameboy.hpp"

Gameboy::Gameboy(bool debugger) : gpu(&memory), cpu(&memory) {
    this->debugger = debugger;
    bool init = cpu.Init();
    if (!init) {
        throw "Could not initialze CPU.";
    }
}

void Gameboy::Debugger() {
    std::string input;
    //printf("Welcome to Gameboy debugger\n");
    //printf("> ");
    //scanf("%s\n", input);
}

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