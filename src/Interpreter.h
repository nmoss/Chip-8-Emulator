#ifndef __Interpreter_H__
#define __Interpreter_H__

#include <cstdlib>
#include "Memory.h"
// Decodes and executes instructions fetched from memory
// Fetches the opcode which represents the current instruction
// There are 35 opcodes which are two bytes long stored big endian
// A byte is 8 bits so instructions are of the form 1111 1111 1111 1111
// Can do a switch with opcode & 0xF000
// F = 1111 so we can get the value of any individual 'nibble' or 4 bits
// Contains a memory object representing chip-8 memory throughout program
// execution
// TODO add graphics object
class Interpreter {

    public:
        Memory* mem;
        bool drawFlag;
        Interpreter(Memory* m);
        ~Interpreter();
        //Emulates one cycle by executing one instruction
        void execute();
        // Runs the chip-8 program stored in memory
        //void run();

};
#endif
