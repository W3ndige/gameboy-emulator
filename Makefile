SOURCEDIR = src
OBJS = $(shell find $(SOURCEDIR) -name '*.cpp')
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3
LIBS = -lSDL2
OBJ_NAME = bin/emulator

all: $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LIBS) -o $(OBJ_NAME)

clean:
	rm $(OBJ_NAME)
	rm memory_dump.bin
