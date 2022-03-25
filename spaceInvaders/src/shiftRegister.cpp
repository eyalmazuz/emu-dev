#include "../include/shiftRegister.hpp"

#define MERGE(data ,value) (data >> 8 | ((uint16_t) value << 8))
#define SHIFT_LEFT(a, size) ((a >> size))
#define SHIFT_RIGHT(a, size) ((a << size))
#define GET_OFFSET(offest) (offest & 0x7)

ShiftRegister::ShiftRegister() { }

ShiftRegister::~ShiftRegister() { }

void ShiftRegister::shift(uint8_t value) {
    data = MERGE(value, data);
}

uint16_t ShiftRegister::read() {
    return data;
}

void ShiftRegister::offset(uint8_t offset) {
    offset = GET_OFFSET(offset);
    data = SHIFT_RIGHT(data, offset);
    data = SHIFT_LEFT(data, 8);
}