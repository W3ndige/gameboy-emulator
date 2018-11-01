#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP

#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>

class Gameboy;
class CPU;

struct DebuggerState {
    bool debugging;
    bool step_set;
    bool breakpoint_set;
    uint16_t breakpoint;
};

enum class Command {
    Run,
    Step,
    Memory,
    Break,
    Exit,
    Unknown
};

typedef std::vector<std::string> Arguments;

struct DebuggerCommand {
    Command cmd;
    Arguments args;
};

class Debugger {
    public:
        Debugger(Gameboy &gameboy_state);
        void Debug();

    private:
        Gameboy &gameboy;
        DebuggerState state;
        DebuggerCommand last_command;
        void GetInput();
        DebuggerCommand ParseInput(std::string input);
        Command ParseCommand(std::string cmd);
        void MemoryCommand(DebuggerCommand input);
        void BreakpointCommand(DebuggerCommand input);

};


#endif