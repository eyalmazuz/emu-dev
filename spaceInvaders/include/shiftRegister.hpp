#ifndef _SHIFT_REGISTER_H_
#define _SHIFT_REGISTER_H_

#include <stdint.h>


class ShiftRegister {
    public:
        ShiftRegister();
        ~ShiftRegister();
        void shift(uint8_t value);
        uint16_t read();
        void offset(uint8_t offset);

    private:
        uint16_t data;
};

#endif