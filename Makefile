OBJS = src/main.cpp src/helper/helper.cpp src/memory/memory.cpp src/cpu/cpu.cpp src/gpu/gpu.cpp src/gui/gui.cpp src/gameboy/gameboy.cpp
CC = g++
COMPILER_FLAGS = -std=c++11 -Wall -Wextra -O3
LINKER_FLAGS = -lSDL2
OBJ_NAME = bin/emulator

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:
	rm $(OBJ_NAME)
	rm memory_dump.bin
