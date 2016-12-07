#include "Interpreter.h"
#include <cstdio>
Interpreter::Interpreter(Memory* m){
    mem = m;
}

Interpreter::~Interpreter(){}

void Interpreter::execute(){
    // fetch opcode
    // opcode is actually two bytes long << 8 to get correct length
    // must get PC and PC+1 and merge them to get the actual opcode
    unsigned short opcode = mem->memory[mem->pc] << 8 | mem->memory[mem->pc+1];

    printf("%s\n", "OP:");
    printf("%x\n", opcode);
    printf("%x\n", opcode & 0xF000);
    //mem->pc += 2;
    // switch on the value of the first 4 bits F = 1111
    switch(opcode & 0xF000){
        case 0x0000:
            switch(opcode & 0x00FF){
                // Clear the screen
                case 0x00E0:
                    printf("%s\n", "Clear the screen.");
                break;
                // Return from subroutine
                // Sets the program counter = to top of the stack
                // decrements the stack pointer
                case 0x00EE:
                    mem->sp--;
                    mem->pc = mem->stack[mem->sp];
                    mem->pc += 2; // Maybe wrong
                break;
            }
        break;
        // Jump to address NNN
        case 0x1000:
            mem->pc = opcode & 0x0FFF;
        break;
        // Call subroutine at address NNN
        case 0x2000:
            mem->stack[mem->sp] = mem->pc;
            mem->sp++;
            mem->pc = opcode & 0x0FFF; // NNN
        break;
        // Skips the next instruction if Vx == kk
        case 0x3000:
            if(mem->V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)){
                mem->pc += 4;
            } else {
                mem->pc += 2;
            }
        break;
        // Skips the next instruction if Vx != kk
        case 0x4000:
            if(mem->V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)){
                mem->pc += 4;
            } else {
                mem->pc += 2;
            }
        break;
        // Skips the next instruction if Vx == Vk
        case 0x5000:
            if(mem->V[(opcode & 0x0F00) >> 8] == mem->V[(opcode & 0x00F0) >> 4]){
                mem->pc += 4;
            } else {
                mem->pc += 2;
            }
        break;
        // Sets Vx = kk
        case 0x6000:
            mem->V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            mem->pc += 2;
        break;
        // Sets Vx += kk
        case 0x7000:
            mem->V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            mem->pc += 2;
        break;
        case 0x8000:
            switch(opcode & 0x000F){
                // Sets Vx = Vk
                case 0x0000:
                    mem->V[(opcode & 0x0F00) >> 8] = mem->V[(opcode & 0x00F0) >> 4];
                    mem->pc += 2;
                break;
                // Sets Vx = Vx | Vk
                case 0x0001:
                    mem->V[(opcode & 0x0F00) >> 8] = mem->V[(opcode & 0x0F00) >> 8] | mem->V[(opcode & 0x00F0) >> 4];
                    mem->pc += 2;
                break;
                // Sets Vx = Vx & Vk
                case 0x0002:
                    mem->V[(opcode & 0x0F00) >> 8] = mem->V[(opcode & 0x0F00) >> 8] & mem->V[(opcode & 0x00F0) >> 4];
                    mem->pc += 2;
                break;
                // Sets Vx = Vx XOR Vk
                case 0x0003:
                    mem->V[(opcode & 0x0F00) >> 8] = mem->V[(opcode & 0x0F00) >> 8] ^ mem->V[(opcode & 0x00F0) >> 4];
                    mem->pc += 2;
                break;
                // Vx = Vx + Vk
                // May also set carry register V[F] = 1 if value is greater than 255
                case 0x0004:
                    mem->V[(opcode & 0x0F00) >> 8] = mem->V[(opcode & 0x0F00) >> 8] + mem->V[(opcode & 0x00F0) >> 4];
                    if(mem->V[(opcode & 0x0F00) >> 8] > 255){
                        mem->V[0xF] = 1;
                    } else {
                        mem->V[0xF] = 0;
                    }
                    mem->pc += 2;
                break;
                // Vx = Vx - Vy
                // VF set to 0 when there is a borrow
                case 0x0005:
                    if(mem->V[(opcode & 0x0F00) >> 8] > mem->V[(opcode & 0x00F0) >> 4]){
                        mem->V[0xF] = 0;
                    } else {
                        mem->V[0xF] = 1;
                    }
                    mem->V[(opcode & 0x0F00) >> 8] = mem->V[(opcode & 0x0F00) >> 8] - mem->V[(opcode & 0x00F0) >> 4];
                    mem->pc += 2;
                break;
                // VF set to the value of the least significant bit
                // Vx shifted to right by one (divided by 2)
                case 0x0006:
                    mem->V[0xF] = mem->V[(opcode & 0x0F00) >> 8] & 7;
                    mem->V[(opcode & 0x0F00) >> 8] = mem->V[(opcode & 0x0F00) >> 8] >> 1;
                    mem->pc += 2;
                break;
                // Vx = Vy - Vx
                // VF set to 0 when there is a borrow and 1 otherwise
                case 0x0007:
                    if(mem->V[(opcode & 0x0F00) >> 8] < mem->V[(opcode & 0x00F0) >> 4]){
                        mem->V[0xF] = 0;
                    } else {
                        mem->V[0xF] = 1;
                    }
                    mem->V[(opcode & 0x0F00) >> 8] = mem->V[(opcode & 0x00F0) >> 4] - mem->V[(opcode & 0x0F00) >> 8];
                    mem->pc += 2;
                break;
                // VF set to the value of the most significant bit
                case 0x000E:
                    mem->V[0xF] = mem->V[(opcode & 0x0F00) >> 8] >> 7;
                    mem->V[(opcode & 0x0F00) >> 8] = mem->V[(opcode & 0x0F00) >> 8] << 1;
                    mem->pc += 2;
                break;
            } // switch
        break;
        // Skips the next instruction if Vx != Vy
        case 0x9000:
            if(mem->V[(opcode & 0x0F00) >> 8] != mem->V[(opcode & 0x00F0) >> 4]){
                mem->pc += 4;
            } else {
                mem->pc += 2;
            }
        break;
        // Set I to the adress NNN
        case 0xA000:
            mem->I = opcode & 0x0FFF;// NNN
            mem->pc += 2;
        break;
        // Jumps to address NNN + V0
        case 0xB000:
            mem->pc = (opcode & 0x0FFF) + mem->V[0];
        break;
        default:
            printf("%s\n", "Unrecognized Instruction:");
            printf("%x\n", opcode);
        break;
    }
}
