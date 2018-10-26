#include "gameboy.hpp"

int debug = 0;

Gameboy::Gameboy(bool debugger) : gpu(&memory), cpu(&memory) {
    debugger_info.breakpoint_set = false;
    debugger_info.debugger = debugger;
    bool init = cpu.Init();
    if (!init) {
        throw "Could not initialze CPU.";
    }
}

void Gameboy::Debugger() {
    std::vector<std::string> parsed = DebuggerParseInput();
    if (parsed.size() == 1) {
        if (parsed[0] == "con") {
            debugger_info.debugger = false;
        }
    }

    else if (parsed.size() == 2) {
        if (parsed[0] == "jmp") {
            cpu.ArtificialJump(std::stoi(parsed[1]));
            cpu.Diagnostics();
        }
        else if (parsed[0] == "brk") {
            debugger_info.breakpoint_set = true;
            debugger_info.breakpoint = std::stoi(parsed[1]);
        }
    }

    else {
        std::cout << "Invalid debugger command" << std::endl;
    }
}

std::vector<std::string> Gameboy::DebuggerParseInput() {
    std::string input;
    char c;
    std::cout << "> ";
    while (std::cin.get(c) && c != '\n') {
        input += c;
    }

    std::istringstream iss(input);
    std::vector<std::string> parsed(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>());
    return parsed;
}

void Gameboy::Emulate() {

    if (debugger_info.debugger == true) {
        Debugger();
    }

    if (debugger_info.breakpoint_set) {
        if (cpu.Breakpoint(debugger_info.breakpoint)) {
            debugger_info.debugger = true;
            debugger_info.breakpoint_set = false;
            cpu.Diagnostics();
            Debugger();
        }
    }

    unsigned int current_cycle = cpu.GetLastOpcodeTime();
    cpu.FetchAndDispatch();
    unsigned int cycles = cpu.GetLastOpcodeTime() - current_cycle;
    gpu.UpdateGraphics(cycles);
}