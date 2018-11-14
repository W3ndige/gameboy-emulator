#include "../cpu.hpp"

/**< 8 Bit Loads */

void CPU::LD8_r_nn(uint8_t &reg) {
    uint8_t n = memory->ReadByteMemory(program_counter);
    reg = n;
    program_counter++;
    clocks.m_cycles += 2;
    clocks.t_cycles += 8;
}

void CPU::LD8_r1_r2(uint8_t &reg1, uint8_t reg2) {
    reg1 = reg2;
    clocks.m_cycles += 1;
    clocks.t_cycles += 4;
}

void CPU::LD8_r1_mem(uint8_t &reg1, uint16_t address) {
    reg1 = memory->ReadByteMemory(address);
    clocks.m_cycles += 1;
    clocks.t_cycles += 8;
}

void CPU::LD8_mem_r1(uint16_t address, uint8_t reg) {
    memory->WriteByteMemory(address, reg);
    clocks.m_cycles += 1;
    clocks.t_cycles += 8;
}

/**< 16 Bit Loads */

void CPU::LD16_r_nn(uint16_t &reg) {
    uint16_t nn = memory->ReadWordMemory(program_counter);
    program_counter += 2;
    reg = nn;
    clocks.m_cycles += 2;
    clocks.t_cycles += 12;

}

void CPU::Push(uint16_t &reg) {
    sp_register.pair -= 2;
    memory->WriteWordMemory(sp_register.pair, reg);
    clocks.m_cycles += 1;
    clocks.t_cycles += 16;
}

void CPU::Pop(uint16_t &reg) {
    reg = memory->ReadWordMemory(sp_register.pair);
    sp_register.pair += 2;
    clocks.m_cycles += 1;
    clocks.t_cycles += 16;
}

/**< When performing a POP AF instruction, bits 0 - 3 are ignored/masked out */
void CPU::PopAF() {
    af_register.pair = memory->ReadWordMemory(sp_register.pair);
    for (size_t i = 0; i < 4; i++) {
        ClearBit(af_register.low, i);
    }
    sp_register.pair += 2;
    clocks.m_cycles += 1;
    clocks.t_cycles += 16; 
}