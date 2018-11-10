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
            printf("0x%04x\t0x%02x\t%s", pc, op, opcode_cb_names[ex_op].c_str());
            pc++;   
        }
        printf("0x%04x\t0x%02x\t%s", pc, op, opcode_disassembly[op].disassembled.c_str());
        uint16_t jump = opcode_disassembly[op].jump_offset;
        switch (jump) {
            case 0x00:  pc++; break;
            case 0x01:
            {
                uint8_t n = memory[pc + 1];
                printf("\tn = 0x%02x", n);
                pc += jump;
                break;
            }
            case 0x02:
            {
                uint8_t nn = memory[pc + 2];
                uint8_t n = memory[pc + 1];
                printf("\tnn = 0x%02x%02x", nn, n);
                pc += jump;
                break;  
            }
        }
        puts("");
        /*if (opcode_disassembly[op].jump_offset != 0) {
            pc += opcode_disassembly[op].jump_offset;
        } else {
            pc++;
        }*/
    }
}

