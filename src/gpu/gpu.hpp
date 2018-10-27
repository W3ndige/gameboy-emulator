#ifndef GPU_HPP
#define GPU_HPP

#include "../memory/memory.hpp"
#include "../helper/helper.hpp"
#include "../cpu/cpu.hpp"
#include "../gui/gui.hpp"

class GPU {
    public:
        GPU(Memory *mem, CPU *cpu);
        void UpdateGraphics(int cycles);

    private:
        uint8_t pixels[160][144];
        int scanline_counter;

        uint8_t current_mode;
        uint8_t current_line;
        Memory *memory;
        CPU *cpu;
        GUI gui;

        uint8_t IsLCDEnabled();
        void SetLCDStatus(); /**< Set the status according to memory address 0xFF41 */
        void DrawScanLine();
        void RenderTiles();
        void RenderSprites();
        void PrintPixels();
};

#endif