#include "cpu.hpp"

CPU::CPU(Memory *mem) {
    memory = mem;
}

bool CPU::Init() {
    program_counter = 0x00;
    clocks.m_cycles = 0;
    clocks.t_cycles = 0;
    clocks.divide_counter = 0;
    interupts = false;
    pending_interupt_disabled = false;
    pending_interupt_enabled = false;
    halt = 0;
    stop = 0;

    /**< Read the bootstrap ROM into bootstrap memory */
    memory->LoadBootstrap();
    
    /**< If booting failed or stopped, initialize register with
     *   values that bootstrap sets */
    if (!memory->IsBooting()) {
        program_counter = 0x100;

        af_register.pair = 0x01B0;
        bc_register.pair = 0x0013;
        de_register.pair = 0x00D8; 
        hl_register.pair = 0x014D;
        sp_register.pair = 0xFFFE;

        /**< Place power up sequence values in memory */
        memory->WriteByteMemory(0xFF05, 0x00);
        memory->WriteByteMemory(0xFF06, 0x00);
        memory->WriteByteMemory(0xFF07, 0x00);
        memory->WriteByteMemory(0xFF10, 0x80);
        memory->WriteByteMemory(0xFF11, 0xBF);
        memory->WriteByteMemory(0xFF12, 0xF3);
        memory->WriteByteMemory(0xFF14, 0xBF);
        memory->WriteByteMemory(0xFF16, 0x3F);
        memory->WriteByteMemory(0xFF17, 0x00);
        memory->WriteByteMemory(0xFF19, 0xBF);
        memory->WriteByteMemory(0xFF1A, 0x7F);
        memory->WriteByteMemory(0xFF1B, 0xFF);
        memory->WriteByteMemory(0xFF1C, 0x9F);
        memory->WriteByteMemory(0xFF1E, 0xBF);
        memory->WriteByteMemory(0xFF20, 0xFF);
        memory->WriteByteMemory(0xFF21, 0x00);
        memory->WriteByteMemory(0xFF22, 0x00);
        memory->WriteByteMemory(0xFF23, 0xBF);
        memory->WriteByteMemory(0xFF24, 0x77);
        memory->WriteByteMemory(0xFF25, 0xF3);
        memory->WriteByteMemory(0xFF26, 0xF1);
        memory->WriteByteMemory(0xFF40, 0x91);
        memory->WriteByteMemory(0xFF42, 0x00);
        memory->WriteByteMemory(0xFF43, 0x00);
        memory->WriteByteMemory(0xFF45, 0x00);
        memory->WriteByteMemory(0xFF47, 0xFC);
        memory->WriteByteMemory(0xFF48, 0xFF);
        memory->WriteByteMemory(0xFF49, 0xFF);
        memory->WriteByteMemory(0xFF4A, 0x00);
        memory->WriteByteMemory(0xFF4B, 0x00);
        memory->WriteByteMemory(0xFFFF, 0x00);
        memory->WriteByteMemory(0x8AAA, 0xFF);
        memory->WriteByteMemory(0x8AAB, 0xFF);
        memory->WriteByteMemory(0x8AAC, 0xFF);
        memory->WriteByteMemory(0x8AAD, 0xFF);
    }

    /**< Try reading cartridge ROM into main memory */
    return memory->LoadCartridge();
}

void CPU::FetchAndDispatch() {
    //printf("0x%04x\n", program_counter);
    uint8_t opcode = memory->ReadByteMemory(program_counter);
    program_counter++;
    ExecuteInstruction(opcode);
}

