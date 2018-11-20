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
        if (state.stepper_count) {
            state.stepper_count--;
        }
        if (!state.stepper_count) {
            state.debugging = true;
            state.step_set = false;
            gameboy.cpu.Diagnostics();
            GetInput();
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

void Debugger::PrintHelp() {

    printf("");


}

void Debugger::GetInput() {
    char *line = readline("\033[1m>\033[0m ");
    if(*line) {
        add_history(line);
    }

    DebuggerCommand input = ParseInput(std::string(line));
    free(line);

    switch (input.cmd) {
        case Command::Run: state.debugging = false; break;
        case Command::Step: StepCommand(input); break;
        case Command::Memory: MemoryCommand(input); break;
        case Command::Stack: StackCommand(input); break;
        case Command::Info: InfoCommand(); break;
        case Command::Break: BreakpointCommand(input); break;
        case Command::Exit: gameboy.debugging = false; break;
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
    else if (cmd == "stack" || cmd == "sp") return Command::Stack;
    else if (cmd == "info" || cmd == "i") return Command::Info;
    else if (cmd == "break" || cmd == "b") return Command::Break;
    else if (cmd == "quit" || cmd == "q" || cmd == "exit" || cmd == "e") return Command::Exit;
    return Command::Unknown;
}

void Debugger::StepCommand(DebuggerCommand input) {
    state.debugging = false;
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
        size_t how_many = std::stoul(input.args[1], nullptr, 10);
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

void Debugger::StackCommand(DebuggerCommand input) {
    size_t args_size;
    if ((args_size = input.args.size()) < 1) {
        // TODO Throw exception?
        return;
    }

    if (args_size == 1) {
        size_t how_many = std::stoul(input.args[0], nullptr, 10);
        for (size_t i = 0; i < how_many; i++) {
            printf("0x%02x ", gameboy.memory.ReadByteMemory(gameboy.cpu.GetStackPointer() + i));
        }
    }
    puts("");
}

void Debugger::InfoCommand() {
    printf("TITLE\t%s\n", gameboy.memory.cartridge_header.title.c_str());
    printf("CARTRIDGE TYPE\t");
    switch (gameboy.memory.cartridge_header.cartridge_type) {
        case 0x00: puts("ROM ONLY"); break;
        case 0x01: puts("MBC1"); break;
        case 0x02: puts("MBC1+RAM"); break;
        case 0x03: puts("MBC1+RAM+BATTERY"); break;
        case 0x05: puts("MBC2"); break;
        case 0x06: puts("MBC2+BATTERY"); break;
        case 0x08: puts("ROM+RAM"); break;
        case 0x09: puts("ROM+RAM+BATTERY"); break;
        case 0x0B: puts("MMM01"); break;
        case 0x0C: puts("MMM01+RAM"); break;
        case 0x0D: puts("MMM01+RAM+BATTERY"); break;
        case 0x0F: puts("MBC3+TIMER+BATTERY"); break;
        case 0x10: puts("MBC3+TIMER+RAM+BATTERY"); break;
        case 0x11: puts("MBC3"); break;
        case 0x12: puts("MBC3+RAM)"); break;
        case 0x13: puts("MBC3+RAM+BATTERY"); break;
        case 0x19: puts("MBC5"); break;
        case 0x1A: puts("MBC5+RAM"); break;
        case 0x1B: puts("MBC5+RAM+BATTERY"); break;
        case 0x1C: puts("MBC5+RUMBLE"); break;
        case 0x1D: puts("MBC5+RUMBLE+RAM"); break;
        case 0x1E: puts("MBC5+RUMBLE+RAM+BATTERY"); break;
        case 0x20: puts("MBC6"); break;
        case 0x22: puts("MBC7+SENSOR+RUMBLE+RAM+BATTERY"); break;
        case 0xFC: puts("POCKET CAMERA"); break;
        case 0xFD: puts("BANDAI TAMA5"); break;
        case 0xFE: puts("HuC3"); break;
        case 0xFF: puts("HuC1+RAM+BATTERY"); break;
    }

    printf("ROM SIZE\t");
    switch (gameboy.memory.cartridge_header.rom_size) {
        case 0x00: puts("32KByte (no ROM banking)"); break;
        case 0x01: puts("64KByte (4 banks)"); break;
        case 0x02: puts("128KByte (8 banks)"); break;
        case 0x03: puts("256KByte (16 banks)"); break;
        case 0x04: puts("512KByte (32 banks)"); break;
        case 0x05: puts("1MByte (64 banks)"); break;
        case 0x06: puts("2MByte (128 banks)"); break;
        case 0x07: puts("4MByte (256 banks)"); break;
        case 0x08: puts("8MByte (512 banks)"); break;
        case 0x52: puts("1.1MByte (72 banks)"); break;
        case 0x53: puts("1.2MByte (80 banks)"); break;
        case 0x54: puts("1.5MByte (96 banks)"); break;
    }

    printf("RAM SIZE\t");
    switch (gameboy.memory.cartridge_header.ram_size) {
        case 0x00: puts("None"); break;
        case 0x01: puts("2 KBytes"); break;
        case 0x02: puts("8 Kbytes"); break;
        case 0x03: puts("32 KBytes"); break;
        case 0x04: puts("128 KBytes"); break;
        case 0x05: puts("64 KBytes"); break;
    }
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