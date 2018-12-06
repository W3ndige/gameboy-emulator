/**
 * @file helper.hpp
 * @author w3ndige (w3ndige@protonmail.com)
 * @brief Helper functions used accross the project.
 * @version 0.1
 * @date 2018-12-06
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef HELPER_HPP
#define HELPER_HPP

#include "../memory/memory.hpp"

/**
 * @brief Set the flag-th bit of object.
 * 
 * @param reg Value to set the bit. 
 * @param flag Number of bit to set. 
 */

void SetBit(uint8_t &reg, uint8_t flag);

/**
 * @brief Test the flag-th bit of object.
 * 
 * @param data Value from which we test a bit.
 * @param flag NUmber of bit to test
 * @return int Is a bit set or not.
 */

int TestBit(uint8_t data, uint8_t flag);

/**
 * @brief Clear the flag-th bit of object.
 * 
 * @param reg Value to set the clear. 
 * @param flag Number of bit to clear. 
 */

void ClearBit(uint8_t &reg, uint8_t flag);

#endif