void CPU::ExecuteInstruction(uint8_t opcode) {
    switch (opcode) {
        case 0x00: NOP(); break;
        case 0x76: HALT(); break;
        //case 0x1000: STOP(); break;
        case 0xf3: DI(); break;
        case 0xfb: EI(); break;
        
        /**< Load 8 bit immediate value */
        case 0x06: LD8_r_nn(bc_register.high); break;
        case 0x0e: LD8_r_nn(bc_register.low); break;
        case 0x16: LD8_r_nn(de_register.high); break;
        case 0x1e: LD8_r_nn(de_register.low); break;
        case 0x26: LD8_r_nn(hl_register.high); break;
        case 0x2e: LD8_r_nn(hl_register.low); break;

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
        case 0x5a: LD8_r1_r2(de_register.low, de_register.high); break;
        case 0x5b: LD8_r1_r2(de_register.low, de_register.low); break;
        case 0x5c: LD8_r1_r2(de_register.low, hl_register.high); break;
        case 0x5d: LD8_r1_r2(de_register.low, hl_register.low); break;
        case 0x60: LD8_r1_r2(hl_register.high, bc_register.high); break;
        case 0x61: LD8_r1_r2(hl_register.high, bc_register.low); break;
        case 0x62: LD8_r1_r2(hl_register.high, de_register.high); break;
        case 0x63: LD8_r1_r2(hl_register.high, de_register.low); break;
        case 0x64: LD8_r1_r2(hl_register.high, hl_register.high); break;
        case 0x65: LD8_r1_r2(hl_register.high, hl_register.low); break;
        case 0x68: LD8_r1_r2(hl_register.low, bc_register.high); break;
        case 0x69: LD8_r1_r2(hl_register.low, bc_register.low); break;
        case 0x6a: LD8_r1_r2(hl_register.low, de_register.high); break;
        case 0x6b: LD8_r1_r2(hl_register.low, de_register.low); break;
        case 0x6c: LD8_r1_r2(hl_register.low, hl_register.high); break;
        case 0x6d: LD8_r1_r2(hl_register.low, hl_register.low); break;

        case 0x47: LD8_r1_r2(bc_register.high, af_register.high); break;
        case 0x4f: LD8_r1_r2(bc_register.low, af_register.high); break;
        case 0x57: LD8_r1_r2(de_register.high, af_register.high); break;
        case 0x5f: LD8_r1_r2(de_register.low, af_register.high); break;
        case 0x67: LD8_r1_r2(hl_register.high, af_register.high); break;
        case 0x6f: LD8_r1_r2(hl_register.low, af_register.high); break;

        case 0x7e: LD8_r1_mem(af_register.high, hl_register.pair); break;
        case 0x46: LD8_r1_mem(bc_register.high, hl_register.pair); break;
        case 0x4e: LD8_r1_mem(bc_register.low, hl_register.pair); break;
        case 0x56: LD8_r1_mem(de_register.high, hl_register.pair); break;
        case 0x5e: LD8_r1_mem(de_register.low, hl_register.pair); break;
        case 0x66: LD8_r1_mem(hl_register.high, hl_register.pair); break;
        case 0x6e: LD8_r1_mem(hl_register.low, hl_register.pair); break;

        case 0x70: LD8_mem_r1(hl_register.pair, bc_register.high); break;
        case 0x71: LD8_mem_r1(hl_register.pair, bc_register.low); break;
        case 0x72: LD8_mem_r1(hl_register.pair, de_register.high); break;
        case 0x73: LD8_mem_r1(hl_register.pair, de_register.low); break;
        case 0x74: LD8_mem_r1(hl_register.pair, hl_register.high); break;
        case 0x75: LD8_mem_r1(hl_register.pair, hl_register.low); break;

        case 0x36: 
        {
            LD8_mem_r1(hl_register.pair, memory->ReadByteMemory(program_counter));
            program_counter++;
            clocks.t_cycles += 12;
            break;
        }

        case 0x0a: LD8_r1_mem(af_register.high, bc_register.pair); break;
        case 0x1a: LD8_r1_mem(af_register.high, de_register.pair); break;

        case 0xfa: 
        {
            LD8_r1_mem(af_register.high, memory->ReadWordMemory(program_counter)); 
            program_counter += 2;
            clocks.t_cycles += 8;
            break;

        }

        case 0x3e: LD8_r_nn(af_register.high); break;

        case 0x02: LD8_mem_r1(bc_register.pair, af_register.high); break;
        case 0x12: LD8_mem_r1(de_register.pair, af_register.high); break;
        case 0x77: LD8_mem_r1(hl_register.pair, af_register.high); break;
        
        case 0xea:
        {
            uint16_t nn = memory->ReadWordMemory(program_counter);
            program_counter += 2;
            memory->WriteByteMemory(nn, af_register.high);
            clocks.m_cycles += 3;
            clocks.t_cycles += 16;
        }
        break;
       
        case 0xf2: LD8_r1_mem(af_register.high, 0xFF00 + bc_register.low); break;
        case 0xe2: LD8_mem_r1(0xFF00 + bc_register.low, af_register.high); break;

        case 0x3a: LD8_r1_mem(af_register.high, hl_register.pair); hl_register.pair--; break;
        case 0x32: LD8_mem_r1(hl_register.pair, af_register.high); hl_register.pair--; break;
        case 0x2a: LD8_r1_mem(af_register.high, hl_register.pair); hl_register.pair++; break;
        case 0x22: LD8_mem_r1(hl_register.pair, af_register.high); hl_register.pair++; break;

        case 0xe0: LD8_mem_r1(0xFF00 + memory->ReadByteMemory(program_counter), 
                    af_register.high); program_counter++; clocks.t_cycles += 8; break;

        case 0xf0: LD8_r1_mem(af_register.high, 
                    0xFF00 + memory->ReadByteMemory(program_counter)); program_counter++; 
                    clocks.t_cycles += 8; break;

       /**< Load 16 bit immediate value */
        case 0x01: LD16_r_nn(bc_register.pair); break;
        case 0x11: LD16_r_nn(de_register.pair); break;
        case 0x21: LD16_r_nn(hl_register.pair); break;
        case 0x31: LD16_r_nn(sp_register.pair); break;
        case 0xf9: sp_register.pair = hl_register.pair; clocks.t_cycles += 8; break;

        case 0xF8:
        {
            int8_t n = memory->ReadByteMemory(program_counter);
            program_counter++;
            ClearBit(af_register.low, ZERO_FLAG);
            ClearBit(af_register.low, SUBSTRACT_FLAG);
            uint16_t value = sp_register.pair + n;
            LD16_r_nn(value);
            // ??? 
            if (n > 0xFFFF) {
                SetBit(af_register.low, CARRY_FLAG);
            }
			else {
                ClearBit(af_register.low, CARRY_FLAG);
            }
            if ((sp_register.pair & 0xF) + (n & 0xF) > 0xF) {
                SetBit(af_register.low, HALF_CARRY_FLAG);
            }
            else {
                ClearBit(af_register.low, HALF_CARRY_FLAG);
            }
            clocks.t_cycles += 2;
            clocks.m_cycles += 12;
            break;
    }
       
        case 0x08:
        {
            uint16_t nn = memory->ReadWordMemory(program_counter);
            memory->WriteWordMemory(nn, sp_register.pair);
            program_counter += 2;
            clocks.t_cycles += 3;
            clocks.m_cycles += 20;
            break;
        }

        case 0xf5: Push(af_register.pair); break;
        case 0xc5: Push(bc_register.pair); break;
        case 0xd5: Push(de_register.pair); break;
        case 0xe5: Push(hl_register.pair); break;

        case 0xf1: Pop(af_register.pair); break;
        case 0xc1: Pop(bc_register.pair); break;
        case 0xd1: Pop(de_register.pair); break;
        case 0xe1: Pop(hl_register.pair); break;

        // 8 bit alu
        case 0x80: Add8Bit(bc_register.high, 0); break;
        case 0x81: Add8Bit(bc_register.low, 0); break;
        case 0x82: Add8Bit(de_register.high, 0); break;
        case 0x83: Add8Bit(de_register.low, 0); break;
        case 0x84: Add8Bit(hl_register.high, 0); break;
        case 0x85: Add8Bit(hl_register.low, 0); break;
        case 0x86:
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Add8Bit(n, 0); 
            clocks.t_cycles += 4;
            break;
        }
        case 0x87: Add8Bit(af_register.high, 0); break;
        case 0xc6:
        {
            uint8_t n = memory->ReadByteMemory(program_counter);
            program_counter++;
            Add8Bit(n, 0);
            clocks.t_cycles += 4;
            break;
        }

        case 0x8f: Add8Bit(af_register.high, 1); break;
        case 0x88: Add8Bit(bc_register.high, 1); break;
        case 0x89: Add8Bit(bc_register.low, 1); break;
        case 0x8a: Add8Bit(de_register.high, 1); break;
        case 0x8b: Add8Bit(de_register.low, 1); break;
        case 0x8c: Add8Bit(hl_register.high, 1); break;
        case 0x8d: Add8Bit(hl_register.low, 1); break;
        case 0x8e:
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Add8Bit(n, 1); 
            clocks.t_cycles += 4;
            break;
        }
        case 0xce:
        {
            uint8_t n = memory->ReadByteMemory(program_counter);
            program_counter++;
            Add8Bit(n, 1);
            clocks.t_cycles += 4;
            break;
        }

        case 0x90: Sub8Bit(bc_register.high, 0); break;
        case 0x91: Sub8Bit(bc_register.low, 0); break;
        case 0x92: Sub8Bit(de_register.high, 0); break;
        case 0x93: Sub8Bit(de_register.low, 0); break;
        case 0x94: Sub8Bit(hl_register.high, 0); break;
        case 0x95: Sub8Bit(hl_register.low, 0); break;
        case 0x96:
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Sub8Bit(n, 0); 
            clocks.t_cycles += 4;
            break;
        }
        case 0xd6:
        {
            uint8_t n = memory->ReadByteMemory(program_counter);
            program_counter++;
            Sub8Bit(n, 0);
            clocks.t_cycles += 4;
            break;
        }

        case 0x9f: Sub8Bit(af_register.high, 1); break;
        case 0x98: Sub8Bit(bc_register.high, 1); break;
        case 0x99: Sub8Bit(bc_register.low, 1); break;
        case 0x9a: Sub8Bit(de_register.high, 1); break;
        case 0x9b: Sub8Bit(de_register.low, 1); break;
        case 0x9c: Sub8Bit(hl_register.high, 1); break;
        case 0x9d: Sub8Bit(hl_register.low, 1); break;
        case 0x9e:
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Sub8Bit(n, 1); 
            clocks.t_cycles += 4;
            break;
        }

        case 0xa7: And8Bit(af_register.high); break;
        case 0xa0: And8Bit(bc_register.high); break;
        case 0xa1: And8Bit(bc_register.low); break;
        case 0xa2: And8Bit(de_register.high); break;
        case 0xa3: And8Bit(de_register.low); break;
        case 0xa4: And8Bit(hl_register.high); break;
        case 0xa5: And8Bit(hl_register.low); break;
        case 0xa6:
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            And8Bit(n);
            clocks.t_cycles += 4;
            break;
        }
        case 0xe6:
        {
            uint8_t n = memory->ReadByteMemory(program_counter);
            program_counter++;
            And8Bit(n);
            clocks.t_cycles += 4;
            break;
        }

        case 0xb7: Or8Bit(af_register.high); break;
        case 0xb0: Or8Bit(bc_register.high); break;
        case 0xb1: Or8Bit(bc_register.low); break;
        case 0xb2: Or8Bit(de_register.high); break;
        case 0xb3: Or8Bit(de_register.low); break;
        case 0xb4: Or8Bit(hl_register.high); break;
        case 0xb5: Or8Bit(hl_register.low); break;
        case 0xb6:
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Or8Bit(n); 
            clocks.t_cycles += 4;
            break;
        }
        case 0xf6:
        {
            uint8_t n = memory->ReadByteMemory(program_counter);
            program_counter++;
            Or8Bit(n);
            clocks.t_cycles += 4;
            break;
        }

        case 0xaf: Xor8Bit(af_register.high); break;
        case 0xa8: Xor8Bit(bc_register.high); break;
        case 0xa9: Xor8Bit(bc_register.low); break;
        case 0xaa: Xor8Bit(de_register.high); break;
        case 0xab: Xor8Bit(de_register.low); break;
        case 0xac: Xor8Bit(hl_register.high); break;
        case 0xad: Xor8Bit(hl_register.low); break;
        case 0xae:
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Xor8Bit(n); 
            clocks.t_cycles += 4;
            break;
        }
        case 0xee:
        {
            uint8_t n = memory->ReadByteMemory(program_counter);
            program_counter++;
            Xor8Bit(n);
            clocks.t_cycles += 4;
            break;
        }

        case 0xbf: Cmp8Bit(af_register.high); break;
        case 0xb8: Cmp8Bit(bc_register.high); break;
        case 0xb9: Cmp8Bit(bc_register.low); break;
        case 0xba: Cmp8Bit(de_register.high); break;
        case 0xbb: Cmp8Bit(de_register.low); break;
        case 0xbc: Cmp8Bit(hl_register.high); break;
        case 0xbd: Cmp8Bit(hl_register.low); break;
        case 0xbe:
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Cmp8Bit(n); 
            clocks.t_cycles += 4;
            break;
        }
        case 0xfe:
        {
            uint8_t n = memory->ReadByteMemory(program_counter);
            program_counter++;
            Cmp8Bit(n);
            clocks.t_cycles += 4;
            break;
        }

        case 0x3c: Inc8Bit(af_register.high); break;
        case 0x04: Inc8Bit(bc_register.high); break;
        case 0x0c: Inc8Bit(bc_register.low); break;
        case 0x14: Inc8Bit(de_register.high); break;
        case 0x1c: Inc8Bit(de_register.low); break;
        case 0x24: Inc8Bit(hl_register.high); break;
        case 0x2c: Inc8Bit(hl_register.low); break;
        case 0x34:
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Inc8Bit(n);
            memory->WriteByteMemory(hl_register.pair, n);
            clocks.t_cycles += 8;
            break;
        }

        case 0x3d: Dec8Bit(af_register.high); break;
        case 0x05: Dec8Bit(bc_register.high); break;
        case 0x0d: Dec8Bit(bc_register.low); break;
        case 0x15: Dec8Bit(de_register.high); break;
        case 0x1d: Dec8Bit(de_register.low); break;
        case 0x25: Dec8Bit(hl_register.high); break;
        case 0x2d: Dec8Bit(hl_register.low); break;
        case 0x35:
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Dec8Bit(n);
            memory->WriteByteMemory(hl_register.pair, n);
            clocks.t_cycles += 8;
            break;
        }

        // 16 bit ALU

        case 0x09: Add16Bit(bc_register.pair); break;
        case 0x19: Add16Bit(de_register.pair); break;
        case 0x29: Add16Bit(hl_register.pair); break;
        case 0x39: Add16Bit(sp_register.pair); break;

        case 0xe8: AddSP16Bit(); break;

        case 0x03: Inc16Bit(bc_register.pair); break;
        case 0x13: Inc16Bit(de_register.pair); break;
        case 0x23: Inc16Bit(hl_register.pair); break;
        case 0x33: Inc16Bit(sp_register.pair); break;

        case 0x0B: Dec16Bit(bc_register.pair); break;
        case 0x1B: Dec16Bit(de_register.pair); break;
        case 0x2B: Dec16Bit(hl_register.pair); break;
        case 0x3B: Dec16Bit(sp_register.pair); break;

        // Misc opcodes
        case 0x27: DAA(); break;

        case 0x2f:
        {
			af_register.high ^= 0xFF;

			SetBit(af_register.low, SUBSTRACT_FLAG);
			SetBit(af_register.low, HALF_CARRY_FLAG);
            clocks.m_cycles += 1;
            clocks.t_cycles += 4;
            break;
        }
        
        case 0x3f:
        {
			if (TestBit(af_register.low, CARRY_FLAG)) {
                ClearBit(af_register.low, CARRY_FLAG);
            }
            else {
                SetBit(af_register.low, CARRY_FLAG);
            }

			ClearBit(af_register.low, HALF_CARRY_FLAG);
			ClearBit(af_register.low, SUBSTRACT_FLAG);
            clocks.m_cycles += 1;
            clocks.t_cycles += 4;
            break;
        }

        case 0x37:
        {   
			ClearBit(af_register.low, HALF_CARRY_FLAG);
			ClearBit(af_register.low, SUBSTRACT_FLAG);
            SetBit(af_register.low, CARRY_FLAG);
            clocks.m_cycles += 1;
            clocks.t_cycles += 4;
            break;
        }

        // Rotates and shifts
        case 0x07: RLC(af_register.high); break;
        case 0x17: RL(af_register.high); clocks.t_cycles -= 4; break;
        case 0x1F: RR(af_register.high); break;
        case 0x0f: RRC(af_register.high); break;

        // Jumps
        case 0xc3: JUMP(0, 0, 0); break;
        case 0xc2: JUMP(ZERO_FLAG, 0, 1); break;
        case 0xca: JUMP(ZERO_FLAG, 1, 1); break;
        case 0xd2: JUMP(CARRY_FLAG, 0, 1); break;
        case 0xda: JUMP(CARRY_FLAG, 1, 1); break;

        case 0xe9: program_counter += hl_register.pair; clocks.m_cycles += 1;
                    clocks.t_cycles += 4; break;

        case 0x18: JUMP_IMM(0, 0, 0); break;
        case 0x20: JUMP_IMM(ZERO_FLAG, 0, 1); break;
        case 0x28: JUMP_IMM(ZERO_FLAG, 1, 1); break;
        case 0x30: JUMP_IMM(CARRY_FLAG, 0, 1); break;
        case 0x38: JUMP_IMM(CARRY_FLAG, 1, 1); break;

        case 0xcd: CALL(0, 0, 0); break;
        case 0xc4: CALL(ZERO_FLAG, 0, 1); break;
        case 0xcc: CALL(ZERO_FLAG, 1, 1); break;
        case 0xd4: CALL(CARRY_FLAG, 0, 1); break;
        case 0xdc: CALL(CARRY_FLAG, 1, 1); break;

        case 0xc7: RST(0x00); break;
        case 0xcf: RST(0x08); break;
        case 0xd7: RST(0x10); break;
        case 0xdf: RST(0x18); break;
        case 0xe7: RST(0x20); break;
        case 0xef: RST(0x28); break;
        case 0xf7: RST(0x30); break;
        case 0xff: RST(0x38); break;

        case 0xc9: RET(0, 0, 0); break;
        case 0xc0: RET(ZERO_FLAG, 0, 1); break;
        case 0xc8: RET(ZERO_FLAG, 1, 1); break;
        case 0xd0: RET(CARRY_FLAG, 0, 1); break;
        case 0xd8: RET(CARRY_FLAG, 1, 1); break;

        case 0xd9: RET(0, 0, 0); /* enable interrupt */ break;

         // Extended instruction set
        case 0xcb:
            ExecuteExtendedInstruction(memory->ReadByteMemory(program_counter));
            program_counter++;
            break;
        
        default: printf("Unkown opcode: 0x%02x at PC: 0x%04x\n", opcode, program_counter);
    }   
}

