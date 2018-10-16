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
    memory.WriteChunkMemory(0, 0x100, bootstrap_data);
    fclose(bootstrap_file);

    /**< Place power up sequence values in memory */
    memory.WriteByteMemory(0xFF05, 0x00);
    memory.WriteByteMemory(0xFF06, 0x00);
    memory.WriteByteMemory(0xFF07, 0x00);
    memory.WriteByteMemory(0xFF10, 0x80);
    memory.WriteByteMemory(0xFF11, 0xBF);
    memory.WriteByteMemory(0xFF12, 0xF3);
    memory.WriteByteMemory(0xFF14, 0xBF);
    memory.WriteByteMemory(0xFF16, 0x3F);
    memory.WriteByteMemory(0xFF17, 0x00);
    memory.WriteByteMemory(0xFF19, 0xBF);
    memory.WriteByteMemory(0xFF1A, 0x7F);
    memory.WriteByteMemory(0xFF1B, 0xFF);
    memory.WriteByteMemory(0xFF1C, 0x9F);
    memory.WriteByteMemory(0xFF1E, 0xBF);
    memory.WriteByteMemory(0xFF20, 0xFF);
    memory.WriteByteMemory(0xFF21, 0x00);
    memory.WriteByteMemory(0xFF22, 0x00);
    memory.WriteByteMemory(0xFF23, 0xBF);
    memory.WriteByteMemory(0xFF24, 0x77);
    memory.WriteByteMemory(0xFF25, 0xF3);
    memory.WriteByteMemory(0xFF26, 0xF1);
    memory.WriteByteMemory(0xFF40, 0x91);
    memory.WriteByteMemory(0xFF42, 0x00);
    memory.WriteByteMemory(0xFF43, 0x00);
    memory.WriteByteMemory(0xFF45, 0x00);
    memory.WriteByteMemory(0xFF47, 0xFC);
    memory.WriteByteMemory(0xFF48, 0xFF);
    memory.WriteByteMemory(0xFF49, 0xFF);
    memory.WriteByteMemory(0xFF4A, 0x00);
    memory.WriteByteMemory(0xFF4B, 0x00);
    memory.WriteByteMemory(0xFFFF, 0x00);

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

        /*case 0xF8:
            {
                int8_t n = memory.ReadByteMemory(program_counter);
                program_counter++;
                ClearFlag(ZERO_FLAG);
                ClearFlag(SUBSTRACT_FLAG);
                uint16_t value = sp_register.pair + n;
                LD16_r_nn(value);
                if (n > 0xFFFF) {
                    SetFlag(CARRY_FLAG);
                }
			    else {
                    ClearFlag(CARRY_FLAG);
                }
                if ((sp_register.pair & 0xF) + (n & 0xF) > 0xF) {
                    SetFlag(HALF_CARRY_FLAG);
                }
                else {
                    ClearFlag(HALF_CARRY_FLAG);
                }
            }
            timer.t_cycles += 2;
            timer.m_cycles += 12;
            break;
        */
       
        case 0x08:
            program_counter++;
            memory.WriteWordMemory(memory.ReadWordMemory(program_counter), 
                                    sp_register.pair);
            program_counter++;
            timer.t_cycles += 3;
            timer.m_cycles += 20;
            break;

        case 0xF5: Push(af_register.pair); break;
        case 0xC5: Push(bc_register.pair); break;
        case 0xD5: Push(de_register.pair); break;
        case 0xE5: Push(hl_register.pair); break;

        case 0xF1: Pop(af_register.pair); break;
        case 0xC1: Pop(bc_register.pair); break;
        case 0xD1: Pop(de_register.pair); break;
        case 0xE1: Pop(hl_register.pair); break;

        // 8 bit alu
        case 0xAF: Xor8Bit(af_register.high); break;


         // Extended instruction set
        case 0xCB:
            ExecuteExtendedInstruction(memory.ReadByteMemory(program_counter));
            program_counter++;
            break;
        
        default: printf("Unkown opcode: 0x%02x\n", opcode);
    }   
}

