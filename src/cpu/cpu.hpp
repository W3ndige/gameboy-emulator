/**
 * @file cpu.hpp
 * @author W3ndige (w3ndige@protonmail.com)
 * @brief File containing logic of the CPU.
 * @version 0.1
 * @date 2018-12-07
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef CPU_HPP
#define CPU_HPP

#include <iostream>
#include <exception>
#include "../memory/memory.hpp"
#include "../helper/helper.hpp"

const int ZERO_FLAG         = 7; /**< Zero flag */
const int SUBSTRACT_FLAG    = 6; /**< Substract flag */
const int HALF_CARRY_FLAG   = 5; /**< Half carry flag */
const int CARRY_FLAG        = 4; /**< Carry flag */

const int TIMA  = 0xFF05;
const int TMA   = 0xFF06;
const int TMC   = 0xFF07; 

/**
 * @brief Union of 16 bit size, allowing 
 * to access lower and higher bits. 
 * 
 */

union Register { 
    uint16_t pair;      /**< Value of whole register */
    struct {
        uint8_t low;    /**< Lower 8 bits of regiser */
        uint8_t high;   /**< Higher 8 bits of register */
    };
};

/**
 * @brief Structure containing information about the clock. 
 * 
 */

struct Clocks {
    int m_cycles; /**< Machine cycle, usually taking about 3 and 6 clock periods - t_cycles */
    int t_cycles;  /**< Clock periods */
    int divide_counter;
};

/**
 * @brief Class containing logic behind the CPU. 
 * 
 */

class CPU {
    public:

        /**
         * @brief Construct a new CPU object.
         * 
         * @param mem Pointer to memory object.  
         */

        CPU(Memory *mem);

        /**
         * @brief Initialize the CPU.
         * 
         * @param rom_file ROM file to load.
         * @param exit_on_inifite Do we exit on ininite loop
         * @return true Initialization suceeded. 
         * @return false Initialization failed. 
         */

        bool Init(std::string rom_file, bool exit_on_inifite);

        /**
         * @brief Fetch the opcode, pass to 
         * execution function and increment PC.
         * 
         */

        void FetchAndDispatch();

        /**
         * @brief Execute instruction.
         * 
         * @param opcode Opcode to execute. 
         */

        void ExecuteInstruction(uint8_t opcode);

        /**
         * @brief Execute extended instruction.
         * 
         * @param opcode Second byte of extended instruction set opcode. 
         */

        void ExecuteExtendedInstruction(uint8_t opcode);

        /**
         * @brief Get the Program Counter value.
         * 
         * @return uint16_t Value of PC.
         */

        uint16_t GetProgramCounter() const;

        /**
         * @brief Get the Stack Pointer value.
         * 
         * @return uint16_t Value of SP.
         */

        uint16_t GetStackPointer() const;

        /**
         * @brief Is clock enabled?
         * 
         * @return true True if clock is enabled. 
         * @return false False if clock is disabled.
         */

        bool IsClockEnabled();

        /**
         * @brief Get the Clock Frequency.
         * 
         * @return uint8_t Frequency of the clock.
         */

        uint8_t GetClockFrequency();

        /**
         * @brief Set the Clock Frequency.
         * 
         */

        void SetClockFrequency();

        /**
         * @brief TODO
         * 
         * @param cycles Number of cycles that last instruction took.
         */

        void DividerRegister(int cycles);

        /**
         * @brief Update timer register. 
         * 
         * @param cycles Number of cycles that last instruction took.
         */
        
        void UpdateTimer(int cycles);

        bool interupts;                 /**< Are interrupts enabled */
        bool pending_interupt_enabled;  /**< Is pending interrupt enabled */
        bool pending_interupt_disabled; /**< Is pending interrupt disabled */

        /**
         * @brief Request an interrupt.
         * 
         * @param id Id of interrupt.
         */

        void RequestInterupt(int id);

        /**
         * @brief Iterate over interrupt register and service interrupt. 
         * 
         */

        void DoInterupts();

        /**
         * @brief Service specified interrupt.
         * 
         * @param interupt Interrupt to service. 
         */

        void ServiceInterupt(int interupt);

        /**< Debugging type of instructions */

        /**
         * @brief Get the Timer value.
         * 
         * @return unsigned int Value of timer.
         */

        unsigned int GetTimer();

        /**
         * @brief Set breakpoint in CPU.
         * 
         * @param pc Program counter at which we set a breakpoint.
         * @return int Return 1 if encountered breakpoint, else 0.
         */


