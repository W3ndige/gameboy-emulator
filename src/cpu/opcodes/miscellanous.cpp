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
	if (!TestBit(af_register.low, SUBSTRACT_FLAG)) {
        if (TestBit(af_register.low, HALF_CARRY_FLAG) || (af_register.high & 0xf) > 0x9) {
            af_register.high += 0x06;
        }
        if (TestBit(af_register.low, CARRY_FLAG) || af_register.high > 0x9f) {
            af_register.high += 0x60;
        }
    } else {
        if (TestBit(af_register.low, HALF_CARRY_FLAG)) {
            af_register.high = (af_register.high - 0x6) & 0xFF;
        }
        if (TestBit(af_register.low, CARRY_FLAG)) {
            af_register.high -= 0x60;
        }
    } 
    af_register.low &= ~(HALF_CARRY_FLAG | ZERO_FLAG);

    if ((af_register.high & 0x100) == 0x100)
        af_register.low |= CARRY_FLAG;

    af_register.high &= 0xFF;

    if (af_register.high == 0)
        SetBit(af_register.low, ZERO_FLAG);
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
    if (exit_on_inifite_loop && n == -2) {
        exit(0);
    }
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

/**< Unique arithmetic functions */

void CPU::RLCA() {;
	int msb_set = TestBit(af_register.high, 7);
	af_register.low = 0;

	af_register.high <<= 1;
	if (msb_set) {
		SetBit(af_register.low, CARRY_FLAG);
		SetBit(af_register.high, 0);
	}

    clocks.m_cycles += 2;
    clocks.t_cycles += 8;
}

void CPU::RLA() {
	int msb_set = TestBit(af_register.high, 7);
    int carry_set = TestBit(af_register.low, CARRY_FLAG);

	af_register.low = 0;

	af_register.high <<= 1;

	if (msb_set) {
		SetBit(af_register.low, CARRY_FLAG);
	}

    if (carry_set) {
        SetBit(af_register.high, 0);
    }

    clocks.m_cycles += 1;
    clocks.t_cycles += 8;

}

void CPU::RRA() {
	int lsb_set = TestBit(af_register.high, 0);
    int carry_set = TestBit(af_register.low, CARRY_FLAG);

	af_register.low = 0;

	af_register.high >>= 1;

	if (lsb_set) {
		SetBit(af_register.low, CARRY_FLAG);
	}

    if (carry_set) {
        SetBit(af_register.high, 7);
    }

    clocks.m_cycles += 2;
    clocks.t_cycles += 8;
}

void CPU::RRCA() {
	int lsb_set = TestBit(af_register.high, 0);
	af_register.low = 0;

	af_register.high >>= 1;
	if (lsb_set) {
		SetBit(af_register.low, CARRY_FLAG);
		SetBit(af_register.high, 7);
	}

    clocks.m_cycles += 2;
    clocks.t_cycles += 8;
}
