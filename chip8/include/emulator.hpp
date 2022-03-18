#ifndef _EMULATOR_H_
#define _EMULATOR_H_

#include <stddef.h>
#include "display.hpp"
#include "stack.hpp"
#include "memory.hpp"
#include "keyboard.hpp"
#include "cpu.hpp"

class Emulator {

    public:
        Emulator(size_t registerCount, size_t memorySize, size_t stackSize, size_t displayHeight, size_t displayWidth);
        ~Emulator();

        bool loadRom(const char* path);
        void cycle();
        void init();

        void keyPressed(SDL_Keycode key);
        void keyReleased(SDL_Keycode key);

        bool canDraw();
        void setDraw(bool drawFlag);
        uint8_t isDrawn(size_t x, size_t y);

    private:

        uint16_t pc;
        uint16_t opcode;

        uint8_t delay;
        uint8_t sound;

        bool draw;

        Cpu *cpu;
        Display *display;
        Stack *stack;
        Memory *memory;
        Keyboard *keyboard;

        void loadFontSet();

};

#endif