        int Breakpoint(uint16_t pc);

        /**
         * @brief Show the value in registers.
         * 
         */

        void Diagnostics();

    private:
        Register af_register, bc_register,
                 de_register, hl_register;  /**< General purpose register */
     
        Register sp_register;       /**< Stack pointer register */         

        uint16_t program_counter;   /**< Program counter register */
        
        
        Memory *memory;             /** Pointer to memory object */  
        Clocks clocks;              /**< Clocks structure */
        bool exit_on_inifite_loop;  /**< Do we exit on infinite loop */
        int halt;                   /** Halt the CPU */
        int stop;                   /** Stop the CPU */

        /**
         * @brief Print the current flags. 
         * 
         */

        void PrintFlags();

        /**
         * @brief No operation.
         * 
         */

        void NOP();

        /**
         * @brief Power down CPU until an interrupt occurs.
         * 
         */

        void HALT();

        /**
         * @brief Halt CPU & LCD display until button pressed.
         * 
         */

        void STOP();

        /**
         * @brief This instruction disables interrupts but not immediately.
         *  Interrupts are disabled after instruction after DI is executed
         * 
         */

        void DI();

        /**
         * @brief This intruction enables interrupts but not immediately. 
         * Interrupts are enabled after instruction after EI is executed.
         * 
         */

        void EI();

        // 8 bit loads

        /**
         * @brief Load immediate value into the register.
         * 
         * @param reg Address of passed register.
         */

        void LD8_r_nn(uint8_t &reg);

        /**
         * @brief Load register 2 into register 1.
         * 
         * @param reg1 Address of passed register 1.
         * @param reg2 Address of passed register 2.
         */

        void LD8_r1_r2(uint8_t &reg1, uint8_t reg2);

        /**
         * @brief Load the value from memory into register.
         * 
         * @param reg Address of passed register.
         * @param address Address in memory.
         */

        void LD8_r1_mem(uint8_t &reg, uint16_t address);

        /**
         * @brief Put the value from register to memory. 
         * 
         * @param address Address in memory.
         * @param reg Address of passed register.
         */

        void LD8_mem_r1(uint16_t address, uint8_t reg);

        // 16 bit loads

        /**
         * @brief Put the 16 bit immediate value into register pair.
         * 
         * @param reg Address of register pair.
         */

        void LD16_r_nn(uint16_t &reg);

        /**
         * @brief Push the value of register onto the stack.
         * 
         * @param reg Address of register pair.
         */

        void Push(uint16_t &reg);

        /**
         * @brief Pop the value from the stack into the register pair.
         * 
         * @param reg Address of register pair.
         */

        void Pop(uint16_t &reg);

        /**
         * @brief Pop AF register.
         * 
         */

        void PopAF();

        // 8 bit ALU

        /**
         * @brief Add 8 bit register to the A register.
         * 
         * @param reg Value of register.
         */

        void Add8Bit(uint8_t reg);

        /**
         * @brief Add 8 bit register to the A register, 
         * including carry flag.
         * 
         * @param reg Value of register.
         */

        void Adc8Bit(uint8_t reg);

        /**
         * @brief Substract 8 bit register from the A register.
         * 
         * @param reg Value of register.
         */

        void Sub8Bit(uint8_t reg);

        /**
         * @brief Add 8 bit register to the A register, 
         * including carry flag.
         * 
         * @param reg Value of register.
         */


        void Subc8Bit(uint8_t reg);

        /**
         * @brief Perform bitwise AND operation on
         * A register and passed register.
         * 
         * @param reg Value of register.
         */

        void And8Bit(uint8_t reg);

       /**
         * @brief Perform bitwise OR operation on
         * A register and passed register.
         * 
         * @param reg Value of register.
         */

        void Or8Bit(uint8_t reg);

       /**
         * @brief Perform bitwise XOR operation on
         * A register and passed register.
         * 
         * @param reg Value of register.
         */

        void Xor8Bit(uint8_t reg);

        /**
         * @brief Perform comparision betwenA register and passed
         * register. Set flags accordingly.
         * 
         * @param reg Value of passed register. 
         */

        void Cmp8Bit(uint8_t reg);

       /**
         * @brief Increment passed register.
         * 
         * @param reg Value of passed register. 
         */

        void Inc8Bit(uint8_t &reg);

       /**
         * @brief Decrement passed register.
         * 
         * @param reg Value of passed register. 
         */

