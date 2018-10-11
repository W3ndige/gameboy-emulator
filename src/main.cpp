#include "types.hpp"
#include <SDL2/SDL.h>

const int FPS = 60; /**< Frames per second */
const int TICKS_PER_FRAME = 1000 / 60;  /**< Number of CPU ticks per one frame */
const size_t SCREEN_WIDTH  = 160; /**< Width of the screen */
const size_t SCREEN_HEIGHT = 144; /**< Height of the screen */

int main(int argv, char **argc) {
    if (argv < 2) {
        puts("Usage:\t./gb_emulator <rom_file>");
        return 1;
    }

    // Try loading ROM here

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Flood Fill", SDL_WINDOWPOS_CENTERED, 100,
                                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        printf( "SDL Window could not initialize! SDL_Error: %s\n", SDL_GetError() );
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        printf( "SDL Renderer could not initialize! SDL_Error: %s\n", SDL_GetError() );
        SDL_Quit();
        return 1;
    }


    return 0;
}