#ifndef DISASSEMBLER_HPP
#define DISASSEMBLER_HPP

#include <iomanip>
#include <string>
#include <fstream>
#include <cstdint>

struct OpcodeInfo {
    std::string disassembled;
    uint16_t jump_offset;
};

class Disassembler {
    public:
        bool Init(std::string rom_file);
        void Disassemble();

    private:
        uint8_t memory[0x8000];
};

#endif