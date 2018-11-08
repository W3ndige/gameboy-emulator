#ifndef DISASSEMBLER_HPP
#define DISASSEMBLER_HPP

#include <string>
#include <fstream>
#include <cstdint>

struct OpcodeInfo {
    std::string disassembled;
    uint16_t jump_offset;
};

struct OpcodeInfo opcode_names[256] = {
    {"NOP",         0x00},
    {"LD BC,nn",    0x02},
    {"LD (BC),A",   0x00},       
    {"INC BC",      0x00},
    {"INC B",       0x00},
    {"DEC B",       0x00},
    {"LD B,n",      0x01},
    {"RLCA",        0x00},
    {"LD (nn),SP",  0x02},
    {"ADD HL,BC",   0x00},
    {"LD A,(BC)",   0x00},
    {"DEC BC",      0x00},
    {"INC C",       0x00},
    {"DEC C",       0x00},
    {"LD C,n",      0x01},
    {"RRCA",        0x00},
    {"STOP",        0x00},
    {"LD DE,nn",    0x02},
    {"LD (DE),A",   0x00},
    {"INC DE",      0x00},
    {"INC D",       0x00},
    {"DEC D",       0x00},
    {"LD D,n",      0x01},
    {"RLA",         0x00},
    {"JR n",        0x01},
    {"ADD HL,DE",   0x00},
    {"LD A,(DE)",   0x00},
    {"DEC DE",      0x00},
    {"INC E",       0x00},
    {"DEC E",       0x00},
    {"LD E,n",      0x01},
    {"RRA",         0x00},
    {"JR NZ,n",     0x01},
    {"LD HL,nn",    0x02},
    {"LD (HL+),A",  0x00},
    {"INC HL",      0x00},
    {"INC H",       0x00},
    {"DEC H",       0x00},
    {"LD H,n",      0x01},
    {"DAA",         0x00},
    {"JR Z,n",      0x01},
    {"ADD HL,HL",   0x00},
    {"LD A,(HL)",   0x00},
    {"DEC HL",      0x00},
    {"INC L",       0x00},
    {"DEC L",       0x00},
    {"LD L,n",      0x01},
    {"CPL",         0x00},
    {"JR NC,n",     0x01},
    {"LD SP,nn",    0x02},
    {"LD (HL-),A",  0x00},
    {"INC SP",      0x00},
    {"INC (HL)",    0x00},
    {"DEC (HL)",    0x00},
    {"LD (HL),n",   0x01},
    {"SCF",         0x00},
    {"JR C,n",      0x01},
    {"ADD HL,SP",   0x00},
    {"LD A,(HL)",   0x00},
    {"DEC SP",      0x00},
    {"INC A",       0x00},
    {"DEC A",       0x00},
    {"LDA,n",       0x01},
    {"CCF",         0x00},
    {"LD B,B",      0x00},
    {"LD B,C",      0x00},
    {"LD B,D",      0x00},
    {"LD B,E",      0x00},
    {"LD B,H",      0x00},
    {"LD B,L",      0x00},
    {"LD B,(HL)",   0x00},
    {"LD B,A",      0x00},
    {"LD C,B",      0x00},
    {"LD C,C",      0x00},
    {"LD C,D",      0x00},
    {"LD C,E",      0x00},
    {"LD C,H",      0x00},
    {"LD C,L",      0x00},
    {"LD C,(HL)",   0x00},
    {"LD C,A",      0x00},
    {"LD D,C",      0x00},
    {"LD D,B",      0x00},
    {"LD D,D",      0x00},
    {"LD D,E",      0x00},
    {"LD D,H",      0x00},
    {"LD D,L",      0x00},
    {"LD D,(HL)",   0x00},
    {"LD D,A",      0x00},
    {"LD E,B",      0x00},
    {"LD E,C",      0x00},
    {"LD E,D",      0x00},
    {"LD E,E",      0x00},
    {"LD E,H",      0x00},
    {"LD E,L",      0x00},
    {"LD E,(HL)",   0x00},
    {"LD E,A",      0x00},
    {"LD H,B",      0x00},
    {"LD H,C",      0x00},
    {"LD H,D",      0x00},
    {"LD H,E",      0x00},
    {"LD H,H",      0x00},
    {"LD H,L",      0x00},
    {"LD H,(HL)",   0x00},
    {"LD L,B",      0x00},
    {"LD H,A",      0x00},
    {"LD L,C",      0x00},
    {"LD L,D",      0x00},
    {"LD L,E",      0x00},
    {"LD L,H",      0x00},
    {"LD L,L",      0x00},
    {"LD L,(HL)",   0x00},
    {"LD L,A",      0x00},
    {"LD (HL),B",   0x00},
    {"LD (HL),C",   0x00},    
    {"LD (HL),D",   0x00},    
    {"LD (HL),E",   0x00},    
    {"LD (HL),H",   0x00},    
    {"LD (HL),L",   0x00},    
    {"HALT",        0x00},
    {"LD (HL),A",   0x00},
    {"LD A,B",      0x00},
    {"LD A,C",      0x00},
    {"LD A,D",      0x00},
    {"LD A,E",      0x00},
    {"LD A,H",      0x00},
    {"LD A,L",      0x00},
    {"LD A,(HL)",   0x00},
    {"LD A,A",      0x00},
    {"ADD A,B",     0x00},
    {"ADD A,C",     0x00},
    {"ADD A,D",     0x00},
    {"ADD A,E",     0x00},
    {"ADD A,H",     0x00},
    {"ADD A,L",     0x00},
    {"ADD A,(HL)",  0x00},
    {"ADD A,A",     0x00},
    {"ADC A,B",     0x00},
    {"ADC A,C",     0x00},
    {"ADC A,D",     0x00},
    {"ADC A,E",     0x00},
    {"ADC A,H",     0x00},
    {"ADC A,L",     0x00},
    {"ADC A,(HL)",  0x00},
    {"ADC A,A",     0x00},
    {"SUB B",       0x00},
    {"SUB C",       0x00},       
    {"SUB D",       0x00},
    {"SUB E",       0x00},
    {"SUB H",       0x00},
    {"SUB L",       0x00},
    {"SUB (HL)",    0x00},
    {"SUB A",       0x00},
    {"SBC A,B",     0x00},
    {"SBC A,C",     0x00},
    {"SBC A,D",     0x00},
    {"SBC A,E",     0x00},
    {"SBC A,H",     0x00},
    {"SBC A,L",     0x00},
    {"SBC A,(HL)",  0x00},
    {"SBC A,A",     0x00},
    {"AND B",       0x00},
    {"AND C",       0x00},
    {"AND D",       0x00},
    {"AND E",       0x00},
    {"AND H",       0x00},
    {"AND L",       0x00},
    {"AND (HL)",    0x00},
    {"AND A",       0x00},
    {"XOR B",       0x00},
    {"XOR C",       0x00},
    {"XOR D",       0x00},
    {"XOR E",       0x00},
    {"XOR H",       0x00},
    {"XOR L",       0x00},
    {"XOR (HL)",    0x00},
    {"XOR A",       0x00},

