#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>
#include <stddef.h>

class Memory{

    public:
        Memory(size_t size);
        ~Memory();

        uint8_t read(size_t addr);
        void write(size_t addr, uint8_t value);
    
        void clear();
    
    private:
        size_t size;
        uint8_t *ram;
};

#endif