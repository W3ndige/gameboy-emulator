#include "../libs/cxxopts.hpp"
#include "gameboy/gameboy.hpp"
#include "gameboy/disassembler.hpp"

int main(int argc, char **argv) {

    bool debugger = false;
    bool without_boot = false;
    bool exit_on_infinite = false;
    bool extract = false;
    //bool headless = false;
    std::string rom_file;

    try {
        cxxopts::Options options("Gameboy Emulator", "Simple emulator for Gameboy.");
        options.add_options()
        ("d,debugger", "Run with debugger enabled.", cxxopts::value<bool>(debugger))
        ("q,quick", "Run without boot.", cxxopts::value<bool>(without_boot))
        ("j,jump-loop", "Exit when inifite jump encountered", cxxopts::value<bool>(exit_on_infinite))
        ("e,extract", "Extract and disassemble game data", cxxopts::value<bool>(extract));
        //("h,headless", "Run in headless state.", cxxopts::value<bool>(headless));
        auto result = options.parse(argc, argv);
        if (argc < 2) {
            printf("No ROM image specified.\n");
            return -1;
        }
        rom_file = argv[1];
    }

    catch (const cxxopts::OptionException &exception) {
        printf("Error parsing arguments: %s\n", exception.what());
        return -1;
    }

    try {
        if (extract) {
            Disassembler disassembler;
            disassembler.Init(rom_file);
            disassembler.Disassemble();
        } else {
            Gameboy gameboy(debugger, without_boot, exit_on_infinite, rom_file);
            gameboy.Loop();
        }
    }
    catch (const char *e) {
        std::cout << "Exception occured: " << e << std::endl;
        return -1;
    }

    return 0;
}