void CPU::ExecuteExtendedInstruction(uint8_t opcode) {
    switch (opcode) {

        // Rotate opcodes
        case 0x00: RLC(bc_register.high); break;
        case 0x01: RLC(bc_register.low); break;
        case 0x02: RLC(de_register.high); break;
        case 0x03: RLC(de_register.low); break;
        case 0x04: RLC(hl_register.high); break;
        case 0x05: RLC(hl_register.low); break;
        case 0x06: 
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            RLC(n);
            memory->WriteByteMemory(hl_register.pair, n);
            clocks.m_cycles += 1;
            clocks.t_cycles += 8;
            break;
        }
        case 0x07: RLC(af_register.high); break;

        case 0x08: RRC(bc_register.high); break;
        case 0x09: RRC(bc_register.low); break;
        case 0x0A: RRC(de_register.high); break;
        case 0x0B: RRC(de_register.low); break;
        case 0x0C: RRC(hl_register.high); break;
        case 0x0D: RRC(hl_register.low); break;
        case 0x0E: 
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            RRC(n);
            memory->WriteByteMemory(hl_register.pair, n);
            clocks.m_cycles += 1;
            clocks.t_cycles += 8;
            break;
        }
        case 0x0F: RRC(af_register.high); break;

        case 0x10: RL(bc_register.high); break;
        case 0x11: RL(bc_register.low); break;
        case 0x12: RL(de_register.high); break;
        case 0x13: RL(de_register.low); break;
        case 0x14: RL(hl_register.high); break;
        case 0x15: RL(hl_register.low); break;
        case 0x16: 
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            RL(n);
            memory->WriteByteMemory(hl_register.pair, n);
            clocks.m_cycles += 1;
            clocks.t_cycles += 8;
            break;
        }
        case 0x17: RL(af_register.high); break;

        case 0x18: RR(bc_register.high); break;
        case 0x19: RR(bc_register.low); break;
        case 0x1A: RR(de_register.high); break;
        case 0x1B: RR(de_register.low); break;
        case 0x1C: RR(hl_register.high); break;
        case 0x1D: RR(hl_register.low); break;
        case 0x1E: 
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            RR(n);
            memory->WriteByteMemory(hl_register.pair, n);
            clocks.m_cycles += 1;
            clocks.t_cycles += 8;
            break;
        }
        case 0x1F: RR(af_register.high); break;

        // Shift opcodes
        case 0x20: SLA(bc_register.high); break;
        case 0x21: SLA(bc_register.low); break;
        case 0x22: SLA(de_register.high); break;
        case 0x23: SLA(de_register.low); break;
        case 0x24: SLA(hl_register.high); break;
        case 0x25: SLA(hl_register.low); break;
        case 0x26: 
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            SLA(n);
            memory->WriteByteMemory(hl_register.pair, n);
            clocks.m_cycles += 1;
            clocks.t_cycles += 8;
            break;
        }
        case 0x27: SLA(af_register.high); break;

        case 0x28: SRA(bc_register.high); break;
        case 0x29: SRA(bc_register.low); break;
        case 0x2A: SRA(de_register.high); break;
        case 0x2B: SRA(de_register.low); break;
        case 0x2C: SRA(hl_register.high); break;
        case 0x2D: SRA(hl_register.low); break;
        case 0x2E: 
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            SRA(n);
            memory->WriteByteMemory(hl_register.pair, n);
            clocks.m_cycles += 1;
            clocks.t_cycles += 8;
            break;
        }
        case 0x2F: SRA(af_register.high); break;

        // Swap opcodes
        case 0x30: Swap(bc_register.high); break;
        case 0x31: Swap(bc_register.low); break;
        case 0x32: Swap(de_register.high); break;
        case 0x33: Swap(de_register.low); break;
        case 0x34: Swap(hl_register.high); break;
        case 0x35: Swap(hl_register.low); break;
        case 0x36:
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Swap(n);
            memory->WriteByteMemory(hl_register.pair, n);
            clocks.t_cycles += 8;
            break;
        }
        case 0x37: Swap(af_register.high); break;

        // Shift SRL opcodes
        case 0x38: SRL(bc_register.high); break;
        case 0x39: SRL(bc_register.low); break;
        case 0x3A: SRL(de_register.high); break;
        case 0x3B: SRL(de_register.low); break;
        case 0x3C: SRL(hl_register.high); break;
        case 0x3D: SRL(hl_register.low); break;
        case 0x3E: 
        {
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            SRL(n);
            memory->WriteByteMemory(hl_register.pair, n);
            clocks.m_cycles += 1;
            clocks.t_cycles += 8;
            break;
        }
        case 0x3F: SRL(af_register.high); break;

        // Bit opcodes
        case 0x40: Bit(bc_register.high, 0); break;
        case 0x41: Bit(bc_register.low, 0); break;
        case 0x42: Bit(de_register.high, 0); break;
        case 0x43: Bit(de_register.low, 0); break;
        case 0x44: Bit(hl_register.high, 0); break;
        case 0x45: Bit(hl_register.low, 0); break;
        case 0x46:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Bit(n, 0);
            clocks.t_cycles += 8;
            break;
        }
        case 0x47: Bit(af_register.high, 0); break;

        case 0x48: Bit(bc_register.high, 1); break;
        case 0x49: Bit(bc_register.low, 1); break;
        case 0x4a: Bit(de_register.high, 1); break;
        case 0x4b: Bit(de_register.low, 1); break;
        case 0x4c: Bit(hl_register.high, 1); break;
        case 0x4d: Bit(hl_register.low, 1); break;
        case 0x4e:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Bit(n, 1);
            clocks.t_cycles += 8;
            break;
        }
        case 0x4f: Bit(af_register.high, 1); break;

        case 0x50: Bit(bc_register.high, 2); break;
        case 0x51: Bit(bc_register.low, 2); break;
        case 0x52: Bit(de_register.high, 2); break;
        case 0x53: Bit(de_register.low, 2); break;
        case 0x54: Bit(hl_register.high, 2); break;
        case 0x55: Bit(hl_register.low, 2); break;
        case 0x56:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Bit(n, 2);
            clocks.t_cycles += 8;
            break;
        }
        case 0x57: Bit(af_register.high, 2); break;

        case 0x58: Bit(bc_register.high, 3); break;
        case 0x59: Bit(bc_register.low, 3); break;
        case 0x5a: Bit(de_register.high, 3); break;
        case 0x5b: Bit(de_register.low, 3); break;
        case 0x5c: Bit(hl_register.high, 3); break;
        case 0x5d: Bit(hl_register.low, 3); break;
        case 0x5e:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Bit(n, 3);
            clocks.t_cycles += 8;
            break;
        }
        case 0x5f: Bit(af_register.high, 4); break;

        case 0x60: Bit(bc_register.high, 4); break;
        case 0x61: Bit(bc_register.low, 4); break;
        case 0x62: Bit(de_register.high, 4); break;
        case 0x63: Bit(de_register.low, 4); break;
        case 0x64: Bit(hl_register.high, 4); break;
        case 0x65: Bit(hl_register.low, 4); break;
        case 0x66:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Bit(n, 4);
            clocks.t_cycles += 8;
            break;
        }
        case 0x67: Bit(af_register.high, 4); break;

        case 0x68: Bit(bc_register.high, 5); break;
        case 0x69: Bit(bc_register.low, 5); break;
        case 0x6a: Bit(de_register.high, 5); break;
        case 0x6b: Bit(de_register.low, 5); break;
        case 0x6c: Bit(hl_register.high, 5); break;
        case 0x6d: Bit(hl_register.low, 5); break;
        case 0x6e:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Bit(n, 5);
            clocks.t_cycles += 8;
            break;
        }
        case 0x6f: Bit(af_register.high, 6); break;

        case 0x70: Bit(bc_register.high, 6); break;
        case 0x71: Bit(bc_register.low, 6); break;
        case 0x72: Bit(de_register.high, 6); break;
        case 0x73: Bit(de_register.low, 6); break;
        case 0x74: Bit(hl_register.high, 6); break;
        case 0x75: Bit(hl_register.low, 6); break;
        case 0x76:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Bit(n, 6);
            clocks.t_cycles += 8;
            break;
        }
        case 0x77: Bit(af_register.high, 6); break;

        case 0x78: Bit(bc_register.high, 7); break;
        case 0x79: Bit(bc_register.low, 7); break;
        case 0x7a: Bit(de_register.high, 7); break;
        case 0x7b: Bit(de_register.low, 7); break;
        case 0x7c: Bit(hl_register.high, 7); break;
        case 0x7d: Bit(hl_register.low, 7); break;
        case 0x7e:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Bit(n, 7);
            clocks.t_cycles += 8;
            break;
        }
        case 0x7f: Bit(af_register.high, 7); break;

        case 0x80: Res(bc_register.high, 0); break;
        case 0x81: Res(bc_register.low, 0); break;
        case 0x82: Res(de_register.high, 0); break;
        case 0x83: Res(de_register.low, 0); break;
        case 0x84: Res(hl_register.high, 0); break;
        case 0x85: Res(hl_register.low, 0); break;
        case 0x86:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Res(n, 0);
            clocks.t_cycles += 8;
            break;
        }
        case 0x87: Res(af_register.high, 0); break;

        case 0x88: Res(bc_register.high, 1); break;
        case 0x89: Res(bc_register.low, 1); break;
        case 0x8a: Res(de_register.high, 1); break;
        case 0x8b: Res(de_register.low, 1); break;
        case 0x8c: Res(hl_register.high, 1); break;
        case 0x8d: Res(hl_register.low, 1); break;
        case 0x8e:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Res(n, 1);
            clocks.t_cycles += 8;
            break;
        }
        case 0x8f: Res(af_register.high, 1); break;

        case 0x90: Res(bc_register.high, 2); break;
        case 0x91: Res(bc_register.low, 2); break;
        case 0x92: Res(de_register.high, 2); break;
        case 0x93: Res(de_register.low, 2); break;
        case 0x94: Res(hl_register.high, 2); break;
        case 0x95: Res(hl_register.low, 2); break;
        case 0x96:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Res(n, 2);
            clocks.t_cycles += 8;
            break;
        }
        case 0x97: Res(af_register.high, 2); break;

        case 0x98: Res(bc_register.high, 3); break;
        case 0x99: Res(bc_register.low, 3); break;
        case 0x9a: Res(de_register.high, 3); break;
        case 0x9b: Res(de_register.low, 3); break;
        case 0x9c: Res(hl_register.high, 3); break;
        case 0x9d: Res(hl_register.low, 3); break;
        case 0x9e:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Res(n, 3);
            clocks.t_cycles += 8;
            break;
        }
        case 0x9f: Res(af_register.high, 4); break;

        case 0xa0: Res(bc_register.high, 4); break;
        case 0xa1: Res(bc_register.low, 4); break;
        case 0xa2: Res(de_register.high, 4); break;
        case 0xa3: Res(de_register.low, 4); break;
        case 0xa4: Res(hl_register.high, 4); break;
        case 0xa5: Res(hl_register.low, 4); break;
        case 0xa6:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Res(n, 4);
            clocks.t_cycles += 8;
            break;
        }
        case 0xa7: Res(af_register.high, 4); break;

        case 0xa8: Res(bc_register.high, 5); break;
        case 0xa9: Res(bc_register.low, 5); break;
        case 0xaa: Res(de_register.high, 5); break;
        case 0xab: Res(de_register.low, 5); break;
        case 0xac: Res(hl_register.high, 5); break;
        case 0xad: Res(hl_register.low, 5); break;
        case 0xae:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Res(n, 5);
            clocks.t_cycles += 8;
            break;
        }
        case 0xaf: Res(af_register.high, 6); break;

        case 0xb0: Res(bc_register.high, 6); break;
        case 0xb1: Res(bc_register.low, 6); break;
        case 0xb2: Res(de_register.high, 6); break;
        case 0xb3: Res(de_register.low, 6); break;
        case 0xb4: Res(hl_register.high, 6); break;
        case 0xb5: Res(hl_register.low, 6); break;
        case 0xb6:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Res(n, 6);
            clocks.t_cycles += 8;
            break;
        }
        case 0xb7: Res(af_register.high, 6); break;

        case 0xb8: Res(bc_register.high, 7); break;
        case 0xb9: Res(bc_register.low, 7); break;
        case 0xba: Res(de_register.high, 7); break;
        case 0xbb: Res(de_register.low, 7); break;
        case 0xbc: Res(hl_register.high, 7); break;
        case 0xbd: Res(hl_register.low, 7); break;
        case 0xbe:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Res(n, 7);
            clocks.t_cycles += 8;
            break;
        }
        case 0xbf: Res(af_register.high, 7); break;

        case 0xc0: Set(bc_register.high, 0); break;
        case 0xc1: Set(bc_register.low, 0); break;
        case 0xc2: Set(de_register.high, 0); break;
        case 0xc3: Set(de_register.low, 0); break;
        case 0xc4: Set(hl_register.high, 0); break;
        case 0xc5: Set(hl_register.low, 0); break;
        case 0xc6:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Set(n, 0);
            clocks.t_cycles += 8;
            break;
        }
        case 0xc7: Set(af_register.high, 0); break;

        case 0xc8: Set(bc_register.high, 1); break;
        case 0xc9: Set(bc_register.low, 1); break;
        case 0xca: Set(de_register.high, 1); break;
        case 0xcb: Set(de_register.low, 1); break;
        case 0xcc: Set(hl_register.high, 1); break;
        case 0xcd: Set(hl_register.low, 1); break;
        case 0xce:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Set(n, 1);
            clocks.t_cycles += 8;
            break;
        }
        case 0xcf: Set(af_register.high, 1); break;

        case 0xd0: Set(bc_register.high, 2); break;
        case 0xd1: Set(bc_register.low, 2); break;
        case 0xd2: Set(de_register.high, 2); break;
        case 0xd3: Set(de_register.low, 2); break;
        case 0xd4: Set(hl_register.high, 2); break;
        case 0xd5: Set(hl_register.low, 2); break;
        case 0xd6:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Set(n, 2);
            clocks.t_cycles += 8;
            break;
        }
        case 0xd7: Set(af_register.high, 2); break;

        case 0xd8: Set(bc_register.high, 3); break;
        case 0xd9: Set(bc_register.low, 3); break;
        case 0xda: Set(de_register.high, 3); break;
        case 0xdb: Set(de_register.low, 3); break;
        case 0xdc: Set(hl_register.high, 3); break;
        case 0xdd: Set(hl_register.low, 3); break;
        case 0xde:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Set(n, 3);
            clocks.t_cycles += 8;
            break;
        }
        case 0xdf: Set(af_register.high, 4); break;

        case 0xe0: Set(bc_register.high, 4); break;
        case 0xe1: Set(bc_register.low, 4); break;
        case 0xe2: Set(de_register.high, 4); break;
        case 0xe3: Set(de_register.low, 4); break;
        case 0xe4: Set(hl_register.high, 4); break;
        case 0xe5: Set(hl_register.low, 4); break;
        case 0xe6:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Set(n, 4);
            clocks.t_cycles += 8;
            break;
        }
        case 0xe7: Set(af_register.high, 4); break;

        case 0xe8: Set(bc_register.high, 5); break;
        case 0xe9: Set(bc_register.low, 5); break;
        case 0xea: Set(de_register.high, 5); break;
        case 0xeb: Set(de_register.low, 5); break;
        case 0xec: Set(hl_register.high, 5); break;
        case 0xed: Set(hl_register.low, 5); break;
        case 0xee:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Set(n, 5);
            clocks.t_cycles += 8;
            break;
        }
        case 0xef: Set(af_register.high, 6); break;

        case 0xf0: Set(bc_register.high, 6); break;
        case 0xf1: Set(bc_register.low, 6); break;
        case 0xf2: Set(de_register.high, 6); break;
        case 0xf3: Set(de_register.low, 6); break;
        case 0xf4: Set(hl_register.high, 6); break;
        case 0xf5: Set(hl_register.low, 6); break;
        case 0xf6:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Set(n, 6);
            clocks.t_cycles += 8;
            break;
        }
        case 0xf7: Set(af_register.high, 6); break;

        case 0xf8: Set(bc_register.high, 7); break;
        case 0xf9: Set(bc_register.low, 7); break;
        case 0xfa: Set(de_register.high, 7); break;
        case 0xfb: Set(de_register.low, 7); break;
        case 0xfc: Set(hl_register.high, 7); break;
        case 0xfd: Set(hl_register.low, 7); break;
        case 0xfe:
        {   
            uint8_t n = memory->ReadByteMemory(hl_register.pair);
            Set(n, 7);
            clocks.t_cycles += 8;
            break;
        }
        case 0xff: Set(af_register.high, 7); break;

        default: printf("Unknown extended opcode: 0x%02x at PC: 0x%04x\n", 
                        opcode, program_counter);

    }
}

