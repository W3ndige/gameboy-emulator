#ifndef GPU_HPP
#define GPU_HPP

#include <SDL2/SDL.h>

#include "../memory/memory.hpp"
#include "../helper/helper.hpp"
#include "../cpu/cpu.hpp"

class GPU {
    public:
        GPU(Memory *mem, CPU *cpu);
        ~GPU();
        bool Init();
        void UpdateGraphics(int cycles);

    private:
        uint32_t *pixels;
        SDL_Renderer *renderer;
        SDL_Window *window;
        SDL_Texture *texture;
        int scanline_counter;

        uint8_t current_mode;
        uint8_t current_line;
        Memory *memory;
        CPU *cpu;

        uint8_t IsLCDEnabled();
        void SetLCDStatus(); /**< Set the status according to memory address 0xFF41 */
        void DrawScanLine();
        void RenderTiles();
        void RenderSprites();
        int GetColor(uint8_t color_id);
        void PrintPixels();
};

#endif