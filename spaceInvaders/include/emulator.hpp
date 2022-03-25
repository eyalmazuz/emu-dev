#ifndef _EMULATOR_H_
#define _EMULATOR_H_

#include "memory.hpp"
#include "shiftRegister.hpp"
#include "video.hpp"

class Emulator {

    public:
        Emulator(size_t memory_size, size_t screen_h, size_t screen_w);
        ~Emulator();
        bool load_rom(const char* path);

    private:
        Memory *memory;
        Video *video;
        ShiftRegister *shiftReg;

        size_t mem_size;
        size_t screen_h;
        size_t screen_w;

        void init();
};

#endif