/**
 * @file memory.hpp
 * @author W3ndige (w3ndige@protonmail.com)
 * @brief File containing MMU logic.
 * @version 0.1
 * @date 2018-12-06
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include "../helper/helper.hpp"

const int MAX_BOOTSTRAP_SIZE = 0x100;   /**< Maximum size of bootstrap array */
const int MAX_MEMORY_SIZE = 0x10000;    /**< Maximum size of memory array */

/**
 * @brief Structure contains 
 * information about cartridge. 
 */

struct CartidgeHeader {
    std::string title;      /**< Maximum 16 characters */
    uint8_t cartridge_type; /**< Specifies which MBC is used in the cartridge */
    uint8_t rom_size;       /**< Specifies size of the ROM */
    uint8_t ram_size;       /**< Specifies size of the RAM, if any */
};

/**
 * @brief Class contains memory 
 * management unit logic.
 */

class Memory {
    private:
        bool booting; /**< State of booting the Gameboy */ 

        uint8_t bootstrap[MAX_BOOTSTRAP_SIZE] = { 
            0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB,
            0x21, 0x26, 0xFF, 0x0E, 0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3,
            0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0, 0x47, 0x11, 0x04, 0x01,
            0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
            0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22,
            0x23, 0x05, 0x20, 0xF9, 0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99,
            0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20, 0xF9, 0x2E, 0x0F, 0x18,
            0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
            0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20,
            0xF7, 0x1D, 0x20, 0xF2, 0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62,
            0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06, 0x7B, 0xE2, 0x0C, 0x3E,
            0x87, 0xE2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
            0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17,
            0xC1, 0xCB, 0x11, 0x17, 0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9,
            0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83,
            0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
            0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63,
            0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,
            0x3C, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x3C, 0x21, 0x04, 0x01, 0x11,
            0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
            0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE,
            0x3E, 0x01, 0xE0, 0x50};        /**< 256B of bootstrap memory */

        uint8_t memory[MAX_MEMORY_SIZE];    /**< 64KB of memory */

    private:
        
        /**
         * @brief Read sprite RAM between memory adddress 0xFE00-0xFE9F
         * 
         * @param data Bit shifted address from where sprite data is read.
         */

        void DMATransfer(uint8_t data);

        /**
         * @brief Get the Joypad State object
         * 
         * @return uint8_t State of joypad.
         */

        uint8_t GetJoypadState(); 
        
    public:
        uint8_t joypad_state;               /**< State of the joypad */
        CartidgeHeader cartridge_header;    /**< Contains information about 
                                                 the cartridge header */

        /**
         * @brief Construct a new Memory object
         * 
         */

        Memory();
        
        /**
         * @brief Set booting flag to false.
         * 
         */

        void DisableBooting();

        /**
         * @brief Return true if booting, otherwise false.
         * 
         * @return true Gameboy is booting.
         * @return false Gameboy is not booting.
         */

        bool IsBooting();

        /**
         * @brief Load cartridge ROM into memory, return true if loaded, 
         * false if exception occured.
         * 
         * @param rom_file Name of the ROM file to load.
         * @return true Loading worked. 
         * @return false Exception in loading occured. 
         */

        bool LoadCartridge(std::string rom_file);

        /**
         * @brief Load values from cartridge into 
         * the CartidgeHeader structure.
         * 
         */

        void LoadCartridgeHeader();

        /**
         * @brief Set the byte of memory array at 
         * address to value of data, while 
         * checking conditions.
         * 
         * 
         * @param address Offset in memory
         * @param data Byte of data value to write
         */

        void WriteByteMemory(uint16_t address, uint8_t data);

        /**
         * @brief Set the byte of memory array at 
         * address to value of data, without 
         * checking conditions.
         * 
         * @param address Offset in memory
         * @param data Byte of data to write
         */

        void PrivilagedByteWrite(uint16_t address, uint8_t data);

        /**
         * @brief Set the word of memory at address, 
         * and address + 1, to the word of data, 
         * while checking conditions
         * 
         * @param address Offset in memory
         * @param data Word of data to write
         */

        void WriteWordMemory(uint16_t address, uint16_t data);

        /**
         * @brief Read the byte from memory, or set
         * the state of joypad if address is 0xFF00.
         * 
         * @param address Offset in memory
         * @return uint8_t Byte of data from memory.
         */

        uint8_t ReadByteMemory(uint16_t address);

        /**
         * @brief Read the byte from memory, without 
         * setting joypad state. 
         * 
         * @param address offset in memory.
         * @return uint8_t Byte of data from memory.
         */

        uint8_t PrivilagedReadByteMemory(uint16_t address);

        /**
         * @brief Read the word from memory.
         * 
         * @param address offset in memory.
         * @return uint16_t Word of data from memory.
         */
        
        uint16_t ReadWordMemory(uint16_t address);

        /**
         * @brief Load contents of memory to the file. 
         * 
         */

        void DumpMemory();
};


#endif