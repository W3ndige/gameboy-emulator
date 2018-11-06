SOURCEDIR = src
SRCS = $(shell find $(SOURCEDIR) -name '*.cpp')
BUILDDIR = objs
OBJS = $(SRCS:.cpp=.o)

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3
LIBS = -lSDL2
OBJ_NAME = bin/emulator

all: $(OBJDIR)$(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS) -o $(OBJ_NAME)

.PHONY: clean

clean:
	rm -f $(OBJS) $(OBJ_NAME) memory_dump.bin