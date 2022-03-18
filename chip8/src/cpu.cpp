#include <../include/cpu.hpp>

Cpu::Cpu(size_t registerCount):registerCount(registerCount), I(0), delay(0), sound(0) {
    V = new uint8_t[registerCount];
}

Cpu::~Cpu() {
    delete[] V;
}

size_t Cpu::getNumRegisters() {
    return registerCount;
}

uint16_t Cpu::getI(){
    return I;
}

void Cpu::setI(uint16_t value) {
    I = value;
}

uint8_t Cpu::getRegister(size_t index) {
    return V[index];
}

void Cpu::writeRegister(size_t index, uint8_t value) {
    V[index] = value;
}


void Cpu::clear() {
    for (size_t i = 0; i < registerCount; i++) {
        V[i] = 0;
    }
    I = 0;
}