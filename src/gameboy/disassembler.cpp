#include "disassembler.hpp"

bool Disassembler::Init(std::string rom_file) {
    try {
        std::ifstream game_file (rom_file, std::ifstream::binary);
        if (game_file.good()) {
            game_file.read((char *)memory, 0x8000);
        }
        else {
            throw "Error opening cartridge ROM. Unable to continue.";
        }
        game_file.close();
    }
    catch (const char *e) {
        printf("%s\n", e);
        return false;
    }
    
    return true;
}

void Disassembler::Disassemble() {
    uint16_t pc = 0x100;
    while (pc < 0x8000) {
        uint8_t op = memory[pc];
        if (op == 0xcb) {
            uint8_t ex_op = memory[pc + 1];
            printf("0x%04x\t0x%02x\t%s\n", pc, op, opcode_cb_names[ex_op].c_str());
            pc++;   
        }
        printf("0x%04x\t0x%02x\t%s\n", pc, op, opcode_names[op].disassembled.c_str());
        if (opcode_names[op].jump_offset != 0) {
            pc += opcode_names[op].jump_offset;
        } else {
            pc++;
        }
    }
}

