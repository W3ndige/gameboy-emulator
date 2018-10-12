#include "cpu.hpp"

// For now we don't need SDL2 code, let's implement the logic first. 

int main(int argv, char **argc) {
    if (argv < 2) {
        puts("Usage:\t./gb_emulator <rom_file>");
        return 1;
    }
    
    CPU cpu;

    // Test execution
    cpu.ExecuteInstruction();
    cpu.Diagnostics();


    return 0;
}