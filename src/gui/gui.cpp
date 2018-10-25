#include "gui.hpp"

GUI::GUI() {
    window = nullptr;
    renderer = nullptr;
}

bool GUI::Init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow("Gameboy", SDL_WINDOWPOS_CENTERED, 100,
                                          160, 144, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf( "SDL Window could not initialize! SDL_Error: %s\n", SDL_GetError() );
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        printf( "SDL Renderer could not initialize! SDL_Error: %s\n", SDL_GetError() );
        SDL_Quit();
        return false;
    }
    
    return true;
}