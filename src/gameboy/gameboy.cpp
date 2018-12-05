#include "gameboy.hpp"

Gameboy::Gameboy(bool debug, bool without_boot, bool exit_on_inifite, std::string rom_file) 
                : cpu(&memory), gpu(&memory, &cpu), debugger(*this) {
                    
    if (without_boot) {
        memory.DisableBooting();
    }
    bool cpu_init = cpu.Init(rom_file, exit_on_inifite);
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

    std::chrono::time_point<std::chrono::high_resolution_clock> current, previous;
    previous = std::chrono::high_resolution_clock::now();

    SDL_Event event;
    while (running) {
        current = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast
                       <std::chrono::duration<float, std::milli>> 
                       (current - previous);

        previous = current;
        
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
		if (elapsed.count() < DELAY_TIME) {
		    std::this_thread::sleep_for(std::chrono::duration<float, std::milli>
                                       (DELAY_TIME - elapsed.count()));
		}
    }

}

void Gameboy::Emulate() {

    int cycles = 0;
    while (cycles < MAX_CYCLES) {
        if (debugging) {
            debugger.Debug();
        }
        
        int timer = cpu.GetTimer();
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

        int current_cycles = cpu.GetTimer() - timer;
        cpu.UpdateTimer(current_cycles);
        gpu.UpdateGraphics(current_cycles);
        cpu.DoInterupts();

        cycles += current_cycles;
    }
}

void Gameboy::SetKeyPressed(int key) {
    bool prev_unset = false;
    bool req_interrupt = false; 

    if (TestBit(memory.joypad_state, key) == false) {
        prev_unset = true;
    }

    ClearBit(memory.joypad_state, key);

    bool button;
    if (key > 3) {
        button = true;
    }
    else {
        button = false;
    }

    uint8_t key_req = memory.PrivilagedReadByteMemory(0xFF00);
    if (button && !TestBit(key_req, 5)) {
        req_interrupt = true;
    }
    else if (!button && !TestBit(key_req, 4)) {
        req_interrupt = true;
    }

    if (req_interrupt && !prev_unset) {
        cpu.RequestInterupt(4);
    }
}

void Gameboy::SetKeyReleased(int key) {
   SetBit(memory.joypad_state, key);
} 