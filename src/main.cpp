#include "../libs/cxxopts.hpp"
#include "gameboy/gameboy.hpp"

int main(int argc, char **argv) {

    bool debugger = false;
    try {
        cxxopts::Options options("Gameboy Emulator", "Simple emulator for Gameboy.");
        options.add_options()
        ("d,debugger", "Run with debugger enabled.", cxxopts::value<bool>(debugger))
        ("r,rom", "ROM file of an cartridge", cxxopts::value<std::string>());
        auto result = options.parse(argc, argv);
    }

    catch (const cxxopts::OptionException &exception) {
        printf("Error parsing arguments: %s\n", exception.what());
    }

    Gameboy gameboy;
    bool end = false;
    SDL_Event event;

    while (!end) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                end = true;
            }
        }
        gameboy.Emulate();

    }

    return 0;
}