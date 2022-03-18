#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>
#include <stddef.h>

class Cpu {

    public:
        Cpu(size_t registerCount);
        ~Cpu();
        size_t getNumRegisters();
        uint16_t getI();
        void setI(uint16_t value);

        uint8_t getRegister(size_t index);
        void writeRegister(size_t index, uint8_t value); 
    
        void clear();

    private:
        size_t registerCount;
        uint8_t *V;

        uint16_t I;

        uint8_t delay;
        uint8_t sound;
};

#endif