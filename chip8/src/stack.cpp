#include "../include/stack.hpp"

Stack::Stack(size_t size):sp(0), size(size) {
    stack = new uint16_t[size];
}

Stack::~Stack() { 
    delete[] stack;
}

void Stack::push(size_t addr) {
    stack[sp] = addr;
    sp++;
}

uint16_t Stack::pop() {
    sp--;
    uint16_t addr = stack[sp];
    return addr;
}

size_t Stack::getSize() {
    return size;
}

void Stack::clear() {
    sp = 0;
    for(size_t i = 0; i < size; i++) {
        stack[i] = 0;
    }
}