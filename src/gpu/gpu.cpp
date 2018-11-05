#include "gpu.hpp"

GPU::GPU(Memory *mem, CPU *cpu) {
    this->memory = mem;
    this->cpu = cpu;
    current_line = 0;
    current_mode = 0;
    scanline_counter = 0;
    pixels = new uint32_t[(SCREEN_HEIGH / 2) * (SCREEN_WIDTH / 2)];
}

GPU::~GPU() {
    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool GPU::Init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        SDL_Quit();
        return false;
    }
    
    std::string title = "Gameboy " + memory->cartridge_header.title;
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, 100,
                                          SCREEN_HEIGH, SCREEN_WIDTH, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        printf( "SDL Window could not initialize! SDL_Error: %s\n", SDL_GetError() );
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        printf( "SDL Renderer could not initialize! SDL_Error: %s\n", SDL_GetError() );
        SDL_Quit();
        return false;
    }
    SDL_RenderSetScale(renderer, 2, 2);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 160, 144);
    return true;
}

void GPU::UpdateGraphics(int cycles) {
    SetLCDStatus();

    if (!IsLCDEnabled()) {
        return;
    }
    scanline_counter -= cycles;

    if (scanline_counter <= 0) {
        current_line = memory->ReadByteMemory(0xFF44);
        current_line++;
        memory->PrivilagedByteWrite(0xFF44, current_line);
        scanline_counter = 456;

        if (current_line == 144) {
            PrintPixels();
            cpu->RequestInterupt(0) ;
        }
        else if (current_line > 153) {
            memory->PrivilagedByteWrite(0xFF44, 0);
        }
        else if (current_line < 144) {
            DrawScanLine();
        }
    }
}

uint8_t GPU::IsLCDEnabled() {
    uint8_t byte = memory->ReadByteMemory(0xFF40);
    return TestBit(byte, 7);
} 

void GPU::SetLCDStatus() {
    uint8_t status = memory->ReadByteMemory(0xFF41);
    if (IsLCDEnabled() == false) {
        scanline_counter = 456; 
        memory->PrivilagedByteWrite(0xFF44, 0x0);
        status &= 252;
        SetBit(status, 0x0);
        memory->WriteByteMemory(0xFF41, status);
        return;
    }
    
    current_line = memory->ReadByteMemory(0xFF44);
    current_mode = status &= 0x3;
    uint8_t mode = 0;
    uint8_t request_interrupt = 0;

    if (current_line >= 144) {
        mode = 1;
        SetBit(status, 0x0);
        ClearBit(status, 0x1);
        request_interrupt = TestBit(status, 0x4);
    }
    else {
        int mode_2_bounds = 456 - 80;
        int mode_3_bounds = mode_2_bounds - 172;
        if (scanline_counter >= mode_2_bounds) {
            mode = 2;
            SetBit(status, 1);
            ClearBit(status, 0);
            request_interrupt = TestBit(status,5);
        }
        else if (scanline_counter >= mode_3_bounds) {
            mode = 3 ;
            SetBit(status, 1);
            SetBit(status, 0);
        }
        else {
            mode = 0;
            ClearBit(status, 1);
            ClearBit(status, 0);
            request_interrupt = TestBit(status, 3); 
        }
    }

    if (request_interrupt && (mode != current_mode)) {
        cpu->RequestInterupt(1);
    }

    if (current_line == memory->ReadByteMemory(0xFF45)) {
        SetBit(status, 2);
        if (TestBit(status, 6)) {
            cpu->RequestInterupt(1);
        }
    }
    else {
     ClearBit(status, 2) ;
    }

    memory->WriteByteMemory(0xFF41,status) ;

}

void GPU::DrawScanLine() {
	if (IsLCDEnabled()) {
        if (TestBit(memory->ReadByteMemory(0xFF40), 0)) {
            RenderTiles();
        }
        if (TestBit(memory->ReadByteMemory(0xFF40), 1)) {
            RenderSprites();
        }
	}
}

