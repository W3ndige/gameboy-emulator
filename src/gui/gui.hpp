#ifndef GUI_HPP
#define GUI_HPP

#include <SDL2/SDL.h>

class GUI {
    public:
        GUI();
        bool Init();
        SDL_Renderer *renderer;

    private:
        SDL_Window *window;
};

#endif