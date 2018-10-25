#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <stdio.h>
#include <cstdint>
#include <string.h>

class Memory {
    private:
        bool booting;
        uint8_t bootstrap[0x100]; /**< 256B of bootstrap memory */
        uint8_t memory[0x10000]; /**< 64KB of memory */
        
    public:
        Memory();
        void ClearBooting(); /**< Set the bootstrap flag to false, used by CPU when PC == 0x100 */ 
        void LoadBootstrap(); /**< Load the bootstrap file and set the booting flag */
        void LoadCartridge(); /**< Load cartridge ROM into memory */
        void WriteByteMemory(uint16_t address, uint8_t data);
        void WriteWordMemory(uint16_t address, uint16_t data);
        uint8_t ReadByteMemory(uint16_t address);
        uint16_t ReadWordMemory(uint16_t address);
        int DumpMemory();


};


#endif