void CPU::ExecuteExtendedInstruction(uint8_t opcode) {
    switch (opcode) {

        // Bit opcodes
        case 0x7c:
            {
                uint8_t bit = TestBit(hl_register.high, 7);
                if (bit == 0) {
                    SetBit(af_register.low, ZERO_FLAG);
                }
                ClearBit(af_register.low, SUBSTRACT_FLAG);
                SetBit(af_register.low, HALF_CARRY_FLAG);
            }
            timer.m_cycles += 2;
            timer.t_cycles += 8;
            break;

        default: printf("Unknown extended opcode: 0x%02x\n", opcode);

    }
}

void CPU::FetchAndDispatch() {
    uint8_t opcode = memory.ReadByteMemory(program_counter);
    program_counter++;
    ExecuteInstruction(opcode);
}

int CPU::Breakpoint(uint16_t pc) {
    if (pc == program_counter) {
        return 1;
    }
    return 0;
}

void CPU::Diagnostics() {
    printf("PC\t0x%04x\n", program_counter);
    printf("SP\t0x%04x\n", sp_register.pair);
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
    memory.DumpMemory();
}

void CPU::SetBit(uint8_t &reg, uint8_t flag) {
    reg |= 1 << flag;
}

int CPU::TestBit(uint8_t &reg, uint8_t flag) {
    int bit = reg >> flag & 1;
    return bit;
}

void CPU::ClearBit(uint8_t &reg, uint8_t flag) {
    reg &= ~(1 << flag);
}

void CPU::NOP() {
    timer.m_cycles += 1;
    timer.t_cycles += 4;
}

void CPU::HALT() {
    halt = 1;
    timer.m_cycles += 1;
    timer.t_cycles += 4;
}

void CPU::STOP() {
    stop = 1;
    timer.m_cycles += 1;
    timer.t_cycles += 4;
}

void CPU::DI() {
    timer.m_cycles += 1;
    timer.t_cycles += 4;
}

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
    timer.m_cycles += 1;
    timer.t_cycles += 4;
}

void CPU::LD8_r1_mem(uint8_t &reg1, uint16_t address) {
    reg1 = memory.ReadByteMemory(address);
    timer.m_cycles += 1;
    timer.t_cycles += 8;
}

void CPU::LD8_mem_r1(uint16_t address, uint8_t reg1) {
    memory.WriteByteMemory(address, reg1);
    timer.m_cycles += 1;
    timer.t_cycles += 8;
}

/**< 16 Bit Loads */

void CPU::LD16_r_nn(uint16_t &reg) {
    uint16_t nn = memory.ReadWordMemory(program_counter);
    program_counter += 2;
    reg = nn;
    timer.m_cycles += 2;
    timer.t_cycles += 12;

}

void CPU::Push(uint16_t &reg) {
    sp_register.pair -= 2;
    memory.WriteWordMemory(sp_register.pair, reg);
    timer.m_cycles += 1;
    timer.t_cycles += 16;
}

void CPU::Pop(uint16_t &reg) {
    sp_register.pair += 2;
    reg = memory.ReadWordMemory(sp_register.pair);
    timer.m_cycles += 1;
    timer.t_cycles += 12;
}

//  8 bit alu

void CPU::Add8Bit(uint8_t &reg) {
    af_register.high += reg;

    // Rethink flags
    if (af_register.high == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }
    ClearBit(af_register.low, SUBSTRACT_FLAG);

    timer.t_cycles += 1;
    timer.m_cycles += 4;

}

void CPU::Xor8Bit(uint8_t &reg) {
    reg ^= reg;
    if (reg == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }
    ClearBit(af_register.low, SUBSTRACT_FLAG);
    ClearBit(af_register.low, HALF_CARRY_FLAG);
    ClearBit(af_register.low, CARRY_FLAG);
    timer.m_cycles += 1;
    timer.t_cycles += 4;

}