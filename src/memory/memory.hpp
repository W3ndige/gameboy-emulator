#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include "../helper/helper.hpp"

const int MAX_BOOTSTRAP_SIZE = 0x100;
const int MAX_MEMORY_SIZE = 0x10000;

class Memory {
    private:
        bool booting;
        uint8_t bootstrap[MAX_BOOTSTRAP_SIZE]; /**< 256B of bootstrap memory */
        uint8_t memory[MAX_MEMORY_SIZE]; /**< 64KB of memory */
        uint8_t joypad_state;

    private:
        void DMATransfer(uint8_t data);
        uint8_t GetJoypadState(); 
        
    public:
        Memory();
        bool IsBooting(); /**< Return true if in bootstrap */
        bool LoadBootstrap(); /**< Load the bootstrap file and set the booting flag */
        bool LoadCartridge(); /**< Load cartridge ROM into memory */

        void WriteByteMemory(uint16_t address, uint8_t data);
        void PrivilagedByteWrite(uint16_t address, uint8_t data);
        void WriteWordMemory(uint16_t address, uint16_t data);

        uint8_t ReadByteMemory(uint16_t address);
        uint16_t ReadWordMemory(uint16_t address);

        void DumpMemory();


};


#endif