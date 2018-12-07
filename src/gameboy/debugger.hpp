/**
 * @file debugger.hpp
 * @author W3ndige (w3ndige@protonmail.com)
 * @brief File containing logic behind the debugger.
 * @version 0.1
 * @date 2018-12-07
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP

#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <readline/readline.h>
#include <readline/history.h>

class Gameboy;
class CPU;

/**
 * @brief Structure containing information
 * about the state of debugger.
 * 
 */

struct DebuggerState {
    bool debugging;             /**< Is the debugger running */
    bool step_set;              /**< Is stepping set */
    unsigned int stepper_count; /**< How much steps to run */
    bool breakpoint_set;        /**< Is breakpoint set */
    uint16_t breakpoint;        /**< Address where to set breakpoint */
};

/**
 * @brief Commands that are available in debugger.
 * 
 */

enum class Command {
    Run,    /**< Run the program */
    Step,   /**< Step instructions */
    Memory, /**< Show memory */
    Stack,  /**< Show stack */
    Info,   /**< Show info about ROM */
    Break,  /**< Set breakpoint */
    Exit,   /**< Exit emulator */
    Unknown /**< Unknown command */
};

typedef std::vector<std::string> Arguments; /**< Vector of arguments */

/**
 * @brief Structure containing parsed debugger command.
 * 
 */

struct DebuggerCommand {
    Command cmd;    /**< Enum of command */
    Arguments args; /**< Vector of arguemts */
};

/**
 * @brief Class containing logic behind the debugger.
 * 
 */

class Debugger {
    public:
        /**
         * @brief Construct a new Debugger object.
         * 
         * @param gameboy_state Object of Gameboy class. 
         */

        Debugger(Gameboy &gameboy_state);

        /**
         * @brief Do the debugging. 
         * 
         */

        void Debug();

    private:
        Gameboy &gameboy;   /**< Pointer to gameboy object */

        DebuggerState state;            /**< State of the debugger */
        DebuggerCommand last_command;   /**< Last command entered */

        /**
         * @brief Print help.
         * 
         */

        void PrintHelp();

        /**
         * @brief Get the input from the user.
         * 
         */

        void GetInput();

        /**
         * @brief Parse the input from the user. 
         * 
         * @param input Input string.
         * @return DebuggerCommand Structure of parsed command. 
         */

        DebuggerCommand ParseInput(std::string input);

        /**
         * @brief Parse only command.
         * 
         * @param cmd String containing first part of input.
         * @return Command Parsed command. 
         */

        Command ParseCommand(std::string cmd);

        /**
         * @brief Step through the program.
         * 
         * @param input Options of the command. 
         */

        void StepCommand(DebuggerCommand input);

        /**
         * @brief Get the value from memory.
         * 
         * @param input Options of the command. 
         */

        void MemoryCommand(DebuggerCommand input);

        /**
         * @brief Get the value from stack. 
         * 
         * @param input Options of the command. 
         */

        void StackCommand(DebuggerCommand input);

        /**
         * @brief Get the info about the program.
         * 
         */

        void InfoCommand();

        /**
         * @brief Set the breakpoint.
         * 
         * @param input Options of the command.
         */

        void BreakpointCommand(DebuggerCommand input);

};


#endif