    {"OR B",        0x00},
    {"OR C",        0x00},
    {"OR D",        0x00},
    {"OR E",        0x00},
    {"OR H",        0x00},
    {"OR L",        0x00},
    {"OR (HL)",     0x00},
    {"OR A",        0x00},
    {"CP B",        0x00},
    {"CP C",        0x00},
    {"CP D",        0x00},
    {"CP E",        0x00},
    {"CP H",        0x00},
    {"CP L",        0x00},
    {"CP (HL)",     0x00},
    {"CP A",        0x00},

    {"RET NZ",      0x00},
    {"POP BC",      0x00},
    {"JP NZ,nn",    0x02},
    {"JP nn",       0x02},
    {"CALL NZ,nn",  0x02},
    {"PUSH BC",     0x00},
    {"ADD A,n",     0x00},
    {"RST ",        0x00},
    {"RET Z",       0x00},
    {"RET",         0x00},
    {"JP Z,nn",     0x02},
    {"cb opcode",   0x00},
    {"CALL Z,nn",   0x02},
    {"CALL nn",     0x02},
    {"ADC A,n",     0x01},
    {"RST 0x08",    0x00},

    {"RET NC",      0x00},
    {"POP DE",      0x00},
    {"JP NC,nn",    0x02},
    {"unused op",   0x00},
    {"CALL NC,nn",  0x02}, 
    {"PUSH DE",     0x00},
    {"SUB n",       0x01},
    {"RST 0x10",    0x00},
    {"RET C",       0x00},
    {"RETI",        0x00},
    {"JP C,nn",     0x02},
    {"unused",      0x00},
    {"CALL C,nn",   0x02},
    {"unused",      0x00},
    {"SBC A,n",     0x01},
    {"RST 0x18",    0x00},

