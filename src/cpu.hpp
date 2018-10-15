#ifndef CPU_HPP
#define CPU_HPP

#include <exception>
#include "memory.hpp"

const int ZERO_FLAG = 7; 
const int SUBSTRACT_FLAG = 6;
const int HALF_CARRY_FLAG = 5;
const int CARRY_FLAG = 4;

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
        void ExecuteInstruction(uint8_t opcode);
        void FetchAndDispatch();
        void Diagnostics();

    private:
        Register af_register, bc_register, de_register, hl_register; /**< General purpose register */
        Register sp_register; /**< Stack pointer register, no need for low/high access */
        uint16_t program_counter; /**< Program counter register */
        
        Memory memory;
        Timer timer;
        int halt;
        int stop;     
        
        /**< Methods used for flag register */
        void SetFlag(uint8_t flag);
        int TestFlag(uint8_t flag);
        void ClearFlag(uint8_t flag);

        // Miscellaneous opcodes
        void NOP();
        void HALT();
        void STOP();
        void DI();
        void EI();

        // 8 bit loads
        void LD8_r_nn(uint8_t &reg);
        void LD8_r1_r2(uint8_t &reg1, uint8_t &reg2);
        void LD8_r1_mem(uint8_t &reg1, uint16_t address);
        void LD8_mem_r1(uint16_t address, uint8_t reg1);

        // 16 bit loads
        void LD16_r_nn(uint16_t &reg);
};  


#endif