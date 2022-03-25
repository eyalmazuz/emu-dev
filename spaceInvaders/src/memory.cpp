#include "../include/memory.hpp"

Memory::Memory(size_t size): size(size) {

    memory = new uint8_t[size];
}

Memory::~Memory() {
    delete[] memory;
}

void Memory::write(size_t addr, uint8_t value) {
    memory[addr] = value;
}

uint8_t Memory::read(size_t addr) {
    return memory[addr];
}