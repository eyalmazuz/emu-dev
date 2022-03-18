#include <../include/memory.hpp>

Memory::Memory(size_t size):size(size) {

    ram = new uint8_t[size];
}

Memory::~Memory() {
    delete [] ram;
}

uint8_t Memory::read(size_t addr) {
    return ram[addr];
}

void Memory::write(size_t addr, uint8_t value) {
    ram[addr] = value;
}

void Memory::clear() {
    for (size_t i = 0; i < size; i++) {
        ram[i] = 0;
    }
}