void GPU::RenderTiles() {
    uint8_t control = memory->ReadByteMemory(0xFF40);

    uint16_t tile_data = 0;
    uint16_t background_memory = 0;

    uint8_t scroll_y = memory->ReadByteMemory(0xFF42);
    uint8_t scroll_x = memory->ReadByteMemory(0xFF43);

    bool use_unsigned_values = true;

    if (TestBit(control, 4)) {
        tile_data = 0x8000;
    }
    else {
        use_unsigned_values = false;
        tile_data = 0x8800;
    }

    if (TestBit(control, 3)) {
        background_memory = 0x9C00;
    }
    else {
        background_memory = 0x9800;
    }

    uint8_t y_pos = scroll_y + current_line;

    uint16_t tile_row = (((uint8_t)(y_pos / 8)) * 32);
    for (int pixel = 0; pixel < 160; pixel++) {
        uint8_t x_pos = pixel + scroll_x;

        uint16_t tile_col = x_pos / 8;
        int16_t tile_num;

        uint16_t tile_address = background_memory + tile_row + tile_col;
        uint16_t tile_location = tile_data;

        if (use_unsigned_values) {
            tile_num = (uint8_t)memory->ReadByteMemory(tile_address);
            tile_location += (tile_num * 16);
        }
        else {
            tile_num = (int8_t)memory->ReadByteMemory(tile_address);
            tile_location += ((tile_num + 128) * 16);
        }

        uint8_t line = (y_pos % 8) * 2;
        uint8_t byte_1 = memory->ReadByteMemory(tile_location + line);
        uint8_t byte_2 = memory->ReadByteMemory(tile_location + line + 1);

        uint8_t req_bit = 7 - (x_pos % 8);
        uint8_t bit_1 = TestBit(byte_1, req_bit);
        uint8_t bit_2 = TestBit(byte_2, req_bit);
        uint8_t color_id = (bit_1 << 1) | bit_2;
        int color = GetColor(color_id, 0xff47);
        int red, blue, green;

        if (color == 0) {
            red = blue = green = 0xff;
        } else if (color == 1) {
             red = 0xcc, blue = 0xcc, green = 0xcc;
        } else if (color == 2) {
            red = 0x77, blue = 0x77, green = 0x77;
        } else {
            red = blue = green = 0;
        }

        pixels[pixel + (current_line * 160)] = (0xFF << 24) | (red << 16) | (green << 8) | (blue);
    }
}

void GPU::RenderSprites() {
    uint8_t control = memory->ReadByteMemory(0xFF40);
    bool use8x16sprites = false;

    if (TestBit(control, 2)) {
        use8x16sprites = true;
    }

    for (int sprite = 0; sprite < 40; sprite++) {
        uint8_t index = sprite * 4;
        uint8_t y_pos = memory->ReadByteMemory(0xFE00 + index) - 16;
        uint8_t x_pos = memory->ReadByteMemory(0xFE00 + index + 1) - 8;
        uint8_t location = memory->ReadByteMemory(0xFE00 + index + 2);
        uint8_t attributes = memory->ReadByteMemory(0xFE00 + index + 3);
        
        int x_flip = TestBit(attributes, 5);
        int y_flip = TestBit(attributes, 6);

        int y_size = 8;
        if (use8x16sprites) {
            y_size = 16;
        }

        if (current_line >= y_pos && current_line < (y_pos + y_size)) {
            int line = current_line - y_pos;

            if (y_flip) {
                line -= y_size;
                line *= -1;
            }

            line *= 2;
            uint16_t data_address = (0x8000 + (location * 16) + line);

            uint8_t data_1 = memory->ReadByteMemory(data_address);
            uint8_t data_2 = memory->ReadByteMemory(data_address + 1);

            for (int tile_pixel = 7; tile_pixel >= 0; tile_pixel--) {
                int colour_bit = tile_pixel;
                if (x_flip) {
                    colour_bit -= 7;
                    colour_bit *= -1;
                }
                
                uint8_t bit_1 = TestBit(data_1, colour_bit);
                uint8_t bit_2 = TestBit(data_2, colour_bit);
                uint8_t color_id = (bit_2 << 1) | bit_1;
                //uint16_t colour_address = TestBit(attributes, 4) ? 0xff48 : 0xff49;
                uint16_t colour_address = 0xff47;
                int color = GetColor(color_id, colour_address);
                int red, blue, green;
                
                if (color == 0) {
                    continue;
                } else if (color == 1) {
                    red = 0xcc, blue = 0xcc, green = 0xcc;
                } else if (color == 2) {
                    red = 0x77, blue = 0x77, green = 0x77;
                } else {
                    red = blue = green = 0;
                }

                if (TestBit(attributes, 7)) {
                    continue;
                }
                
                int pixel = x_pos + (0 - tile_pixel + 7);

                pixels[pixel + (current_line * 160)] = (0xFF << 24) | (red << 16) | (green << 8) | (blue);
            }
        }
    }
}

int GPU::GetColor(uint8_t color_id, uint16_t address) {
    uint8_t data = memory->ReadByteMemory(address);
    int hi = 2 * color_id + 1;
    int lo = 2 * color_id;
    int bit_1 = TestBit(data, hi);
    int bit_0 = TestBit(data, lo);
    return (bit_1 << 1) | bit_0;    
}

void GPU::PrintPixels() {
    SDL_UpdateTexture(texture, NULL, pixels, 160 * sizeof(Uint32));
    SDL_RenderClear(renderer);
    const SDL_Rect dest = {.x = 0, .y = 0, .w = 160, .h = 144};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_RenderPresent(renderer);
}