#include "../libs/cxxopts.hpp"
#include "gameboy/gameboy.hpp"

int main(int argc, char **argv) {

    bool debugger = false;
    std::string rom_file;

    try {
        cxxopts::Options options("Gameboy Emulator", "Simple emulator for Gameboy.");
        options.add_options()
        ("d,debugger", "Run with debugger enabled.", cxxopts::value<bool>(debugger));
        auto result = options.parse(argc, argv);
        rom_file = argv[1];

    }

    catch (const cxxopts::OptionException &exception) {
        printf("Error parsing arguments: %s\n", exception.what());
        return -1;
    }

    try {
        Gameboy gameboy(debugger, rom_file);
        gameboy.Loop();
    }
    catch (const char *e) {
        std::cout << "Exception occured: " << e << std::endl;
        return -1;
    }

    return 0;
}