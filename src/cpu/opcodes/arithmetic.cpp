#include "../cpu.hpp"

/**<  8 bit ALU */
void CPU::Add8Bit(uint8_t reg) {
    uint8_t before = af_register.high;

    af_register.high += reg;

    af_register.low = 0;
    if (af_register.high == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }

    if ((before + reg) > 0xFF) {
        SetBit(af_register.low, CARRY_FLAG);
    }

	uint16_t htest = (before & 0xF) + (reg & 0xF);
	if (htest > 0xF) {
        SetBit(af_register.low, HALF_CARRY_FLAG);
    }

    clocks.t_cycles += 1;
    clocks.m_cycles += 4;
}

void CPU::Adc8Bit(uint8_t reg) {
    uint8_t before = af_register.high;
    if (TestBit(af_register.low, CARRY_FLAG)) {
        reg++;
    }

    af_register.high += reg;

    af_register.low = 0;
    if (af_register.high == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }

    if ((before + reg) > 0xFF) {
        SetBit(af_register.low, CARRY_FLAG);
    }

	uint16_t htest = (before & 0xF) + (reg & 0xF);
	if (htest > 0xF) {
        SetBit(af_register.low, HALF_CARRY_FLAG);
    }

    clocks.t_cycles += 1;
    clocks.m_cycles += 4;
}


void CPU::Sub8Bit(uint8_t reg) {
    uint8_t before = af_register.high;

    af_register.high -= reg;

    af_register.low = 0;
    if (af_register.high == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }

    SetBit(af_register.low, SUBSTRACT_FLAG);

	if (before < reg) {
        SetBit(af_register.low, CARRY_FLAG);
    }

	int16_t htest = (before & 0xF) - (reg & 0xF);

	if (htest < 0) {
        SetBit(af_register.low, HALF_CARRY_FLAG);
    }

    clocks.m_cycles += 1;
    clocks.t_cycles += 4;
}

void CPU::Subc8Bit(uint8_t reg) {
    uint8_t before = af_register.high;

    if (TestBit(af_register.low, CARRY_FLAG)) {
        reg++;
    }
 
    af_register.high -= reg;

    af_register.low = 0;
    if (af_register.high == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }

    SetBit(af_register.low, SUBSTRACT_FLAG);

	if (before < reg) {
        SetBit(af_register.low, CARRY_FLAG);
    }

	int16_t htest = (before & 0xF) - (reg & 0xF);

	if (htest < 0) {
        SetBit(af_register.low, HALF_CARRY_FLAG);
    }

    clocks.m_cycles += 1;
    clocks.t_cycles += 4;
}

void CPU::And8Bit(uint8_t reg) {
    af_register.high &= reg;

    af_register.low = 0;
    if (af_register.high == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }

    SetBit(af_register.low, HALF_CARRY_FLAG);
    clocks.m_cycles += 1;
    clocks.t_cycles += 4;
}

void CPU::Or8Bit(uint8_t reg) {
    af_register.high |= reg;
    af_register.low = 0;
    if (af_register.high == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }

    clocks.m_cycles += 1;
    clocks.t_cycles += 4;
}

void CPU::Xor8Bit(uint8_t reg) {
    af_register.high ^= reg;
    af_register.low = 0;
    if (af_register.high == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }
    clocks.m_cycles += 1;
    clocks.t_cycles += 4;

}

void CPU::Cmp8Bit(uint8_t reg) {
    uint8_t res = af_register.high - reg;
    af_register.low = 0;

    if (res == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    }

	SetBit(af_register.low, SUBSTRACT_FLAG);

	if (af_register.high < reg) {
        SetBit(af_register.low, CARRY_FLAG);
    }

	int8_t htest = af_register.high & 0xF;
	htest -= (reg & 0xF);

	if (htest < 0) {
        SetBit(af_register.low, HALF_CARRY_FLAG);
    }

    clocks.m_cycles += 1;
    clocks.t_cycles += 4;
}

void CPU::Inc8Bit(uint8_t &reg) {
    uint8_t before = reg;
    reg++;

	if (reg == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    } else {
        ClearBit(af_register.low, ZERO_FLAG);
    }

    ClearBit(af_register.low, SUBSTRACT_FLAG);

	if ((before & 0xF) == 0xF) {
        SetBit(af_register.low, HALF_CARRY_FLAG);
    } else {
        ClearBit(af_register.low, HALF_CARRY_FLAG);
    }

    clocks.m_cycles += 1;
    clocks.t_cycles += 4;
}

void CPU::Dec8Bit(uint8_t &reg) {
    uint8_t before = reg;
    reg--;

    if (reg == 0) {
        SetBit(af_register.low, ZERO_FLAG);
    } else {
        ClearBit(af_register.low, ZERO_FLAG);
    }

    SetBit(af_register.low, SUBSTRACT_FLAG);

    if ((before & 0x0F) == 0) {
        SetBit(af_register.low, HALF_CARRY_FLAG);
    } else {
        ClearBit(af_register.low, HALF_CARRY_FLAG);
    }

    clocks.m_cycles += 1;
    clocks.t_cycles += 4;
}

/**< 16 bit ALU */
void CPU::Add16Bit(uint16_t reg) {

    uint16_t before = hl_register.pair;
    hl_register.pair += reg;

    ClearBit(af_register.low, SUBSTRACT_FLAG);

    if ((before + reg) > 0xFFFF) {
        SetBit(af_register.low, CARRY_FLAG);
    } else {
        ClearBit(af_register.low, CARRY_FLAG);
    }

    if (((before & 0xFF00) & 0xF) + ((reg >> 8) & 0xF)) {
        SetBit(af_register.low, HALF_CARRY_FLAG);
    } else {
        ClearBit(af_register.low, HALF_CARRY_FLAG);
    }

    clocks.m_cycles += 1;
    clocks.t_cycles += 8;
}

void CPU::AddSP16Bit() {
    uint16_t before = sp_register.pair;

    uint16_t nn = memory->ReadByteMemory(program_counter);
    program_counter++;
    sp_register.pair += nn;

    af_register.low = 0;
    if ((before + nn) > 0xFFFF) {
        SetBit(af_register.low, CARRY_FLAG);
    }

    af_register.low = 0;
    if (((before & 0xFF00) & 0xF) + ((nn >> 8) & 0xF)) {
        SetBit(af_register.low, HALF_CARRY_FLAG);
    }

    clocks.m_cycles += 2;
    clocks.t_cycles += 16;
}

void CPU::Inc16Bit(uint16_t &reg) {
    reg++;
    clocks.t_cycles += 1;
    clocks.m_cycles += 8;
}

void CPU::Dec16Bit(uint16_t &reg) {
    reg--;
    clocks.t_cycles += 1;
    clocks.m_cycles += 8;
}