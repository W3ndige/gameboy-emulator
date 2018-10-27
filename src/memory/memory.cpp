#include "memory.hpp"

Memory::Memory(): bootstrap(), memory() {
    booting = true;
}

void Memory::DMATransfer(uint8_t data) {
   uint16_t address = data << 8;
   for (int i = 0 ; i < 0xA0; i++) { /**< Read sprite RAM between memory adddress 0xFE00-0xFE9F */
     WriteByteMemory(0xFE00 + i, ReadByteMemory(address + i));
   }
}

bool Memory::IsBooting() {
    return booting;
}

void Memory::ClearBooting() {
    booting = false;
}

bool Memory::LoadBootstrap() {
    try {
        std::ifstream bootstrap_file ("roms/DMG_ROM.bin", std::ifstream::binary);
        if (bootstrap_file.good()) {
            bootstrap_file.read((char *)bootstrap, MAX_BOOTSTRAP_SIZE);
        }
        else {
            booting = false;
            throw "Error opening ROM. Skipping boot procedure.";
        }
        bootstrap_file.close();
    }
    catch (const char *e) {
        std::cout << "Exception occured: " << e << std::endl;
        return false;
    }

    return true;
}

bool Memory::LoadCartridge() {
    try {
        std::ifstream game_file ("roms/tetris.gb", std::ifstream::binary);
        //std::ifstream game_file ("roms/cpu_instrs.gb", std::ifstream::binary);
        if (game_file.good()) {
            game_file.read((char *)memory, 0x8000);
        }
        else {
            booting = false;
            throw "Error opening cartridge ROM. Unable to continue.";
        }
        game_file.close();
    }
    catch (const char *e) {
        std::cout << "Exception occured: " << e << std::endl;
        return false;
    }
    
    return true;
}

void Memory::WriteByteMemory(uint16_t address, uint8_t data) {

    /**<  Read Only Memory */
    if (address < 0x8000) {
        return;
    }
    /**< Restricted memory */
    else if (address >= 0xFEA0 && address <= 0xFEFF) {
        return;
    }
    /**< ECHO memory */
    else if (address >= 0xE000 && address <= 0xFDFF) {
        memory[address] = data;
        WriteByteMemory(address - 0x2000, data);
    }
    /**< If game wants to write to divider register, reset. */
    else if (address == 0xFF04) {
        memory[address] = 0;
    } 
    // Have to rethink that.
    // else if (address == 0xFF44) { 
    //     memory[address] = 0 ;
    // }
    else if (address == 0xFF46) {
        DMATransfer(data);
    } 
    /**< Write to memory */
    else {
        memory[address] = data;
    }
}

void Memory::PrivilagedByteWrite(uint16_t address, uint8_t data) {
    memory[address] = data;
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

void Memory::DumpMemory() {
    try {
        std::ofstream dump ("memory_dump.bin", std::ofstream::binary);
        if (dump.good()) {
            dump.write((char *)memory, 0x8000);
        }
        else {
            throw "Could not dump memory.";
        }
        dump.close();
    }
    catch (const char *e) {
        std::cout << "Exception occured: " << e << std::endl;
    }
}