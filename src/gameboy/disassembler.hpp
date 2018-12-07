/**
 * @file disassembler.hpp
 * @author w3ndige (w3ndige@protonmail.com)
 * @brief File containt logic of the disassembler. 
 * @version 0.1
 * @date 2018-12-07
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef DISASSEMBLER_HPP
#define DISASSEMBLER_HPP

#include <iomanip>
#include <string>
#include <fstream>
#include <cstdint>

/**
 * @brief Structure containing information about the opcode. 
 * 
 */

struct OpcodeInfo {
    std::string disassembled;   /**< Disassembled string */
    uint16_t jump_offset;       /**< Value to increment PC */
};

/**
 * @brief Class containing disassembler logic.
 * 
 */

class Disassembler {
    public:

        /**
         * @brief Initialize the disassembler.
         * 
         * @param rom_file Name of the ROM to disassemble.
         * @return true Initialization sucedeed.
         * @return false Iniitalization failed. 
         */

        bool Init(std::string rom_file);

        /**
         * @brief Disassemble to ROM file. 
         * 
         */

        void Disassemble();

    private:
        uint8_t memory[0x8000]; /**< Memory array */
};

#endif