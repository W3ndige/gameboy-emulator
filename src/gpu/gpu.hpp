#ifndef GPU_HPP
#define GPU_HPP

#include <memory>
#include <SDL2/SDL.h>

#include "../memory/memory.hpp"
#include "../helper/helper.hpp"
#include "../cpu/cpu.hpp"

const int SCREEN_HEIGH = 320;
const int SCREEN_WIDTH = 288;

class GPU {
    public:
        GPU(Memory *mem, CPU *cpu);
        ~GPU();
        bool Init();
        void UpdateGraphics(int cycles);

    private:
        uint32_t *pixels;
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture;

        //SDL_Renderer *renderer;
        //SDL_Window *window;
        //SDL_Texture *texture;
        
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
        int GetColor(uint8_t color_id, uint16_t addrress);
        void PrintPixels();
};

#endif