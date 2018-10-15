#include "cpu.hpp"

CPU::CPU() {
    /**< Initialize registers */
    program_counter = 0x00;//0x100;

    af_register.pair = 0x01B0;
    bc_register.pair = 0x0013;
    de_register.pair = 0x00D8; 
    hl_register.pair = 0x014D;

    sp_register.pair = 0xFFFE;

    timer.m_cycles = 0;
    timer.t_cycles = 0;
    halt = 0;
    stop = 0;

    /**< Read the bootstrap ROM into memory */
    FILE *bootstrap_file = fopen("bin/DMG_ROM.bin", "rb");
    //if (bootstrap_file == NULL) {
    //    throw 
    //}
    uint8_t bootstrap_data[0x100]; // 256 bytes of bootstrap file
    fread(bootstrap_data, sizeof(uint8_t), 0x100, bootstrap_file);
    memory.WriteMemoryChunk(0, 0x100, bootstrap_data);
    fclose(bootstrap_file);

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

    memory.DumpMemory();
}

void CPU::ExecuteInstruction(uint8_t opcode) {
    switch (opcode) {
        case 0x00: NOP(); break;
        case 0x76: HALT(); break;
        //case 0x1000: STOP(); break;
        case 0xF3: DI(); break;
        case 0xFB: EI(); break;
        
        /**< Load 8 bit immediate value */
        case 0x06: LD8_r_nn(bc_register.high); break;
        case 0x0E: LD8_r_nn(bc_register.low); break;
        case 0x16: LD8_r_nn(de_register.high); break;
        case 0x1E: LD8_r_nn(de_register.low); break;
        case 0x26: LD8_r_nn(hl_register.high); break;
        case 0x2E: LD8_r_nn(hl_register.low); break;

        /**< Load register to register */
        case 0x7F: LD8_r1_r2(af_register.high, af_register.high); break;
        case 0x78: LD8_r1_r2(af_register.high, bc_register.high); break;
        case 0x79: LD8_r1_r2(af_register.high, bc_register.low); break;
        case 0x7A: LD8_r1_r2(af_register.high, de_register.high); break;
        case 0x7B: LD8_r1_r2(af_register.high, de_register.low); break;
        case 0x7C: LD8_r1_r2(af_register.high, hl_register.high); break;
        case 0x7D: LD8_r1_r2(af_register.high, hl_register.low); break;
        case 0x40: LD8_r1_r2(bc_register.high, bc_register.high); break;
        case 0x41: LD8_r1_r2(bc_register.high, bc_register.low); break;
        case 0x42: LD8_r1_r2(bc_register.high, de_register.high); break;
        case 0x43: LD8_r1_r2(bc_register.high, de_register.low); break;
        case 0x44: LD8_r1_r2(bc_register.high, hl_register.high); break;
        case 0x45: LD8_r1_r2(bc_register.high, hl_register.low); break;
        case 0x48: LD8_r1_r2(bc_register.low, bc_register.high); break;
        case 0x49: LD8_r1_r2(bc_register.low, bc_register.low); break;
        case 0x4A: LD8_r1_r2(bc_register.low, de_register.high); break;
        case 0x4B: LD8_r1_r2(bc_register.low, de_register.low); break;
        case 0x4C: LD8_r1_r2(bc_register.low, hl_register.high); break;
        case 0x4D: LD8_r1_r2(bc_register.low, hl_register.low); break;
        case 0x50: LD8_r1_r2(de_register.high, bc_register.high); break;
        case 0x51: LD8_r1_r2(de_register.high, bc_register.low); break;
        case 0x52: LD8_r1_r2(de_register.high, de_register.high); break;
        case 0x53: LD8_r1_r2(de_register.high, de_register.low); break;
        case 0x54: LD8_r1_r2(de_register.high, hl_register.high); break;
        case 0x55: LD8_r1_r2(de_register.high, hl_register.low); break;
        case 0x58: LD8_r1_r2(de_register.low, bc_register.high); break;
        case 0x59: LD8_r1_r2(de_register.low, bc_register.low); break;
        case 0x5A: LD8_r1_r2(de_register.low, de_register.high); break;
        case 0x5B: LD8_r1_r2(de_register.low, de_register.low); break;
        case 0x5C: LD8_r1_r2(de_register.low, hl_register.high); break;
        case 0x5D: LD8_r1_r2(de_register.low, hl_register.low); break;
        case 0x60: LD8_r1_r2(hl_register.high, bc_register.high); break;
        case 0x61: LD8_r1_r2(hl_register.high, bc_register.low); break;
        case 0x62: LD8_r1_r2(hl_register.high, de_register.high); break;
        case 0x63: LD8_r1_r2(hl_register.high, de_register.low); break;
        case 0x64: LD8_r1_r2(hl_register.high, hl_register.high); break;
        case 0x65: LD8_r1_r2(hl_register.high, hl_register.low); break;
        case 0x68: LD8_r1_r2(hl_register.low, bc_register.high); break;
        case 0x69: LD8_r1_r2(hl_register.low, bc_register.low); break;
        case 0x6A: LD8_r1_r2(hl_register.low, de_register.high); break;
        case 0x6B: LD8_r1_r2(hl_register.low, de_register.low); break;
        case 0x6C: LD8_r1_r2(hl_register.low, hl_register.high); break;
        case 0x6D: LD8_r1_r2(hl_register.low, hl_register.low); break;

        case 0x47: LD8_r1_r2(bc_register.high, af_register.high); break;
        case 0x4F: LD8_r1_r2(bc_register.low, af_register.high); break;
        case 0x57: LD8_r1_r2(de_register.high, af_register.high); break;
        case 0x5F: LD8_r1_r2(de_register.low, af_register.high); break;
        case 0x67: LD8_r1_r2(hl_register.high, af_register.high); break;
        case 0x6F: LD8_r1_r2(hl_register.low, af_register.high); break;

        case 0x7E: LD8_r1_mem(af_register.high, hl_register.pair); break;
        case 0x46: LD8_r1_mem(bc_register.high, hl_register.pair); break;
        case 0x4E: LD8_r1_mem(bc_register.low, hl_register.pair); break;
        case 0x56: LD8_r1_mem(de_register.high, hl_register.pair); break;
        case 0x5E: LD8_r1_mem(de_register.low, hl_register.pair); break;
        case 0x66: LD8_r1_mem(hl_register.high, hl_register.pair); break;
        case 0x6E: LD8_r1_mem(hl_register.low, hl_register.pair); break;

        case 0x70: LD8_mem_r1(hl_register.pair, bc_register.high); break;
        case 0x71: LD8_mem_r1(hl_register.pair, bc_register.low); break;
        case 0x72: LD8_mem_r1(hl_register.pair, de_register.high); break;
        case 0x73: LD8_mem_r1(hl_register.pair, de_register.low); break;
        case 0x74: LD8_mem_r1(hl_register.pair, hl_register.high); break;
        case 0x75: LD8_mem_r1(hl_register.pair, hl_register.low); break;

        case 0x36: program_counter++; LD8_mem_r1(hl_register.pair, 
                    memory.ReadByteMemory(program_counter)); timer.t_cycles += 4; break;

        case 0x0A: LD8_r1_mem(af_register.high, bc_register.pair); break;
        case 0x1A: LD8_r1_mem(af_register.high, de_register.pair); break;

        case 0xFA: program_counter++; LD8_r1_mem(af_register.high, 
                    memory.ReadWordMemory(program_counter)); timer.t_cycles += 8; break;

        /*
            LD           A,#             3E         8 
        */

        case 0x02: LD8_mem_r1(bc_register.pair, af_register.high); break;
        case 0x12: LD8_mem_r1(de_register.pair, af_register.high); break;
        case 0x77: LD8_mem_r1(hl_register.pair, af_register.high); break;
        
        //   LD           (nn),A          EA        16
       
        case 0XF2: LD8_r1_mem(af_register.high, 0xFF00 + bc_register.low); break;
        case 0XE2: LD8_mem_r1(0xFF00 + bc_register.low, af_register.high); break;

        case 0x3A: LD8_r1_mem(af_register.high, hl_register.pair); hl_register.pair--; break;
        case 0X32: LD8_mem_r1(hl_register.pair, af_register.high); hl_register.pair--; break;
        case 0x2A: LD8_r1_mem(af_register.high, hl_register.pair); hl_register.pair++; break;
        case 0X22: LD8_mem_r1(hl_register.pair, af_register.high); hl_register.pair++; break;

        case 0xE0: program_counter++; LD8_mem_r1(0xFF00 + memory.ReadByteMemory(program_counter), 
                    af_register.high); timer.t_cycles += 8; break;
        case 0xF0: program_counter++; LD8_r1_mem(af_register.high, 
                    0xFF00 + memory.ReadByteMemory(program_counter)); timer.t_cycles += 8; break;

       /**< Load 16 bit immediate value */
        case 0x01: LD16_r_nn(bc_register.pair); break;
        case 0x11: LD16_r_nn(de_register.pair); break;
        case 0x21: LD16_r_nn(hl_register.pair); break;
        case 0x31: LD16_r_nn(sp_register.pair); break;
        case 0xF9: sp_register.pair = hl_register.pair; timer.t_cycles += 8; break;

        default: printf("Unkown opcode: 0x%02x\n", opcode);
    }   
}

