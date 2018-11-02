#include "debugger.hpp"
#include "gameboy.hpp"

Debugger::Debugger(Gameboy &gameboy_state) : gameboy(gameboy_state) {
    state.breakpoint_set = false;
    state.step_set = false;
}

void Debugger::Debug() {
    if (state.debugging) {
        GetInput();
    }

    /**< Do instructions step by step */
    if (state.step_set) {
        if (!state.stepper_count) {
            state.step_set = false;
            gameboy.cpu.Diagnostics();
            GetInput();
        }
        else {
            state.stepper_count--;
        }
    }

    /**< Check for breakpoints */
    if (state.breakpoint_set && gameboy.cpu.Breakpoint(state.breakpoint)) {
        state.breakpoint_set = false;
        state.debugging = true;
        gameboy.cpu.Diagnostics();
        GetInput();
    }
}

void Debugger::GetInput() {
    std::string tmp;
    std::cout << "> ";
    char c;
    while (std::cin.get(c) && c != '\n') {
        tmp += c;
    }

    DebuggerCommand input = ParseInput(tmp);

    switch (input.cmd) {
        case Command::Run: state.debugging = false; break;
        case Command::Step: StepCommand(input); break;
        case Command::Memory: MemoryCommand(input); break; 
        case Command::Break: BreakpointCommand(input); break;
        case Command::Exit: /* TODO FINISH */; break;
        case Command::Unknown: std::cout << "Unknown command" << std::endl; break;
    }
}

DebuggerCommand Debugger::ParseInput(std::string input) {
    std::istringstream iss(input);
    std::vector<std::string> input_elements((std::istream_iterator<std::string>(iss)),
                                                std::istream_iterator<std::string>());
    
    if (input_elements.size() < 1) {
        return last_command;
    }

    Command cmd = ParseCommand(input_elements[0]);

    std::vector<std::string> args(input_elements.begin() + 1, input_elements.end());
    last_command = DebuggerCommand { cmd, args};
    return {cmd, args};
}

Command Debugger::ParseCommand(std::string cmd) {
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (cmd == "run" || cmd == "r") return Command::Run;
    else if (cmd == "step" || cmd == "s") return Command::Step;
    else if (cmd == "memory" || cmd == "m") return Command::Memory;
    else if (cmd == "break" || cmd == "b") return Command::Break;
    else if (cmd == "quit" || cmd == "q" || cmd == "exit" || cmd == "e") return Command::Exit;
    return Command::Unknown;
}

void Debugger::StepCommand(DebuggerCommand input) {
    if (input.args.size() == 0) {
        state.step_set = true;
        state.stepper_count = 1;
    }
    else if (input.args.size() == 1) {
        state.step_set = true;
        state.stepper_count = std::stoul(input.args[0], nullptr, 10);
    }
}

void Debugger::MemoryCommand(DebuggerCommand input) {
    size_t args_size;
    if ((args_size = input.args.size()) < 1) {
        // TODO Throw exception?
        return;
    }

    int tmp = std::stoul(input.args[0], nullptr, 16);
    if (tmp > 0xFFFF || tmp < 0) {
        return;
    }
    
    uint16_t address = tmp;
    if (args_size == 2) {
        size_t how_many = std::stoul(input.args[1], nullptr, 16);
        // TODO Check bounds
        for (size_t i = 0; i < how_many; i++) {
            printf("0x%02x ", gameboy.memory.ReadByteMemory(address + i));
        }
    }
    else {
        printf("0x%02x ", gameboy.memory.ReadByteMemory(address));
    }
    puts("");
}


void Debugger::BreakpointCommand(DebuggerCommand input) {
    if (input.args.size() < 1) {
        // TODO Throw exception? 
        return;
    }

    int tmp = std::stoul(input.args[0], nullptr, 16);
    if (tmp > 0xFFFF || tmp < 0) {
        return;
    }

    state.breakpoint = tmp;
    state.breakpoint_set = true;
}