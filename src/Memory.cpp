#include "Memory.h"

Memory::Memory(){
    // Load fontset into memory
    for(int i = 0; i < 80; i++){
        memory[i] = fontset[i];
    }
    pc = 0x200;
    sp = 0;
    I = 0;
}

Memory::~Memory(){}
