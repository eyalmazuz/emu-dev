#ifndef _CHIP8_H_
#define _CHIP8_H_

#include <stdint.h>
class CHIP8 {
    public:
        bool load_rom(const char* path);
        void cycle();
        
        CHIP8();
        ~CHIP8();

        uint8_t key[16];
        uint8_t display[64*32];
        bool draw;

    private:
        uint16_t stack[16];
        uint16_t sp;

        uint8_t memory[4096];
        uint8_t V[16];


        uint16_t pc;
        uint16_t I;
        uint16_t opcode;

        uint8_t delay;
        uint8_t sound;

        void init();
};
#endif
