/**
 * @file gpu.hpp
 * @author w3ndige (w3ndige@protonmail.com)
 * @brief Graphical Processing Unit of Gameboy.
 * @version 0.1
 * @date 2018-12-06
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef GPU_HPP
#define GPU_HPP

#include <memory>
#include <SDL2/SDL.h>

#include "../memory/memory.hpp"
#include "../helper/helper.hpp"
#include "../cpu/cpu.hpp"

const int SCREEN_HEIGH = 320; /**< Height of screen */
const int SCREEN_WIDTH = 288; /**< Width of screen */

/**
 * @brief Class containing logic of GPU. 
 * 
 */

class GPU {
    public:

        /**
         * @brief Construct a new GPU object.
         * 
         * @param mem Pointer to Memory object.
         * @param cpu Pointer to CPU object.
         */

        GPU(Memory *mem, CPU *cpu);

        /**
         * @brief Destroy the GPU object.
         * 
         */

        ~GPU();

        /**
         * @brief Initialize the values in GPU.
         * 
         * @return true Initialization succeed.
         * @return false Initialization failed.
         */

        bool Init();

        /**
         * @brief Update the GPU during every cycle of emulator.
         * 
         * @param cycles Value of cycles that were 
         * used by last instruction in CPU. 
         */

        void UpdateGraphics(int cycles);

        /**
         * @brief Render graphics to screen with SDL2. 
         * 
         */

        void RenderGraphics();

    private:
        uint32_t *pixels; /**< Pointer to an array of pixels */
        std::unique_ptr<SDL_Window, 
                        decltype(&SDL_DestroyWindow)> window; /**< Pointer to SDL_Window */
        std::unique_ptr<SDL_Renderer, 
                        decltype(&SDL_DestroyRenderer)> renderer; /**< Pointer to SDL_Renderer */
        std::unique_ptr<SDL_Texture, 
                        decltype(&SDL_DestroyTexture)> texture; /**< Pointer to SDL_Texture */

        int scanline_counter;   /**< Counter, when to move to the next line */
        uint8_t current_mode;   /**< Mode in which GPU is working. */
        uint8_t current_line;   /**< Current line being drawed. */
        Memory *memory;         /**< Pointer to memory object. */
        CPU *cpu;               /**< Pointer to CPU object */
        
        /**
         * @brief Returns 0 if the LCD is enabled or 0 if is not.
         * 
         * @return int Value of 7-th bit of LCD control register.
         */

        int IsLCDEnabled();

        /**
         * @brief  Set the status according to memory address 0xFF41
         * 
         */

        void SetLCDStatus();

        /**
         * @brief Decide between drawing background or
         * drawing sprites.
         * 
         */

        void DrawScanLine();

        /**
         * @brief Draw background according to ROM into the pixels array.
         * 
         */

        void RenderTiles();

        /**
         * @brief Draw sprites according to ROM into the pixels array. 
         * 
         */

        void RenderSprites();

        /**
         * @brief Get the color object according to the palette.
         * 
         * @param color_id Color ID to get. 
         * @param addrress Address of the colour palette.
         * @return int Return color.
         */

        int GetColor(uint8_t color_id, uint16_t addrress);
};

#endif