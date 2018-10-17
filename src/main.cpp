#include "cpu.hpp"

// For now we don't need SDL2 code, let's implement the logic first. 

int main(int argv, char **argc) {
    if (argv < 2) {
        puts("Usage:\t./gb_emulator <rom_file>");
        return 1;
    }
    
    CPU cpu;

    // TODO STEP
    for (int i = 0; i < 0x1000; i++) {
        if (cpu.Breakpoint(0x0045)) {
            cpu.Diagnostics();
            break;
        }
        cpu.FetchAndDispatch();
    }


    return 0;
}