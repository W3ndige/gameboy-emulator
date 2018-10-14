#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <stdio.h>
#include <cstdint>
#include <string.h>

class Memory {
    private:
        uint8_t memory[0x10000]; /**< 64KB of memory */
        
    public:
        Memory();
        void WriteMemory(uint16_t address, uint8_t data);
        int WriteMemoryChunk(uint16_t offset, uint16_t size, uint8_t *data);
        uint8_t ReadByteMemory(uint16_t address);
        uint16_t ReadWordMemory(uint16_t address);
        int DumpMemory();


};


#endif