void CPU::FetchAndDispatch() {
    uint8_t opcode = memory.ReadByteMemory(program_counter);
    program_counter++;
    ExecuteInstruction(opcode);
}

void CPU::Diagnostics() {
    printf("PC\t0x%02x\n", program_counter);
    printf("SP\t0x%02x\n", sp_register.pair);
    printf("A\t0x%02x\n", af_register.high);
    printf("B\t0x%02x\n", bc_register.high);
    printf("C\t0x%02x\n", bc_register.low);
    printf("D\t0x%02x\n", de_register.high);
    printf("E\t0x%02x\n", de_register.low);
    printf("H\t0x%02x\n", hl_register.high);
    printf("L\t0x%02x\n", hl_register.low);
    printf("FLAG\t0x%02x\n", af_register.low);
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

/*
    0x76 (HALT)
    Power down CPU until an interrupt occurs. Use this
    when ever possible to reduce energy consumption
*/

void CPU::HALT() {
    halt = 1;
    timer.m_cycles += 1;
    timer.t_cycles += 4;
}

/*
    0x1000 (STOP)
    Halt CPU & LCD display until button pressed.
*/

void CPU::STOP() {
    stop = 1;
    timer.m_cycles += 1;
    timer.t_cycles += 4;
}

/*
    0xF3 (DI)
    This instruction disables interrupts but not
    immediately. Interrupts are disabled after
    instruction after DI is executed.
*/

void CPU::DI() {
    timer.m_cycles += 1;
    timer.t_cycles += 4;
}

/*
    0xFB (EI)
    Enable interrupts. This intruction enables interrupts
    but not immediately. Interrupts are enabled after
    instruction after EI is executed.
*/

void CPU::EI() {
    timer.m_cycles += 1;
    timer.t_cycles += 4;
}

/**< 8 Bit Loads */

void CPU::LD8_r_nn(uint8_t &reg) {
    uint8_t nn = memory.ReadByteMemory(program_counter);
    reg = nn;
    program_counter++;
    timer.m_cycles += 1;
    timer.t_cycles += 8;
}

void CPU::LD8_r1_r2(uint8_t &reg1, uint8_t &reg2) {
    reg1 = reg2;
    program_counter++;
    timer.m_cycles += 1;
    timer.t_cycles += 4;
}

void CPU::LD8_r1_mem(uint8_t &reg1, uint16_t address) {
    reg1 = memory.ReadByteMemory(address);
    program_counter++;
    timer.m_cycles += 1;
    timer.t_cycles += 8;
}

void CPU::LD8_mem_r1(uint16_t address, uint8_t reg1) {
    memory.WriteMemory(address, reg1);
    program_counter++;
    timer.m_cycles += 1;
    timer.t_cycles += 8;
}

/**< 16 Bit Loads */

void CPU::LD16_r_nn(uint16_t &reg) {
    uint16_t nn = memory.ReadWordMemory(program_counter);
    reg = nn;
    program_counter++;
    timer.m_cycles += 2;
    timer.t_cycles += 12;

}
