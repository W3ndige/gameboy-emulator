#include "cpu.hpp"

CPU::CPU() {
    /**< Initialize registers */
    program_counter = 0x100;

    af_register.pair = 0x01B0;
    bc_register.pair = 0x0013;
    de_register.pair = 0x00D8; 
    hl_register.pair = 0x014D;

    stack_pointer.pair = 0xFFFE;

    timer.m_cycles = 0;
    timer.t_cycles = 0;

    /**< Place power up sequence values in memory */
    memory.WriteMemory(0xFF05, 0x00);
    memory.WriteMemory(0xFF06, 0x00);
    memory.WriteMemory(0xFF07, 0x00);
    memory.WriteMemory(0xFF10, 0x80);
    memory.WriteMemory(0xFF11, 0xBF);
    memory.WriteMemory(0xFF12, 0xF3);
    memory.WriteMemory(0xFF14, 0xBF);
    memory.WriteMemory(0xFF16, 0x3F);
    memory.WriteMemory(0xFF17, 0x00);
    memory.WriteMemory(0xFF19, 0xBF);
    memory.WriteMemory(0xFF1A, 0x7F);
    memory.WriteMemory(0xFF1B, 0xFF);
    memory.WriteMemory(0xFF1C, 0x9F);
    memory.WriteMemory(0xFF1E, 0xBF);
    memory.WriteMemory(0xFF20, 0xFF);
    memory.WriteMemory(0xFF21, 0x00);
    memory.WriteMemory(0xFF22, 0x00);
    memory.WriteMemory(0xFF23, 0xBF);
    memory.WriteMemory(0xFF24, 0x77);
    memory.WriteMemory(0xFF25, 0xF3);
    memory.WriteMemory(0xFF26, 0xF1);
    memory.WriteMemory(0xFF40, 0x91);
    memory.WriteMemory(0xFF42, 0x00);
    memory.WriteMemory(0xFF43, 0x00);
    memory.WriteMemory(0xFF45, 0x00);
    memory.WriteMemory(0xFF47, 0xFC);
    memory.WriteMemory(0xFF48, 0xFF);
    memory.WriteMemory(0xFF49, 0xFF);
    memory.WriteMemory(0xFF4A, 0x00);
    memory.WriteMemory(0xFF4B, 0x00);
    memory.WriteMemory(0xFFFF, 0x00); 
}

void CPU::ExecuteInstruction() {
    uint8_t opcode = memory.ReadMemory(program_counter);
    printf("OPCODE\t0x%02x\n", opcode);
    switch (opcode) {
        case 0x00:
            NOP();
    }
    program_counter++;
}

void CPU::Diagnostics() {
    printf("PC\t0x%02x\n", program_counter);
    printf("SP\t0x%02x\n", stack_pointer.pair);
    printf("A\t0x%02x\n", af_register.high);
    printf("B\t0x%02x\n", bc_register.high);
    printf("C\t0x%02x\n", bc_register.low);
    printf("D\t0x%02x\n", de_register.high);
    printf("E\t0x%02x\n", de_register.low);
    printf("H\t0x%02x\n", hl_register.high);
    printf("L\t0x%02x\n", hl_register.low);
    printf("TIMER M\t0x%02x\n", timer.m_cycles);
    printf("TIMER T\t0x%02x\n", timer.t_cycles);
}

/*  
    0x00 (NOP)
    No operation
*/

void CPU::NOP() {
    timer.m_cycles += 1;
    timer.t_cycles += 4;
}