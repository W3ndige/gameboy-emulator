#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <vector>
#include <sstream>
#include <iterator>
#include "../gpu/gpu.hpp"
#include "../cpu/cpu.hpp"

struct DebuggerInfo {
    bool debugger;
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
        GPU gpu;
        CPU cpu;
        DebuggerInfo debugger_info;

        std::vector<std::string> DebuggerParseInput();
};

#endif