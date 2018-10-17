#ifndef CPU_HPP
#define CPU_HPP

#include <exception>
#include "memory.hpp"

const int ZERO_FLAG = 7;  // Z
const int SUBSTRACT_FLAG = 6; // N
const int HALF_CARRY_FLAG = 5; // H
const int CARRY_FLAG = 4; // C

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
        void FetchAndDispatch();
        void ExecuteInstruction(uint8_t opcode);
        void ExecuteExtendedInstruction(uint8_t opcode);
        int Breakpoint(uint16_t pc);
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
        void SetBit(uint8_t &reg, uint8_t flag);
        int TestBit(uint8_t &reg, uint8_t flag);
        void ClearBit(uint8_t &reg, uint8_t flag);

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
        void Push(uint16_t &reg);
        void Pop(uint16_t &reg);

        // 8 bit ALU
        void Add8Bit(uint8_t &reg, int add_carry);
        void Sub8Bit(uint8_t &reg, int sub_carry);
        void And8Bit(uint8_t &reg);
        void Or8Bit(uint8_t &reg);
        void Xor8Bit(uint8_t &reg);
        void Cmp8Bit(uint8_t &reg);
        void Inc8Bit(uint8_t &reg);
        void Dec8Bit(uint8_t &reg);

};  


#endif