uint16_t CPU::GetProgramCounter() {
    return program_counter;
}

bool CPU::IsClockEnabled() {
     return TestBit(memory->ReadByteMemory(TMC), 2) ? true : false;
}

uint8_t CPU::GetClockFrequency() {
    return memory->ReadByteMemory(TMC) & 0x3;
}

void CPU::SetClockFrequency() {
   uint8_t frequency = GetClockFrequency( ) ;
   switch (frequency) {
     case 0: clocks.t_cycles = 1024; break;
     case 1: clocks.t_cycles = 16; break;
     case 2: clocks.t_cycles = 64; break;
     case 3: clocks.t_cycles = 256; break;
   }
}

void CPU::DividerRegister(int cycles) {
   clocks.divide_counter += cycles;
   if (clocks.divide_counter >= 255) {
        clocks.divide_counter = 0;
        uint8_t tmp = memory->ReadByteMemory(0xFF04);
        tmp++;
        memory->PrivilagedByteWrite(0xFF04, tmp);
   }
}

void CPU::UpdateTimer(int cycles) {
    DividerRegister(cycles);

    if (!IsClockEnabled()) {
        return;
    }

    clocks.t_cycles -= cycles;
    if (clocks.t_cycles <= 0) {
        SetClockFrequency();

        if (memory->ReadByteMemory(TIMA) == 255) {
            memory->WriteByteMemory(TIMA, memory->ReadByteMemory(TMA));
            RequestInterupt(2);
        }
        else {
            memory->WriteByteMemory(TIMA, memory->ReadByteMemory(TIMA) + 1);
        }
    }
}

