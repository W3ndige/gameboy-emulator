#include "cpu.hpp"

// For now we don't need SDL2 code, let's implement the logic first. 

int debug = 1;

int main(int argv, char **argc) {
    if (argv < 2) {
        puts("Usage:\t./gb_emulator <rom_file>");
        return 1;
    }
    
    CPU cpu;

    // TODO STEP
    for (;;) {
        if (debug) {
            cpu.Diagnostics();
            getchar();
        }
        cpu.FetchAndDispatch(debug);
    }


    return 0;
}