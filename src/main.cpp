#include "cpu.hpp"

// For now we don't need SDL2 code, let's implement the logic first. 

int debug = 0;

int main(int argv, char **argc) {
    if (argv < 2) {
        puts("Usage:\t./gb_emulator <rom_file>");
        return 1;
    }
    
    CPU cpu;

    for (;;) {
        if (cpu.Breakpoint(0x0064)) {
            cpu.Diagnostics();
            debug = 1;
            getchar();
        }
        if (debug) {
            cpu.Diagnostics();
            getchar();
        }
        cpu.FetchAndDispatch(debug);
    }

    return 0;
}