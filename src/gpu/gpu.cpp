#include "gpu.hpp"

GPU::GPU(Memory *mem, CPU *cpu): pixels() {
    this->memory = mem;
    this->cpu = cpu;
    current_line = 0;
    current_mode = 0;
    scanline_counter = 0;
    gui.Init();
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

/*
Bit 7 - LCD Display Enable (0=Off, 1=On)
Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
Bit 5 - Window Display Enable (0=Off, 1=On)
Bit 4 - BG & Window Tile Data Select (0=8800-97FF, 1=8000-8FFF)
Bit 3 - BG Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
Bit 2 - OBJ (Sprite) Size (0=8x8, 1=8x16)
Bit 1 - OBJ (Sprite) Display Enable (0=Off, 1=On)
Bit 0 - BG Display (for CGB see below) (0=Off, 1=On)
*/

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

        pixels[pixel][current_line] = (bit_1 << 1) | bit_2;
    }
}

void GPU::RenderSprites() {
    /*
    uint8_t control = memory->ReadByteMemory(0xFF40);

    bool use8x16sprites = false;
    if (TestBit(control, 2)) {
        use8x16sprites = true;
    }

    for (int sprite = 0; sprite < 40; sprite++) {
        uint8_t index = sprite * 4;
        uint8_t y_pos = memory->ReadByteMemory(0xFFE00 + index) - 16;
        uint8_t x_pos = memory->ReadByteMemory(0xFFE00 + index + 1) - 9;
        uint8_t location = memory->ReadByteMemory(0xFFE00 + index + 2);
        uint8_t attributes = memory->ReadByteMemory(0xFFE00 + index + 3);
        
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
            uint8_t data_address = (0x8000 + (location * 16) + line);
            uint8_t data_1 = memory->ReadByteMemory(data_address);
            uint8_t data_2 = memory->ReadByteMemory(data_address + 1);

            for (int tile_pixel = 7; tile_pixel >= 0; tile_pixel--) {

            }

        }

    }

    */
}

void GPU::PrintPixels() {
    /**< Drawing lines first */
    for (int i = 0; i < 144; i++) {
        for (int j = 0; j < 160; j++) {
            switch (pixels[j][i]) {
                case 0:
                    SDL_SetRenderDrawColor(gui.renderer, 0xff, 0xff, 0xff, 0xff);
                    SDL_RenderDrawPoint(gui.renderer, j, i);
                    break;
                case 1:
                    // TODO
                    break;
                case 2:
                    SDL_SetRenderDrawColor(gui.renderer, 0x00, 0x00, 0x00, 0xff);
                    SDL_RenderDrawPoint(gui.renderer, j, i);
                    break;
                case 3:
                    // TODO
                    break;                    
            }
        }
    }
    SDL_RenderPresent(gui.renderer);
}