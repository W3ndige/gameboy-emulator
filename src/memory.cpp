#include "memory.hpp"

Memory::Memory(): memory() {}

void Memory::WriteByteMemory(uint16_t address, uint8_t data) {
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
        WriteByteMemory(address - 0x2000, data);
    }
    // Write to memory
    else {
        memory[address] = data;
    }
}

void Memory::WriteWordMemory(uint16_t address, uint16_t data) {
    memory[address + 1] = data >> 8;
    memory[address] = data & 0xFF;
}


int Memory::WriteChunkMemory(uint16_t offset, uint16_t size, uint8_t *data) {
    if ((memory + offset + size) <= (memory + sizeof(memory))) {
        memcpy(memory + offset, data, size);
        return 0;
    }
    return -1;
}

uint8_t Memory::ReadByteMemory(uint16_t address) {
    return memory[address];
}

uint16_t Memory::ReadWordMemory(uint16_t address) {
    uint16_t word = ReadByteMemory(address + 1);
	word = word << 8;
	word |= ReadByteMemory(address) ;
	return word ;
}

int Memory::DumpMemory() {
    FILE *dump = fopen("memory_dump.bin", "wb");
    if (dump == NULL) {
        return -1;
    }
	fwrite(memory, sizeof(uint8_t), 0x10000, dump);
    fclose(dump);
    return 0;
}