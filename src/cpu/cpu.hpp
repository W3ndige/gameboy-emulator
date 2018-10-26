#ifndef CPU_HPP
#define CPU_HPP

#include <iostream>
#include <exception>
#include "../memory/memory.hpp"
#include "../helper/helper.hpp"

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
        CPU(Memory *mem);
        bool Init();

        void FetchAndDispatch();
        void ExecuteInstruction(uint8_t opcode);
        void ExecuteExtendedInstruction(uint8_t opcode);

        void ArtificialJump(int offset);
        unsigned int GetLastOpcodeTime();
        int Breakpoint(uint16_t pc);
        void Diagnostics();

    private:
        Register af_register, bc_register, de_register, hl_register; /**< General purpose register */
        Register sp_register; /**< Stack pointer register, no need for low/high access */
        uint16_t program_counter; /**< Program counter register */
        
        Memory *memory;
        Timer timer;
        int halt;
        int stop;

        void PrintFlags();

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
        void Add8Bit(uint8_t reg, int add_carry);
        void Sub8Bit(uint8_t &reg, int sub_carry);
        void And8Bit(uint8_t &reg);
        void Or8Bit(uint8_t &reg);
        void Xor8Bit(uint8_t &reg);
        void Cmp8Bit(uint8_t &reg);
        void Inc8Bit(uint8_t &reg);
        void Dec8Bit(uint8_t &reg);

        // 16 bit ALU
        void Add16Bit(uint16_t &reg);
        void AddSP16Bit();
        void Inc16Bit(uint16_t &reg);
        void Dec16Bit(uint16_t &reg);

        // Extended instruction set
        void Swap(uint8_t &reg);
        void RL(uint8_t &reg);

        // Misc instructions
        void DAA();

        // Jumps
        void JUMP(uint8_t flag, int condition, int use_condition);
        void JUMP_IMM(uint8_t flag, int condition, int use_condition);

        // Calls
        void CALL(uint8_t flag, int condition, int use_condition);

        void RET(uint8_t flag, int condition, int use_condition);

};  


#endif