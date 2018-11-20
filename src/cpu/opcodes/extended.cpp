#include "../cpu.hpp"

// Right shift - lsb, left shift msb

// Extended instruction set
void CPU::RLC(uint8_t &reg) {;
	int msb_set = TestBit(reg, 7);
	af_register.low = 0;

	reg <<= 1;
	if (msb_set) {
		SetBit(af_register.low, CARRY_FLAG);
		SetBit(reg, 0);
	}

	if (reg == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }

    clocks.m_cycles += 2;
    clocks.t_cycles += 8;
}

void CPU::RRC(uint8_t &reg) {
	int lsb_set = TestBit(reg, 0);
	af_register.low = 0;

	reg >>= 1;
	if (lsb_set) {
		SetBit(af_register.low, CARRY_FLAG);
		SetBit(reg, 7);
	}

	if (reg == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }

    clocks.m_cycles += 2;
    clocks.t_cycles += 8;
}

void CPU::RL(uint8_t &reg) {
	int msb_set = TestBit(reg, 7);
    int carry_set = TestBit(af_register.low, CARRY_FLAG);

	af_register.low = 0;

	reg <<= 1;

	if (msb_set) {
		SetBit(af_register.low, CARRY_FLAG);
	}

    if (carry_set) {
        SetBit(reg, 0);
    }

	if (reg == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }
    clocks.m_cycles += 1;
    clocks.t_cycles += 8;
}

void CPU::RR(uint8_t &reg) {
	int lsb_set = TestBit(reg, 0);
    int carry_set = TestBit(af_register.low, CARRY_FLAG);

	af_register.low = 0;

	reg >>= 1;

	if (lsb_set) {
		SetBit(af_register.low, CARRY_FLAG);
	}

    if (carry_set) {
        SetBit(reg, 7);
    }

	if (reg == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }
    clocks.m_cycles += 2;
    clocks.t_cycles += 8;
}

void CPU::SLA(uint8_t &reg) {
	int msb_set = TestBit(reg, 7);
	reg <<= 1;

	af_register.low = 0;

	if (msb_set) {
        SetBit(af_register.low, CARRY_FLAG);
    }
	
    if (reg == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }

    clocks.m_cycles += 2;
    clocks.t_cycles += 8;
}

void CPU::SRA(uint8_t &reg) {
	int lsb_set = TestBit(reg, 0);
    int msb_set = TestBit(reg, 7);

	reg >>= 1;

	af_register.low = 0;

	if (msb_set) {
        SetBit(reg, 7);
    }

	if (lsb_set) {
        SetBit(af_register.low, CARRY_FLAG);
    }

    if (reg == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }
    clocks.m_cycles += 2;
    clocks.t_cycles += 8;
}

void CPU::Swap(uint8_t &reg) {
    
    af_register.low = 0;
    reg = (((reg & 0xF0) >> 4) | ((reg & 0x0F) << 4));
    if (reg == 0) {
        SetBit(af_register.high, ZERO_FLAG);
    }
    clocks.t_cycles += 2;
    clocks.m_cycles += 8;
}

void CPU::SRL(uint8_t &reg) {
	int lsb_set = TestBit(reg, 0);

	af_register.low = 0;

	reg >>= 1;

	if (lsb_set) {
		SetBit(af_register.low, CARRY_FLAG);
    }

    if (reg == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }
    clocks.m_cycles += 2;
    clocks.t_cycles += 8;
}

void CPU::Bit(uint8_t &reg, uint8_t bit) {
    if (TestBit(reg, bit)) {
        ClearBit(af_register.low, ZERO_FLAG);
    } else {
        SetBit(af_register.low, ZERO_FLAG);
    }

    ClearBit(af_register.low, SUBSTRACT_FLAG);
    SetBit(af_register.low, HALF_CARRY_FLAG);
    clocks.m_cycles += 2;
    clocks.t_cycles += 8;
}

void CPU::Res(uint8_t &reg, uint8_t bit) {
    ClearBit(reg, bit);
    clocks.m_cycles += 2;
    clocks.t_cycles += 8;
}

void CPU::Set(uint8_t &reg, uint8_t bit) {
    SetBit(reg, bit);
    clocks.m_cycles += 2;
    clocks.t_cycles += 8;
}
