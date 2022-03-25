#include "../include/memory.hpp"

Memory::Memory(size_t size): size(size) {

    memory = new uint8_t[size];
}

Memory::~Memory() {
    delete[] memory;
}