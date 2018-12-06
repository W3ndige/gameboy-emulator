#ifndef CPU_HPP
#define CPU_HPP

#include <iostream>
#include <exception>
#include "../memory/memory.hpp"
#include "../helper/helper.hpp"

const int ZERO_FLAG         = 7; // Z
const int SUBSTRACT_FLAG    = 6; // N
const int HALF_CARRY_FLAG   = 5; // H
const int CARRY_FLAG        = 4; // C

const int TIMA  = 0xFF05;
const int TMA   = 0xFF06;
const int TMC   = 0xFF07; 

union Register { 
    uint16_t pair;
    struct {
        uint8_t low;
        uint8_t high;
    };
};

struct Clocks {
    int m_cycles; /**< Machine cycle, usually taking about 3 and 6 clock periods - t_cycles */
    int t_cycles;  /**< Clock periods */
    int divide_counter;
};

class CPU {
    public:
        CPU(Memory *mem);
        /**< CPU type of instructions */
        bool Init(std::string rom_file, bool exit_on_inifite);
        void FetchAndDispatch();
        void ExecuteInstruction(uint8_t opcode);
        void ExecuteExtendedInstruction(uint8_t opcode);
        uint16_t GetProgramCounter() const;
        uint16_t GetStackPointer() const;

        /**< Timer type of instructions */
        bool IsClockEnabled();
        uint8_t GetClockFrequency();
        void SetClockFrequency();
        void DividerRegister(int cycles);
        void UpdateTimer(int cycles);

        /**< Interrupts */
        bool interupts;
        bool pending_interupt_enabled;
        bool pending_interupt_disabled;
        void RequestInterupt(int id);
        void DoInterupts();
        void ServiceInterupt(int interupt);

        /**< Debugging type of instructions */
        unsigned int GetTimer();
        int Breakpoint(uint16_t pc);
        void Diagnostics();

    private:
        /**< General purpose register */
        Register af_register, bc_register, 
                 de_register, hl_register;

        /**< Stack pointer register */         
        Register sp_register;

        /**< Program counter register */
        uint16_t program_counter;
        
        
        Memory *memory;
        Clocks clocks;
        bool exit_on_inifite_loop;
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
        void LD8_r1_r2(uint8_t &reg1, uint8_t reg2);
        void LD8_r1_mem(uint8_t &reg, uint16_t address);
        void LD8_mem_r1(uint16_t address, uint8_t reg);

        // 16 bit loads
        void LD16_r_nn(uint16_t &reg);
        void Push(uint16_t &reg);
        void Pop(uint16_t &reg);
        void PopAF();

        // 8 bit ALU
        void Add8Bit(uint8_t reg);
        void Adc8Bit(uint8_t reg);
        void Sub8Bit(uint8_t reg);
        void Subc8Bit(uint8_t reg);
        void And8Bit(uint8_t reg);
        void Or8Bit(uint8_t reg);
        void Xor8Bit(uint8_t reg);
        void Cmp8Bit(uint8_t reg);
        void Inc8Bit(uint8_t &reg);
        void Dec8Bit(uint8_t &reg);

        // 16 bit ALU
        void Add16Bit(uint16_t reg);
        void AddSP16Bit();
        void Inc16Bit(uint16_t &reg);
        void Dec16Bit(uint16_t &reg);

        // Extended instruction set
        void RLC(uint8_t &reg);
        void RRC(uint8_t &reg);
        void RL(uint8_t &reg);
        void RR(uint8_t &reg);
        void SLA(uint8_t &reg);
        void SRA(uint8_t &reg);
        void Swap(uint8_t &reg);
        void SRL(uint8_t &reg);
        void Bit(uint8_t &reg, uint8_t bit);
        void Res(uint8_t &reg, uint8_t bit);
        void Set(uint8_t &reg, uint8_t bit);

        // Misc instructions
        void DAA();
        void RLCA();
        void RLA();
        void RRCA();
        void RRA();

        // Jumps
        void JUMP(uint8_t flag, int condition, int use_condition);
        void JUMP_IMM(uint8_t flag, int condition, int use_condition);

        // Calls
        void CALL(uint8_t flag, int condition, int use_condition);
        void RST(uint8_t arg);
        void RET(uint8_t flag, int condition, int use_condition);

};  


#endif