    {"LD (0xFF00+n),A", 0x01},
    {"POP HL",          0x00},
    {"LD (0xFF00+C),A", 0x00},
    {"unused opcode",   0x00},
    {"unused opcode",   0x00},
    {"PUSH HL",         0x00},
    {"AND n",           0x01},
    {"RST 0x20",        0x00},
    {"ADD SP,n",        0x01},
    {"JP (HL)",         0x00},
    {"LD (nn),A",       0x02},
    {"unused opcode",   0x00},
    {"unused opcode",   0x00},
    {"unused opcode",   0x00},
    {"XOR n",           0x01},
    {"RST 0x28",        0x00},

    {"LD A,(0xFF00+n)", 0x01},
    {"POP AF",          0x00},
    {"LD A,(0xFF00+C)", 0x00},
    {"DI",              0x00},
    {"unused opcode",   0x00},
    {"PUSH AF",         0x00},
    {"OR n",            0x01},
    {"RST 0x30",        0x00},
    {"LD HL,SP",        0x00},
    {"LD SP,HL",        0x00},
    {"LD A,(nn)",       0x01},
    {"EI",              0x00},
    {"unused opcode",   0x00},
    {"unused opcode",   0x00},
    {"CP n",            0x01},
    {"RST 0x38",        0x00}
};

