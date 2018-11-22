#include "memory.hpp"

Memory::Memory(): memory() {
    booting = true;
    joypad_state = 0xFF;
}

void Memory::DMATransfer(uint8_t data) {
    uint16_t address = data << 8;

    /**< Read sprite RAM between memory adddress 0xFE00-0xFE9F */
    for (size_t i = 0 ; i < 0xA0; i++) {
        WriteByteMemory(0xFE00 + i, ReadByteMemory(address + i));
    }
}

uint8_t Memory::GetJoypadState() {
    uint8_t res = memory[0xFF00];
    res ^= 0xFF;

    if (!TestBit(res, 4)) {
        uint8_t top_joypad = joypad_state >> 4;
        top_joypad |= 0xF0;
        res &= top_joypad;
    } else if (!TestBit(res, 5)) {
        uint8_t bottom_joypad = joypad_state & 0xF;
        bottom_joypad |= 0xF0;
        res &= bottom_joypad;
   }
   return res;

}

void Memory::DisableBooting() {
    booting = false;
}

bool Memory::IsBooting() {
    return booting;
}

bool Memory::LoadCartridge(std::string rom_file) {
    try {
        std::ifstream game_file (rom_file, std::ifstream::binary);
        if (game_file.good()) {
            game_file.read((char *)memory, 0x8000);
            LoadCartridgeHeader();
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

void Memory::LoadCartridgeHeader() {
    std::string tmp_title;
    for (size_t i = 0; i < 15; i++) {
        tmp_title += char(memory[0x134 + i]);
    }
    cartridge_header.title = tmp_title;
    cartridge_header.cartridge_type = memory[0x147];
    cartridge_header.rom_size = memory[0x148];
    cartridge_header.ram_size = memory[0x149];
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
    /**< If game wants to write to current
     *   scanline register, reset. */
    else if (address == 0xFF44) { 
        memory[address] = 0;
    }
    else if (address == 0xFF46) {
        DMATransfer(data);
    }
    /**< Set booting to false if program 
     *   wants to write to this memory */
    else if (address == 0xFF50) {
        booting = false;
        return;
    }
    /**< Serial data used for Blargg test output */
    else if (address == 0xFF02 && data == 0x81) {
        std::cout << char(ReadByteMemory(0xFF01));
    }
    /**< Write to memory */
    else {
        memory[address] = data;
    }
}

void Memory::PrivilagedByteWrite(uint16_t address, uint8_t data) {
    /**< Some system functions have to write beyond limits */
    memory[address] = data;
}

void Memory::WriteWordMemory(uint16_t address, uint16_t data) {
    memory[address + 1] = data >> 8;
    memory[address] = data & 0xFF;
}

uint8_t Memory::ReadByteMemory(uint16_t address) {
    /**< If booting we want to read from bootstrap memory */
    if (booting == true && address <= 0x00FF) {
        return bootstrap[address];
    }

    /**< Get the state of the joypad */ 
    else if (address == 0xFF00) {
        return GetJoypadState() ;
    }

    return memory[address];
}

uint8_t Memory::PrivilagedReadByteMemory(uint16_t address) {
    return memory[address];
}

uint16_t Memory::ReadWordMemory(uint16_t address) {
    /**< Read word in little endian format */
    uint16_t word = ReadByteMemory(address + 1);
	word = word << 8;
	word |= ReadByteMemory(address) ;
	return word;
}

void Memory::DumpMemory() {
    try {
        std::ofstream dump ("memory_dump.bin", std::ofstream::binary);
        if (dump.good()) {
            dump.write((char *)memory, 0x10000);
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