Gameboy Emulator
==============

Simple Gameboy Emulator written in C++ and SDL2, with debugger and disassembler.

To-Do List
----------

* Allow support for memory banking.
* GPU issues with Flappyboy.
* Disassembler code review. 
* Code review.

Usage
-----

#### Boot options

* `./bin/emulator ROM_GAME` -- run the emulator with specified ROM.
* `./bin/emulator -q ROM_GAME` -- run the emulator without boot sequence.
* `./bin/emulator -j ROM_GAME` -- run the emulator, exiting while infinite loop is encountered.
* `./bin/emulator -e ROM_GAME` -- disassemble the ROM.
* `./bin/emulator -d ROM_GAME` -- run the debugger.

#### Debugger commands

* `run` or `r` -- run the ROM.
* `step` or `s` -- step one instruction.
* `memory addr num` or `m` -- view num of bytes from memory at address addr.
* `stack num` or `s` -- view num of bytes from stack.
* `info` or `i` -- get the information about the cartridge
* `break addr` or `b` -- set the breakpoint at address addr.
