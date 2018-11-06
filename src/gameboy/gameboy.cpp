#include "gameboy.hpp"

Gameboy::Gameboy(bool debug, std::string rom_file) : cpu(&memory), gpu(&memory, &cpu), debugger(*this) {
    bool cpu_init = cpu.Init(rom_file);
    bool gpu_init = gpu.Init();
    if (!cpu_init) {
        throw "Could not initialize CPU.";
    }
    if (!gpu_init) {
        throw "Could not initialize GPU.";
    }
    debugging = debug;
}

void Gameboy::Loop() {
    bool running = true;

    SDL_Event event;
    while (running) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                int key = -1;
                switch (event.key.keysym.sym) {
                    case SDLK_a: key = 4;       break;
                    case SDLK_s: key = 5;       break;
                    case SDLK_RETURN: key = 7;  break;
                    case SDLK_SPACE: key = 6;   break;
                    case SDLK_RIGHT: key = 0;   break;
                    case SDLK_LEFT: key = 1;    break;
                    case SDLK_UP: key = 2;      break;
                    case SDLK_DOWN: key = 3;    break;
		        }
                if (key != -1) {
                    SetKeyPressed(key);
                }
            } else if (event.type == SDL_KEYUP) {
                int key = -1;
                switch (event.key.keysym.sym) {
                    case SDLK_a: key = 4;       break;
                    case SDLK_s: key = 5;       break;
                    case SDLK_RETURN: key = 7;  break;
                    case SDLK_SPACE: key = 6;   break;
                    case SDLK_RIGHT: key = 0;   break;
                    case SDLK_LEFT: key = 1;    break;
                    case SDLK_UP: key = 2;      break;
                    case SDLK_DOWN: key = 3;    break;
		        }
                if (key != -1) {
                    SetKeyReleased(key);
                }
            }
        }
        Emulate();
    }

}

void Gameboy::Emulate() {

    if (debugging) {
        debugger.Debug();
    }
    
    int current_cycle = cpu.GetLastOpcodeTime();
    cpu.FetchAndDispatch();

    if (cpu.pending_interupt_enabled == true && 
        memory.ReadByteMemory(cpu.GetProgramCounter() - 1) != 0xfb) {
        cpu.interupts = true;
        cpu.pending_interupt_enabled = false;
    }

    if (cpu.pending_interupt_disabled == true && 
        memory.ReadByteMemory(cpu.GetProgramCounter() - 1) != 0xfb) {
        cpu.interupts = false;
        cpu.pending_interupt_disabled = false;
    }

    int cycles = cpu.GetLastOpcodeTime() - current_cycle;
    cpu.UpdateTimer(cycles);
    gpu.UpdateGraphics(cycles);
    cpu.DoInterupts();
}

void Gameboy::SetKeyPressed(int key) {
    bool prev_unset = false;
    bool button = true;
    bool req_interrupt = false; 

    if (TestBit(memory.joypad_state, key)) {
        prev_unset = true;
    }

    ClearBit(memory.joypad_state, key);

    if (key < 3) {
        button = false;
    }

    uint8_t key_req = memory.PrivilagedReadByteMemory(0xFF00);
    if (button && !TestBit(key_req, 5)) {
        req_interrupt = true;
    }
    if (req_interrupt && !prev_unset) {
        cpu.RequestInterupt(4);
    }
}

void Gameboy::SetKeyReleased(int key) {
   SetBit(memory.joypad_state, key);
} 