#include "memory.hpp"

Memory::Memory(): memory() {}

void Memory::WriteMemory(uint16_t address, uint8_t data) {
    //  Read Only Memory
    if (address < 0x8000) {
        return;
    }
    // Restricted memory
    else if (address >= 0xFEA0 && address <= 0xFEFF) {
        return;
    }
    // ECHO memory
    else if (address >= 0xE000 && address <= 0xFDFF) {
        memory[address] = data;
        WriteMemory(address - 0x2000, data);
    }
    // Write to memory
    else {
        memory[address] = data;
    }
}

uint8_t Memory::ReadMemory(uint16_t address) {
    return memory[address];
}