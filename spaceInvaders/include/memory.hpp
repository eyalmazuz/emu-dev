#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stddef.h>
#include <stdint.h>

class Memory {

    public:
        Memory(size_t size);
        ~Memory();

        uint8_t read();
        void write(size_t addr, uint8_t value);

    private:
        uint8_t *memory;
        size_t size;
};

#endif