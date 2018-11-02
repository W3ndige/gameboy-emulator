#include "gameboy.hpp"

Gameboy::Gameboy(bool debug) : cpu(&memory), gpu(&memory, &cpu), debugger(*this) {
    bool init = cpu.Init();
    if (!init) {
        throw "Could not initialize CPU.";
    }
    debugging = debug;
    memory.PrintCartidgeHeader();
}

void Gameboy::Emulate() {

    if (debugging) {
        debugger.Debug();
    }
    
    int current_cycle = cpu.GetLastOpcodeTime();
    cpu.FetchAndDispatch();

    if (cpu.pending_interupt_enabled == true && 
        memory.ReadByteMemory(cpu.GetProgramCounter() - 1) != 0xfb) {
        cpu.interupts = true;
        cpu.pending_interupt_enabled = false;
    }

    if (cpu.pending_interupt_disabled == true && 
        memory.ReadByteMemory(cpu.GetProgramCounter() - 1) != 0xfb) {
        cpu.interupts = false;
        cpu.pending_interupt_disabled = false;
    }

    int cycles = cpu.GetLastOpcodeTime() - current_cycle;
    cpu.UpdateTimer(cycles);
    gpu.UpdateGraphics(cycles);
    cpu.DoInterupts();
}