#include "memory.hpp"

Memory::Memory(): bootstrap(), memory() {
    booting = true;
    joypad_state = 0xFF;
}

void Memory::DMATransfer(uint8_t data) {
    uint16_t address = data << 8;

    /**< Read sprite RAM between memory adddress 0xFE00-0xFE9F */
    for (int i = 0 ; i < 0xA0; i++) {
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
    }
    else if (!TestBit(res, 5)) {
        uint8_t bottom_joypad = joypad_state & 0xF;
        bottom_joypad |= 0xF0;
        res &= bottom_joypad;
   }
   return res;

}

bool Memory::IsBooting() {
    return booting;
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
        std::ifstream game_file ("roms/flappyboy.gb", std::ifstream::binary);
        //std::ifstream game_file ("roms/individual/11-op a,(hl).gb", std::ifstream::binary);
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
        tmp_title += char(memory[0x104 + i]);
    }
    cartridge_header.title = tmp_title;
    cartridge_header.cartridge_type = memory[0x147];
    cartridge_header.rom_size = memory[0x148];
    cartridge_header.ram_size = memory[0x149];
}

void Memory::PrintCartidgeHeader() {
    printf("TITLE\t%s\n", cartridge_header.title.c_str());
    printf("CARTRIDGE TYPE\t");
    switch (cartridge_header.cartridge_type) {
        case 0x00: puts("ROM ONLY"); break;
        case 0x01: puts("MBC1"); break;
        case 0x02: puts("MBC1+RAM"); break;
        case 0x03: puts("MBC1+RAM+BATTERY"); break;
        case 0x05: puts("MBC2"); break;
        case 0x06: puts("MBC2+BATTERY"); break;
        case 0x08: puts("ROM+RAM"); break;
        case 0x09: puts("ROM+RAM+BATTERY"); break;
        case 0x0B: puts("MMM01"); break;
        case 0x0C: puts("MMM01+RAM"); break;
        case 0x0D: puts("MMM01+RAM+BATTERY"); break;
        case 0x0F: puts("MBC3+TIMER+BATTERY"); break;
        case 0x10: puts("MBC3+TIMER+RAM+BATTERY"); break;
        case 0x11: puts("MBC3"); break;
        case 0x12: puts("MBC3+RAM)"); break;
        case 0x13: puts("MBC3+RAM+BATTERY"); break;
        case 0x19: puts("MBC5"); break;
        case 0x1A: puts("MBC5+RAM"); break;
        case 0x1B: puts("MBC5+RAM+BATTERY"); break;
        case 0x1C: puts("MBC5+RUMBLE"); break;
        case 0x1D: puts("MBC5+RUMBLE+RAM"); break;
        case 0x1E: puts("MBC5+RUMBLE+RAM+BATTERY"); break;
        case 0x20: puts("MBC6"); break;
        case 0x22: puts("MBC7+SENSOR+RUMBLE+RAM+BATTERY"); break;
        case 0xFC: puts("POCKET CAMERA"); break;
        case 0xFD: puts("BANDAI TAMA5"); break;
        case 0xFE: puts("HuC3"); break;
        case 0xFF: puts("HuC1+RAM+BATTERY"); break;
    }

    printf("ROM SIZE\t");
    switch (cartridge_header.rom_size) {
        case 0x00: puts("32KByte (no ROM banking)"); break;
        case 0x01: puts("64KByte (4 banks)"); break;
        case 0x02: puts("128KByte (8 banks)"); break;
        case 0x03: puts("256KByte (16 banks)"); break;
        case 0x04: puts("512KByte (32 banks)"); break;
        case 0x05: puts("1MByte (64 banks)"); break;
        case 0x06: puts("2MByte (128 banks)"); break;
        case 0x07: puts("4MByte (256 banks)"); break;
        case 0x08: puts("8MByte (512 banks)"); break;
        case 0x52: puts("1.1MByte (72 banks)"); break;
        case 0x53: puts("1.2MByte (80 banks)"); break;
        case 0x54: puts("1.5MByte (96 banks)"); break;
    }

    printf("RAM SIZE\t");
    switch (cartridge_header.ram_size) {
        case 0x00: puts("None"); break;
        case 0x01: puts("2 KBytes"); break;
        case 0x02: puts("8 Kbytes"); break;
        case 0x03: puts("32 KBytes"); break;
        case 0x04: puts("128 KBytes"); break;
        case 0x05: puts("64 KBytes"); break;
    }
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
    else if (address == 0xFF44) { 
        memory[address] = 0;
    }
    else if (address == 0xFF46) {
        DMATransfer(data);
    }
    /**< Set booting to false if program wants to write to this memory */
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