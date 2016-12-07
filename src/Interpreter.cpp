#include "Interpreter.h"

Interpreter::Interpreter(Memory* m){
    mem = m;
}

Interpreter::~Interpreter(){}

void Interpreter::execute(){
    // fetch opcode
    // opcode is actually two bytes long << 8 to get correct length
    // must get PC and PC+1 and merge them to get the actual opcode
    unsigned short opcode = mem->memory[mem->pc] << 8 | mem->memory[mem->pc+1];

    switch(opcode & 0xF000){
        
    }

}
