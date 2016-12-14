#include "Memory.h"

Memory::Memory(char* program){
    // Load program into memory
    memset(memory, 0, sizeof(memory));
    memset(gfx, 0, sizeof(gfx));
    memset(key, 0, sizeof(key));

    FILE* fp;
    fp = fopen(program, "rb");
    fread(memory+0x200, 1, 4096-0x200, fp);
    fclose(fp);
    // Load fontset into memory
    for(int i = 0; i < 80; i++){
        memory[i] = fontset[i];
    }

    // Clear stack
    memset(stack, 0, sizeof(stack));
    // Clear registers
    memset(V, 0, sizeof(V));
    pc = 0x200;
    sp = 0;
    I = 0;
    delay_timer = 0;
    sound_timer = 0;
}

Memory::~Memory(){}
