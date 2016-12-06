#ifndef __MEMORY_H__
#define __MEMORY_H__

// The program memory for Chip-8 virtual machine
// The Chip-8 language can access 4KB of RAM from addresses 0x000 to 0xFFF
// The first 512 bytes were reserved for the interpreter and are not used
// 0x000 to 0x200 reserved for the interpreter
// 0xF00 to 0xFFF reserved for display refresh (256 bytes)
// 0xEA0 to 0xEFF reserved for the stack (96 bytes)
// 0x200 to 0xFFF program space (some programs start at 0x600)
// 0x000 to 0x1FF font storage (in the area reserved for interpreter)
// Programs should address between 0x200 up to 0xEA0
// Chip-8 has 16 8-bit registers Vx (where x is a hexadecimal digit 0-F)
// VF should not be used by any program as it contains flag data
// I is a 16-bit register for containing memory addresses
// PC is 16-bit
// SP is 8-bit
// The stack is an array of 16 16-bit values
class Memory {
    public:
        /* Fontset */
        const unsigned char fontset[80] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };
        unsigned short I;
        unsigned short pc;
        unsigned short sp;
        unsigned short stack[16];
        unsigned char memory[4096];
        Memory();
        ~Memory();


};

#endif
