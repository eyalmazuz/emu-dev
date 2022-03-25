#include "../include/shiftRegister.hpp"

#define MERGE(a ,b) (((uint16_t)a) << 8 | ((uint16_t) b))
#define SHIFT_LEFT(a, size) ((a >> size))
#define SHIFT_RIGHT(a, size) ((a << size))

ShiftRegister::ShiftRegister() { }

ShiftRegister::~ShiftRegister() { }

void ShiftRegister::shift(uint8_t value) {
    data = MERGE(value, data);
}

uint16_t ShiftRegister::read() {
    return data;
}

void ShiftRegister::offset(uint8_t offset) {
    data = SHIFT_RIGHT(data, offset);
    data = SHIFT_LEFT(data, 8);
}