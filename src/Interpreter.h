#ifndef __Interpreter_H__
#define __Interpreter_H__

#include "Memory.h"
// Decodes and executes instructions fetched from memory
// Fetches the opcode which represents the current instruction
// There are 35 opcodes which are two bytes long stored big endian
// Contains a memory object representing chip-8 memory throughout program
// execution
// TODO add graphics object
class Interpreter {

    public:
        Memory* mem;
        Interpreter(Memory* m);
        ~Interpreter();
        //Emulates one cycle by executing one instruction
        void execute();
        // Runs the chip-8 program stored in memory
        //void run();

};
#endif
