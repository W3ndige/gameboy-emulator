#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>

class Memory {
    private:
        uint8_t memory[0x10000]; /**< 64KB of memory */
        
    public:
        Memory();
        void WriteMemory(uint16_t address, uint8_t data);
        uint8_t ReadMemory(uint16_t address);


};


#endif