static const std::string opcode_cb_names[256] = {
    "RLC B",
    "RLC C",
    "RLC D",
    "RLC E",
    "RLC H",
    "RLC L",
    "RLC (HL)",
    "RLC A",
    "RRC B",
    "RRC C",
    "RRC D",
    "RRC E",
    "RRC H",
    "RRC L",
    "RRC (HL)",
    "RRC A",

    "RL B",
    "RL C",
    "RL D",
    "RL E",
    "RL H",
    "RL L ",
    "RL (HL)",
    "RL A",
    "RR B",
    "RR C",
    "RR D",
    "RR E",
    "RR H",
    "RR L",
    "RR (HL)",
    "RR A",

    "SLA B",
    "SLA C",
    "SLA D",
    "SLA E",
    "SLA H",
    "SLA L",
    "SLA (HL)",
    "SLA A",
    "SRA B",
    "SRA C",
    "SRA D",
    "SRA E",
    "SRA H",
    "SRA L",
    "SRA (HL)",
    "SRA A",

    "SWAP B",
    "SWAP C",
    "SWAP D",
    "SWAP E",
    "SWAP H",
    "SWAP L",
    "SWAP (HL)",
    "SWAP A",
    "SRL B",
    "SRL C",
    "SRL D",
    "SRL E",
    "SRL H",
    "SRL L",
    "SRL (HL)",
    "SRL A",

    "BIT 0 B",
    "BIT 0 C",
    "BIT 0 D",
    "BIT 0 E",
    "BIT 0 H",
    "BIT 0 L",
    "BIT 0 (HL)",
    "BIT 0 A",
    "BIT 1 B",
    "BIT 1 C",
    "BIT 1 D",
    "BIT 1 E",
    "BIT 1 H",
    "BIT 1 L",
    "BIT 1 (HL)",
    "BIT 1 A",

    "BIT 2 B",
    "BIT 2 C",
    "BIT 2 D",
    "BIT 2 E",
    "BIT 2 H",
    "BIT 2 L",
    "BIT 2 (HL)",
    "BIT 2 A",
    "BIT 3 B",
    "BIT 3 C",
    "BIT 3 D",
    "BIT 3 E",
    "BIT 3 H",
    "BIT 3 L",
    "BIT 3 (HL)",
    "BIT 3 A",

    "BIT 4 B",
    "BIT 4 C",
    "BIT 4 D",
    "BIT 4 E",
    "BIT 4 H",
    "BIT 4 L",
    "BIT 4 (HL)",
    "BIT 4 A",
    "BIT 5 B",
    "BIT 5 C",
    "BIT 5 D",
    "BIT 5 E",
    "BIT 5 H",
    "BIT 5 L",
    "BIT 5 (HL)",
    "BIT 5 A",

    "BIT 6 B",
    "BIT 6 C",
    "BIT 6 D",
    "BIT 6 E",
    "BIT 6 H",
    "BIT 6 L",
    "BIT 6 (HL)",
    "BIT 6 A",
    "BIT 7 B",
    "BIT 7 C",
    "BIT 7 D",
    "BIT 7 E",
    "BIT 7 H",
    "BIT 7 L",
    "BIT 7 (HL)",
    "BIT 7 A",

    "RES 0 B",
    "RES 0 C",
    "RES 0 D",
    "RES 0 E",
    "RES 0 H",
    "RES 0 L",
    "RES 0 (HL)",
    "RES 0 A",
    "RES 1 B",
    "RES 1 C",
    "RES 1 D",
    "RES 1 E",
    "RES 1 H",
    "RES 1 L",
    "RES 1 (HL)",
    "RES 1 A",

    "RES 2 B",
    "RES 2 C",
    "RES 2 D",
    "RES 2 E",
    "RES 2 H",
    "RES 2 L",
    "RES 2 (HL)",
    "RES 2 A",
    "RES 3 B",
    "RES 3 C",
    "RES 3 D",
    "RES 3 E",
    "RES 3 H",
    "RES 3 L",
    "RES 3 (HL)",
    "RES 3 A",

    "RES 4 B",
    "RES 4 C",
    "RES 4 D",
    "RES 4 E",
    "RES 4 H",
    "RES 4 L",
    "RES 4 (HL)",
    "RES 4 A",
    "RES 5 B",
    "RES 5 C",
    "RES 5 D",
    "RES 5 E",
    "RES 5 H",
    "RES 5 L",
    "RES 5 (HL)",
    "RES 5 A",

    "RES 6 B",
    "RES 6 C",
    "RES 6 D",
    "RES 6 E",
    "RES 6 H",
    "RES 6 L",
    "RES 6 (HL)",
    "RES 6 A",
    "RES 7 B",
    "RES 7 C",
    "RES 7 D",
    "RES 7 E",
    "RES 7 H",
    "RES 7 L",
    "RES 7 (HL)",
    "RES 7 A",

    "SET 0 B",
    "SET 0 C",
    "SET 0 D",
    "SET 0 E",
    "SET 0 H",
    "SET 0 L",
    "SET 0 (HL)",
    "SET 0 A",
    "SET 1 B",
    "SET 1 C",
    "SET 1 D",
    "SET 1 E",
    "SET 1 H",
    "SET 1 L",
    "SET 1 (HL)",
    "SET 1 A",

    "SET 2 B",
    "SET 2 C",
    "SET 2 D",
    "SET 2 E",
    "SET 2 H",
    "SET 2 L",
    "SET 2 (HL)",
    "SET 2 A",
    "SET 3 B",
    "SET 3 C",
    "SET 3 D",
    "SET 3 E",
    "SET 3 H",
    "SET 3 L",
    "SET 3 (HL)",
    "SET 3 A",

    "SET 4 B",
    "SET 4 C",
    "SET 4 D",
    "SET 4 E",
    "SET 4 H",
    "SET 4 L",
    "SET 4 (HL)",
    "SET 4 A",
    "SET 5 B",
    "SET 5 C",
    "SET 5 D",
    "SET 5 E",
    "SET 5 H",
    "SET 5 L",
    "SET 5 (HL)",
    "SET 5 A",

    "SET 6 B",
    "SET 6 C",
    "SET 6 D",
    "SET 6 E",
    "SET 6 H",
    "SET 6 L",
    "SET 6 (HL)",
    "SET 6 A",
    "SET 7 B",
    "SET 7 C",
    "SET 7 D",
    "SET 7 E",
    "SET 7 H",
    "SET 7 L",
    "SET 7 (HL)",
    "SET 7 A"
};

class Disassembler {
    public:
        bool Init(std::string rom_file);
        void Disassemble();

    private:
        uint8_t memory[0x8000];
};

#endif