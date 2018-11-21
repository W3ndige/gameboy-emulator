#include "helper.hpp"

void SetBit(uint8_t &reg, uint8_t flag) {
    reg |= 1 << flag;
}

int TestBit(uint8_t data, uint8_t flag) {
	uint8_t mask = 1 << flag;
	return (data & mask) ? 1 : 0;
}

void ClearBit(uint8_t &reg, uint8_t flag) {
    reg &= ~(1 << flag);
}