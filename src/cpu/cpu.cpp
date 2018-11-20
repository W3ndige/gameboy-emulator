#include "cpu.hpp"

CPU::CPU(Memory *mem) {
    memory = mem;
}

bool CPU::Init(std::string rom_file, bool exit_on_inifite) {
    program_counter = 0x00;
    clocks.m_cycles = 0;
    clocks.t_cycles = 0;
    clocks.divide_counter = 0;
    interupts = false;
    pending_interupt_disabled = false;
    pending_interupt_enabled = false;
    halt = 0;
    stop = 0;
    exit_on_inifite_loop = exit_on_inifite;
    
    /**< If booting failed or stopped, initialize register with
     *   values that bootstrap sets */
    if (!memory->IsBooting()) {
        program_counter = 0x100;

        af_register.pair = 0x01B0;
        bc_register.pair = 0x0013;
        de_register.pair = 0x00D8; 
        hl_register.pair = 0x014D;
        sp_register.pair = 0xFFFE;

        /**< Place power up sequence values in memory */
        memory->WriteByteMemory(0xFF05, 0x00);
        memory->WriteByteMemory(0xFF06, 0x00);
        memory->WriteByteMemory(0xFF07, 0x00);
        memory->WriteByteMemory(0xFF10, 0x80);
        memory->WriteByteMemory(0xFF11, 0xBF);
        memory->WriteByteMemory(0xFF12, 0xF3);
        memory->WriteByteMemory(0xFF14, 0xBF);
        memory->WriteByteMemory(0xFF16, 0x3F);
        memory->WriteByteMemory(0xFF17, 0x00);
        memory->WriteByteMemory(0xFF19, 0xBF);
        memory->WriteByteMemory(0xFF1A, 0x7F);
        memory->WriteByteMemory(0xFF1B, 0xFF);
        memory->WriteByteMemory(0xFF1C, 0x9F);
        memory->WriteByteMemory(0xFF1E, 0xBF);
        memory->WriteByteMemory(0xFF20, 0xFF);
        memory->WriteByteMemory(0xFF21, 0x00);
        memory->WriteByteMemory(0xFF22, 0x00);
        memory->WriteByteMemory(0xFF23, 0xBF);
        memory->WriteByteMemory(0xFF24, 0x77);
        memory->WriteByteMemory(0xFF25, 0xF3);
        memory->WriteByteMemory(0xFF26, 0xF1);
        memory->WriteByteMemory(0xFF40, 0x91);
        memory->WriteByteMemory(0xFF42, 0x00);
        memory->WriteByteMemory(0xFF43, 0x00);
        memory->WriteByteMemory(0xFF45, 0x00);
        memory->WriteByteMemory(0xFF47, 0xFC);
        memory->WriteByteMemory(0xFF48, 0xFF);
        memory->WriteByteMemory(0xFF49, 0xFF);
        memory->WriteByteMemory(0xFF4A, 0x00);
        memory->WriteByteMemory(0xFF4B, 0x00);
        memory->WriteByteMemory(0xFFFF, 0x00);
        memory->WriteByteMemory(0x8AAA, 0xFF);
        memory->WriteByteMemory(0x8AAB, 0xFF);
        memory->WriteByteMemory(0x8AAC, 0xFF);
        memory->WriteByteMemory(0x8AAD, 0xFF);
    }

    /**< Try reading cartridge ROM into main memory */
    return memory->LoadCartridge(rom_file);
}

void CPU::FetchAndDispatch() {
    //printf("0x%04x\n", program_counter);
    uint8_t opcode = memory->ReadByteMemory(program_counter);
    program_counter++;
    ExecuteInstruction(opcode);
}

uint16_t CPU::GetProgramCounter() const {
    return program_counter;
}

uint16_t CPU::GetStackPointer() const {
    return sp_register.pair;
}


bool CPU::IsClockEnabled() {
     return TestBit(memory->ReadByteMemory(TMC), 2) ? true : false;
}