        void Dec8Bit(uint8_t &reg);

        // 16 bit ALU

       /**
         * @brief Add the value from passed register to register HL.
         * 
         * @param reg Value of passed register. 
         */

        void Add16Bit(uint16_t reg);


       /**
         * @brief Add the value from passed register to register SP.
         * 
         * @param reg Value of passed register. 
         */

        void AddSP16Bit();

        /**
         * @brief Increment 16 bit register.
         * 
         * @param reg Address of passed register. 
         */

        void Inc16Bit(uint16_t &reg);

        /**
         * @brief Decrement 16 bit register.
         * 
         * @param reg Address of passed register. 
         */

        void Dec16Bit(uint16_t &reg);

        // Extended instruction set

        /**
         * @brief Rotate n left. Old bit 7 to Carry flag.
         * 
         * @param reg Address of passed register.
         */

        void RLC(uint8_t &reg);

        /**
         * @brief Rotate n right. Old bit 0 to Carry flag.
         * 
         * @param reg Address of passed register.
         */
        
        void RRC(uint8_t &reg);

        /**
         * @brief Rotate n left through Carry flag.
         * 
         * @param reg Address of passed register.
         */

        void RL(uint8_t &reg);

        /**
         * @brief Rotate n right through Carry flag.
         * 
         * @param reg Address of passed register.
         */

        void RR(uint8_t &reg);

        /**
         * @brief Shift n left into Carry. LSB of n set to 0.
         * 
         * @param reg Address of passed register.
         */

        void SLA(uint8_t &reg);

        /**
         * @brief Shift n right into Carry. MSB doesn't change.
         * 
         * @param reg Address of passed register.
         */

        void SRA(uint8_t &reg);

        /**
         * @brief Swap lower and upper 4 bits of an register.
         * 
         * @param reg  Address of passed register.
         */

        void Swap(uint8_t &reg);

        /**
         * @brief Shift n right into Carry. MSB set to 0.
         * 
         * @param reg  Address of passed register.
         */

        void SRL(uint8_t &reg);

        /**
         * @brief Test bit b in register r.
         * 
         * @param reg Address of passed register.
         * @param bit Index of bit to test.
         */

        void Bit(uint8_t &reg, uint8_t bit);

        /**
         * @brief Reset bit b in register r.
         * 
         * @param reg Address of passed register.
         * @param bit Index of bit to reset.
         */

        void Res(uint8_t &reg, uint8_t bit);

        /**
         * @brief Set bit b in register r.
         * 
         * @param reg Address of passed register.
         * @param bit Index of bit to set.
         */

        void Set(uint8_t &reg, uint8_t bit);

        /**
         * @brief Decimal adjust register A.
         * 
         */

        void DAA();

        /**
         * @brief Rotate A left. Old bit 7 to Carry flag.
         * 
         */

        void RLCA();

        /**
         * @brief Rotate A left through Carry flag.
         * 
         */

        void RLA();

        /**
         * @brief Rotate A right. Old bit 0 to Carry flag.
         * 
         */

        void RRCA();

        /**
         * @brief Rotate A right through Carry flag.
         * 
         */

        void RRA();

        // Jumps

        /**
         * @brief Set program counter to the immediate value.
         * 
         * @param flag Flag is used in condition.
         * @param condition Condition used in jump.
         * @param use_condition Are we using condition.
         */

        void JUMP(uint8_t flag, int condition, int use_condition);

        /**
         * @brief Increment program counter by the immediate value.
         * 
         * @param flag Flag is used in condition.
         * @param condition Condition used in jump.
         * @param use_condition Are we using condition.
         */

        void JUMP_IMM(uint8_t flag, int condition, int use_condition);

        // Calls

        /**
         * @brief Call the function at address used in immediate value.
         * Push current PC at the stack.
         * 
         * @param flag Flag is used in condition.
         * @param condition Condition used in jump.
         * @param use_condition Are we using condition.
         */

        void CALL(uint8_t flag, int condition, int use_condition);

        /**
         * @brief Push present address onto stack. 
         * Jump to address $0000 + n.
         * 
         * @param arg Value of n.
         */

        void RST(uint8_t arg);

        /**
         * @brief Return from function, 
         * Set the PC to the popped value from stack.
         * 
         * @param flag Flag is used in condition.
         * @param condition Condition used in jump.
         * @param use_condition Are we using condition.
         */

        void RET(uint8_t flag, int condition, int use_condition);

};  


#endif