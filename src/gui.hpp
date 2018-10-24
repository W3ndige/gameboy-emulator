#ifndef GUI_HPP
#define GUI_HPP

#include <SDL2/SDL.h>

class GUI {
    public:
        GUI();
        bool Init();
        SDL_Window *window;
        SDL_Renderer *renderer;
};

#endif