uint8_t CPU::GetClockFrequency() {
    return memory->ReadByteMemory(TMC) & 0x3;
}

void CPU::SetClockFrequency() {
   uint8_t frequency = GetClockFrequency( ) ;
   switch (frequency) {
     case 0: clocks.t_cycles = 1024; break;
     case 1: clocks.t_cycles = 16; break;
     case 2: clocks.t_cycles = 64; break;
     case 3: clocks.t_cycles = 256; break;
   }
}

void CPU::DividerRegister(int cycles) {
   clocks.divide_counter += cycles;
   if (clocks.divide_counter >= 255) {
        clocks.divide_counter = 0;
        uint8_t tmp = memory->ReadByteMemory(0xFF04);
        tmp++;
        memory->PrivilagedByteWrite(0xFF04, tmp);
   }
}

void CPU::UpdateTimer(int cycles) {
    DividerRegister(cycles);

    if (!IsClockEnabled()) {
        return;
    }

    clocks.t_cycles -= cycles;
    if (clocks.t_cycles <= 0) {
        SetClockFrequency();

        if (memory->ReadByteMemory(TIMA) == 255) {
            memory->WriteByteMemory(TIMA, memory->ReadByteMemory(TMA));
            RequestInterupt(2);
        }
        else {
            memory->WriteByteMemory(TIMA, memory->ReadByteMemory(TIMA) + 1);
        }
    }
}

void CPU::RequestInterupt(int id) {
    uint8_t req = memory->ReadByteMemory(0xFF0F);
    SetBit(req, id);
    memory->PrivilagedByteWrite(0xFF0F, req); 
}

void CPU::DoInterupts() {
    if (!interupts) {
        return;
    }
    uint8_t req = memory->ReadByteMemory(0xFF0F);
    uint8_t enabled = memory->ReadByteMemory(0xFFFF);
    if (req > 0) {

        for (int i = 0 ; i < 5; i++) {
            if (TestBit(req, i)) {
                if (TestBit(enabled, i)) {
                    ServiceInterupt(i) ;
                }
            }
        }
    }
}

void CPU::ServiceInterupt(int id) {
   interupts = false;
   uint8_t req = memory->ReadByteMemory(0xFF0F);
   ClearBit(req, id);
   memory->WriteByteMemory(0xFF0F, req) ;

   Push(program_counter);

   switch (id) {
     case 0: program_counter = 0x40; break;
     case 1: program_counter = 0x48; break;
     case 2: program_counter = 0x50; break;
     case 4: program_counter = 0x60; break;
   }
}

void CPU::ArtificialJump(int offset) {
    program_counter += offset;
}

unsigned int CPU::GetLastOpcodeTime() {
    return clocks.t_cycles;
}

int CPU::Breakpoint(uint16_t pc) {
    if (pc == program_counter) {
        return 1;
    }
    return 0;
}

void CPU::Diagnostics() {
    printf("\033[31;1mPC\033[0m\t0x%04x\n", program_counter);
    printf("\033[31;1mSP\033[0m\t0x%04x\n", sp_register.pair);
    printf("\033[31;1mAF\033[0m\t0x%04x\n", af_register.pair);
    printf("\033[31;1mBC\033[0m\t0x%04x\n", bc_register.pair);
    printf("\033[31;1mDE\033[0m\t0x%04x\n", de_register.pair);
    printf("\033[31;1mHL\033[0m\t0x%04x\n", hl_register.pair);
    PrintFlags();
    memory->DumpMemory();
}

void CPU::PrintFlags() {
    printf("\033[31;1mZERO FLAG\033[0m\t%d\n",          TestBit(af_register.low, 7));
    printf("\033[31;1mSUBSTRACT FLAG\033[0m\t%d\n",     TestBit(af_register.low, 6));
    printf("\033[31;1mHALF CARRY FLAG\033[0m\t%d\n",    TestBit(af_register.low, 5));
    printf("\033[31;1mCARRY FLAG\033[0m\t%d\n",         TestBit(af_register.low, 4));

}