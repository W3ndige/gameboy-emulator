#include "gpu.hpp"
#include "cpu.hpp"

int debug = 0;

int main(int argv, char **argc) {
    if (argv < 2) {
        puts("Usage:\t./gb_emulator <rom_file>");
        return 1;
    }
    
    Memory memory;
    GPU gpu(&memory);
    CPU cpu(&memory);

    bool end = false;
    SDL_Event event;
    char q;

    while (!end) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                end = true;
            }
        }
        /*if (cpu.Breakpoint(0x100) || debug) {
            cpu.Diagnostics();
            debug = 1;
            q = getchar();
        }
        if (q == 'q') {
            end = true;
        }*/
        unsigned int current_cycle = cpu.GetLastOpcodeTime();
        cpu.FetchAndDispatch(debug);
        unsigned int cycles = cpu.GetLastOpcodeTime() - current_cycle;
        gpu.UpdateGraphics(cycles);

    }

    return 0;
}