#include "gpu.hpp"
#include "cpu.hpp"

// For now we don't need SDL2 code, let's implement the logic first. 

int debug = 0;

int main(int argv, char **argc) {
    if (argv < 2) {
        puts("Usage:\t./gb_emulator <rom_file>");
        return 1;
    }
    
    Memory memory;
    GPU gpu(&memory);
    CPU cpu(&memory);

    for (;;) {
        /*if (cpu.Breakpoint(0x0099)) {
            cpu.Diagnostics();
            debug = 1;
            getchar();
        }
        if (debug) {
            cpu.Diagnostics();
            getchar();
        }*/
        unsigned int current_cycle = cpu.GetLastOpcodeTime();
        cpu.FetchAndDispatch(debug);
        unsigned int cycles = cpu.GetLastOpcodeTime() - current_cycle;
        gpu.UpdateGraphics(cycles);
    }

    return 0;
}