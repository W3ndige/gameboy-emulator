#ifndef HELPER_HPP
#define HELPER_HPP

#include "../memory/memory.hpp"

/**< Methods used for setting bits in register */
void SetBit(uint8_t &reg, uint8_t flag);
int TestBit(uint8_t data, uint8_t flag);
void ClearBit(uint8_t &reg, uint8_t flag);

#endif