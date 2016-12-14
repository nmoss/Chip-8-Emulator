#include "Interpreter.h"
#include <cstdio>
Interpreter::Interpreter(Memory* m){
    mem = m;
    drawFlag = false;
}

Interpreter::~Interpreter(){}

void Interpreter::execute(){
    // fetch opcode
    // opcode is actually two bytes long << 8 to get correct length
    // must get PC and PC+1 and merge them to get the actual opcode
    unsigned short opcode = mem->memory[mem->pc] << 8 | mem->memory[mem->pc+1];

    /*
    printf("%s\n", "OP:");
    printf("%x\n", opcode);
    printf("%x\n", opcode & 0xF000);
    */
    //mem->pc += 2;
    // switch on the value of the first 4 bits F = 1111
    switch(opcode & 0xF000){
        case 0x0000:
            switch(opcode & 0x00FF){
                // Clear the screen
                case 0x00E0:
                    //printf("%s\n", "Clear the screen.");
                    memset(mem->gfx, 0, sizeof(mem->gfx));
                    mem->pc += 2;
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
        // Sets Vx = kk AND random bytes
        case 0xC000:
            mem->V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF) & rand();
            mem->pc += 2;
        break;
        // Draws a sprite at coordinate VX,VY of height N
        case 0xD000: {
            unsigned short x = mem->V[(opcode & 0x0F00) >> 8];
            unsigned short y = mem->V[(opcode & 0x00F0) >> 4];
            unsigned short height = opcode & 0x000F;
            mem->V[0xF] = 0; // Collision detection
            for(int yline = 0; yline < height; yline++){
                unsigned short pixel = mem->memory[mem->I + yline];
                for(int xline = 0; xline < 8; xline++){
                    //Check to see if the pixel is already 1
                    if ((pixel & (0x80 >> xline)) != 0) {
                        // Collision detection
                        if(mem->gfx[(x + xline + ((y + yline) * 64))] == 1){
                            mem->V[0xF] = 1;
                        } // if
                        mem->gfx[x + xline + ((y + yline) * 64)] ^= 1;
                    }//if
                }//for
            }//for
            drawFlag = true;
            mem->pc += 2;
        }
        break;
        // Checks to see if a key has been pressed
        case 0xE000:
            switch(opcode & 0x00FF){
                // Skips the next instruction if the key Vx has been pressed
                case 0x009E:
                    if(mem->key[mem->V[(opcode & 0x0F00) >> 8]] != 0){
                        mem->pc += 4;
                        //mem->key[mem->V[(opcode & 0x0F00) >> 8]] = 0;
                    } else {
                        mem->pc += 2;
                    }
                break;
                // Skips the next instruction if the key Vx has not been pressed
                case 0x00A1:
                    if(mem->key[mem->V[(opcode & 0x0F00) >> 8]] == 0){
                        mem->pc += 4;
                        //mem->key[mem->V[(opcode & 0x0F00) >> 8]] = 0;
                    } else {
                        mem->pc += 2;
                    }
                break;
            }
        break;
        case 0xF000:
            switch(opcode & 0x00FF){
                // Sets Vx = delay timer value
                case 0x0007:
                    mem->V[(opcode & 0x0F00) >> 8] = mem->delay_timer;
                    mem->pc += 2;
                break;
                // Waits for a key press and sets that key to true
                case 0x000A:
                    for(int i = 0; i < 0x10; i++){
                        if(mem->key[i]){
                            mem->V[(opcode & 0x0F00) >> 8] = i;
                            //mem->key[i] = 0;
                            mem->pc += 2;
                        }
                    }
                    //memset(mem->key, 0, sizeof(mem->key));
                    //mem->pc += 2;
                break;
                // Sets the delay timer = Vx
                case 0x0015:
                    mem->delay_timer = mem->V[(opcode & 0x0F00) >> 8];
                    mem->pc += 2;
                break;
                // Sets the sound timer = Vx
                case 0x0018:
                    mem->sound_timer = mem->V[(opcode & 0x0F00) >> 8];
                    mem->pc += 2;
                break;
                // Sets I = I + Vx
                case 0x001E:
                    mem->I += mem->V[(opcode & 0x0F00) >> 8];
                    mem->pc += 2;
                break;
                // Sets I = location of the hexadecimal sprite in Vx
                case 0x0029:
                    mem->I = mem->V[(opcode & 0x0F00) >> 8] * 5;
                    mem->pc += 2;
                break;
                // Stores the binary encoded representation of Vx in I, I+1, I+2
                case 0x033:
                    mem->memory[mem->I]     = mem->V[(opcode & 0x0F00) >> 8] / 100;
                    mem->memory[mem->I + 1] = (mem->V[(opcode & 0x0F00) >> 8] / 10) % 10;
                    mem->memory[mem->I + 2] = (mem->V[(opcode & 0x0F00) >> 8] % 100) % 10;
                    mem->pc += 2;
                break;
                // Stores registers V0 to Vx in memory location I
                case 0x055:
                    for(int i = 0; i < ((opcode & 0x0F00) >> 8); i++){
                        mem->memory[mem->I+i] = mem->V[i];
                    }//for
                    mem->pc +=2;
                break;
                // Loads registers V0 to Vx from memory location I
                case 0x065:
                    for(int i = 0; i < ((opcode & 0x0F00) >> 8); i++){
                        mem->V[i] = mem->memory[mem->I+i];
                    }//for
                    mem->pc +=2;
                break;
            } // switch
        break;
        default:
            //printf("%s\n", "Unrecognized Instruction:");
            //printf("%x\n", opcode);
        break;
    } // switch

    if(mem->sound_timer > 0){
        //if(mem->sound_timer == 1)
            //int x = 1;
            //printf("%s\n", "BEEP");
        mem->sound_timer--;
    }
    if(mem->delay_timer > 0){
        mem->delay_timer--;
    }
}