void CPU::RequestInterupt(int id) {
    uint8_t req = memory->ReadByteMemory(0xFF0F);
    SetBit(req, id);
    memory->PrivilagedByteWrite(0xFF0F, req); 
}

void CPU::DoInterupts() {
    if (!interupts) {
        return;
    }
    uint8_t req = memory->ReadByteMemory(0xFF0F);
    uint8_t enabled = memory->ReadByteMemory(0xFFFF);
    if (req > 0) {

        for (int i = 0 ; i < 5; i++) {
            if (TestBit(req, i)) {
                if (TestBit(enabled, i)) {
                    ServiceInterupt(i) ;
                }
            }
        }
    }
}

void CPU::ServiceInterupt(int id) {
   interupts = false;
   uint8_t req = memory->ReadByteMemory(0xFF0F);
   ClearBit(req, id);
   memory->WriteByteMemory(0xFF0F, req) ;

   Push(program_counter);

   switch (id) {
     case 0: program_counter = 0x40; break;
     case 1: program_counter = 0x48; break;
     case 2: program_counter = 0x50; break;
     case 4: program_counter = 0x60; break;
   }
}

void CPU::ArtificialJump(int offset) {
    program_counter += offset;
}

unsigned int CPU::GetLastOpcodeTime() {
    return clocks.t_cycles;
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
    printf("AF\t0x%04x\n", af_register.pair);
    printf("BC\t0x%04x\n", bc_register.pair);
    printf("DE\t0x%04x\n", de_register.pair);
    printf("HL\t0x%04x\n", hl_register.pair);
    PrintFlags();
    memory->DumpMemory();
}

void CPU::PrintFlags() {
    printf("ZERO FLAG: %d\n", TestBit(af_register.low, 7));
    printf("SUBSTRACT FLAG: %d\n", TestBit(af_register.low, 6));
    printf("HALF CARRY FLAG: %d\n", TestBit(af_register.low, 5));
    printf("CARRY FLAG: %d\n", TestBit(af_register.low, 4));

}