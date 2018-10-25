#include "../libs/cxxopts.hpp"
#include "gameboy/gameboy.hpp"

int main(int argv, char **argc) {

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