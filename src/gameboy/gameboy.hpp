#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <vector>
#include <sstream>
#include <iterator>
#include "../gpu/gpu.hpp"
#include "../cpu/cpu.hpp"

struct DebuggerInfo {
    bool debugger_set;
    bool step_set;
    bool breakpoint_set;
    int breakpoint;
};

class Gameboy {
    public:
        Gameboy(bool debugger);
        void Debugger();
        void Emulate();

    private:
        Memory memory;
        CPU cpu;
        GPU gpu;
        DebuggerInfo debugger_info;

        std::vector<std::string> DebuggerParseInput();
};

#endif