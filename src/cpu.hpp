#ifndef CPU_HPP
#define CPU_HPP

#include <stdio.h>
#include "memory.hpp"

#define ZERO_FLAG 7; 
#define SUBSTRACT_FLAG 6;
#define HALF_CARRY_FLAG 5;
#define CARRY_FLAG 4;

union Register { 
    uint16_t pair;
    struct {
        uint8_t low;
        uint8_t high;
    };
};

struct Timer {
    unsigned int m_cycles; /**< Machine cycle, usually taking about 3 and 6 clock periods - t_cycles */
    unsigned int t_cycles;  /**< Clock periods */
};

class CPU {
    public:
        CPU();
        void ExecuteInstruction();
        void Diagnostics();

    private:
        Register af_register, bc_register, de_register, hl_register; /**< General purpose register */
        Register stack_pointer; /**< Stack pointer register, no need for low/high access */
        uint16_t program_counter; /**< Program counter register */
        
        Memory memory;
        Timer timer;
        int halt;
        int stop;      

        // Miscellaneous Opcodes
        void Nop();
        void Halt();
        void Stop();
        void Di();
        void Ei();
};  


#endif