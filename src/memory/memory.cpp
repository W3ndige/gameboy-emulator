#include "memory.hpp"

Memory::Memory(): bootstrap(), memory() {
    booting = true;
}

void Memory::LoadBootstrap() {
    FILE *bootstrap_file = fopen("roms/DMG_ROM.bin", "rb");
    if (bootstrap_file == NULL) {
        booting  = false;
    }
    fread(bootstrap, sizeof(uint8_t), 0x100, bootstrap_file);
    fclose(bootstrap_file);
}

void Memory::LoadCartridge() {
    FILE *game_file = fopen("roms/tetris.gb", "rb");
    //if (bootstrap_file == NULL) {
    //    throw 
    //}
    fread(memory, sizeof(uint8_t), 0x8000, game_file);
    fclose(game_file);
}

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
    // Have to rethink that.
    // else if (address == 0xFF44) { 
    //     memory[address] = 0 ;
    // } 
    // Write to memory
    else {
        memory[address] = data;
    }
}

void Memory::WriteWordMemory(uint16_t address, uint16_t data) {
    memory[address + 1] = data >> 8;
    memory[address] = data & 0xFF;
}

uint8_t Memory::ReadByteMemory(uint16_t address) {
    if (booting == true && address <= 0x00FF) {
        return bootstrap[address];
    }

    return memory[address];
}

uint16_t Memory::ReadWordMemory(uint16_t address) {
    uint16_t word = ReadByteMemory(address + 1);
	word = word << 8;
	word |= ReadByteMemory(address) ;
	return word;
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

void Memory::ClearBooting() {
    booting = false;
}