/**
 * @file gameboy.hpp
 * @author W3ndige (w3ndige@protonmail.com)
 * @brief File containting all of the Gameboy components. 
 * @version 0.1
 * @date 2018-12-07
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <thread>
#include "debugger.hpp"
#include "../gpu/gpu.hpp"
#include "../cpu/cpu.hpp"

const float FPS         = 59.73f;           /**< Number of FPS to emulate */
const int   MAX_CYCLES  = 70224;            /**< Maximum number of cycles */
const float DELAY_TIME  = 1000.0f / FPS;    /**< Time of delay to emulate FPS */

/**
 * @brief Class connecting all of the components of Gameboy. 
 * 
 */

class Gameboy {
    public:

        /**
         * @brief Construct a new Gameboy object.
         * 
         * @param debug Flag to run the debugger. 
         * @param without_boot Flag to run without booting. 
         * @param exit_on_inifite Flag to exit on inifite loop.
         * @param rom_file Name of ROM file to run. 
         */

        Gameboy(bool debug, bool without_boot, bool exit_on_inifite, std::string rom_file);

        /**
         * @brief Main program loop, checks for events, emulates and renders.
         * 
         */

        void Loop();

    private:
        Memory memory;  /**< Memory object */
        CPU cpu;        /**< CPU object */ 
        GPU gpu;        /**< GPU object */
        
        bool debugging;         /**< Flag to check debugger ir ran. */
        friend class Debugger;  /**< Friend class of debugger */
        Debugger debugger;      /**< Debugger object */

        /**
         * @brief Emulate the logic.
         * 
         */

        void Emulate(); 

        /**
         * @brief Set the Key Pressed object.
         * 
         * @param key Number of key that was pressed. 
         */

        void SetKeyPressed(int key);

        /**
         * @brief Set the Key Released object.
         * 
         * @param key Number of key that was released. 
         */

        void SetKeyReleased(int key);

};

#endif