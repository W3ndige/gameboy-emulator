OBJS = src/main.cpp src/memory.cpp src/cpu.cpp
CC = g++
COMPILER_FLAGS = -Wall -Wextra -O3
#LINKER_FLAGS = -lSDL2
OBJ_NAME = bin/emulator

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_NAME)

clean:
	rm $(OBJ_NAME)
