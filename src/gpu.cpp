#include "gpu.hpp"

GPU::GPU(Memory *mem) {
    memory = mem;
    current_line = 0;
    current_mode = 0;
    scanline_counter = 0;
}

void GPU::UpdateGraphics(int cycles) {
    SetLCDStatus();

    if (IsLCDEnabled()) {
        scanline_counter -= cycles;
    }
    else {
        return;
    }

    if (scanline_counter <= 0) {
        uint8_t memory_val = memory->ReadByteMemory(0xFF44);
        memory_val++;
        memory->WriteByteMemory(0xFF44, memory_val);
        current_line = memory_val;
        scanline_counter = 456;

        if (current_line == 144) {
            printf("New Frame\n");
            PrintPixels();
            puts("\n");
            // TODO() Request Interrupt
        }
        else if (current_line > 153) {
            memory->WriteByteMemory(0xFF44, 0);
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
        memory->WriteByteMemory(0xFF44, 0x0);
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

     // TODO Finish function.
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

    uint8_t scroll_x = memory->ReadByteMemory(0xFF42);
    uint8_t scroll_y = memory->ReadByteMemory(0xFF43);
    uint8_t window_x = memory->ReadByteMemory(0xFF4A);
    uint8_t window_y = memory->ReadByteMemory(0xFF4B) - 7;

    bool window_enabled = false;
    bool use_unsigned_values = true;

    if (TestBit(control, 5)) {
        if (memory->ReadByteMemory(0xFF44) >= window_y) {
            window_enabled = true;
        }
    }

    if (TestBit(control, 4)) {
        tile_data = 0x8000;
    }
    else {
        use_unsigned_values = false;
        tile_data = 0x8800;
    }

    if (window_enabled == false) {
        if (TestBit(control, 3)) {
            background_memory = 0x9C00;
        }
        else {
            background_memory = 0x9800;
        }
    }
    else {
        if (TestBit(control, 6)) {
            background_memory = 0x9C00;
        }
        else {
            background_memory = 0x9800;
        }
    }

    uint8_t y_pos = 0;

    if (!window_enabled) {
        y_pos = scroll_y + current_line;
    }
    else {
        y_pos = current_line - window_y;
    }

    uint16_t tile_row = (uint16_t)((y_pos / 8) * 32);

    for (int pixel = 0; pixel < 160; pixel++) {
        uint8_t x_pos = pixel + scroll_x;
        if (window_enabled) {
            if (pixel >= window_x) {
                x_pos = pixel - window_x;
            }
        }

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

        uint8_t line = y_pos % 8;
        line *= 2;
        uint8_t byte_1 = memory->ReadByteMemory(tile_location + line);
        uint8_t byte_2 = memory->ReadByteMemory(tile_location + line + 1);

        uint8_t req_bit = 7 - (x_pos % 8);
        uint8_t bit_1 = (byte_1 >> req_bit) & 1;
        uint8_t bit_2 = (byte_2 >> req_bit) & 1;

        pixels[pixel][current_line] = (bit_1 << 1) | bit_2;
    }
}

void GPU::RenderSprites() {


}

void GPU::PrintPixels() {
    /**< Drawing lines first */
    for (int i = 0; i < 144; i++) {
        for (int j = 0; j < 160; j++) {
            if (pixels[j][i] == 0) {
                printf(" ");
            }
            else if (pixels[j][i]== 1) {
                printf("*");
            }
            else if (pixels[j][i] == 2) {
                printf(".");
            }
            else {
                printf("#");
            }
        }
        printf("\n");
    }
}