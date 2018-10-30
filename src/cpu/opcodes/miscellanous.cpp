#include "../cpu.hpp"

void CPU::NOP() {
    clocks.m_cycles += 1;
    clocks.t_cycles += 4;
}

void CPU::HALT() {
    halt = 1;
    clocks.m_cycles += 1;
    clocks.t_cycles += 4;
}

void CPU::STOP() {
    stop = 1;
    clocks.m_cycles += 1;
    clocks.t_cycles += 4;
}

void CPU::DI() {
    pending_interupt_disabled = true;
    clocks.m_cycles += 1;
    clocks.t_cycles += 4;
}

void CPU::EI() {
    pending_interupt_enabled = true;
    clocks.m_cycles += 1;
    clocks.t_cycles += 4;
}


// Misc instructions

void CPU::DAA() {
	if (TestBit(af_register.low, SUBSTRACT_FLAG)) {
		if ((af_register.high & 0x0F) > 0x09 || af_register.low & 0x20) {
			af_register.high -= 0x06;
			if ((af_register.high & 0xF0) == 0xF0) {
                af_register.low |= 0x10;
            } else {
                af_register.low &=~0x10;
            }
		}

		if ((af_register.high & 0xF0) > 0x90 || af_register.low & 0x10) {
            af_register.high -= 0x60;
        }
	}
	else {
		if ((af_register.high & 0x0F) > 9 || af_register.low & 0x20) {
            af_register.high += 0x06;
			if ((af_register.high & 0xF0) == 0) {
                af_register.low |= 0x10;
            }
            else {
                af_register.low &= ~0x10;
            }
        }

		if ((af_register.high & 0xF0) > 0x90 || af_register.low & 0x10) {
            af_register.high += 0x60;
        }
	}

	if (af_register.high == 0) {
        af_register.low |= 0x80;
    } 
    else {
        af_register.low &= ~0x80;
    }
}

// Jumps

void CPU::JUMP(uint8_t flag, int condition, int use_condition) {
    int16_t nn = (int16_t)memory->ReadWordMemory(program_counter);
    program_counter += 2;
    if (!use_condition) {
        program_counter = nn;
    }
    else if (TestBit(af_register.low, flag) == condition) {
        program_counter = nn;
    }
    clocks.t_cycles += 3;
    clocks.m_cycles += 12;

}

void CPU::JUMP_IMM(uint8_t flag, int condition, int use_condition) {
    int8_t n = (int8_t)memory->ReadByteMemory(program_counter);
    if (!use_condition) {
        program_counter += n;
    }
    else if (TestBit(af_register.low, flag) == condition) {
        program_counter += n;
    }
    program_counter++;
    clocks.t_cycles += 2;
    clocks.m_cycles += 8;

}

// Calls

void CPU::CALL(uint8_t flag, int condition, int use_condition) {
    uint16_t nn = memory->ReadWordMemory(program_counter);
    program_counter += 2;
    if (!use_condition) {
        Push(program_counter);
        program_counter = nn;
    }
    else if (TestBit(af_register.low, flag) == condition) {
        Push(program_counter);
        program_counter = nn;
    }
    clocks.t_cycles += 3;
    clocks.m_cycles += 16;
}

void CPU::RST(uint8_t arg) {
	Push(program_counter);
	program_counter = arg;
}

void CPU::RET(uint8_t flag, int condition, int use_condition) {
    if (!use_condition) {
        Pop(program_counter);
    }
    else if (TestBit(af_register.low, flag) == condition) {
        Pop(program_counter);
    }
    clocks.t_cycles += 1;
    clocks.m_cycles += 8;
}

