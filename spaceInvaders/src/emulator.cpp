#include "../include/emulator.hpp"
#include <iostream>
#include <fstream>

Emulator::Emulator(size_t mem_size, size_t screen_h, size_t screen_w):
                            mem_size(mem_size), screen_h(screen_h), screen_w(screen_w)  {
    memory = new Memory(mem_size);
    video = new Video(screen_h, screen_w);
    shiftReg = new ShiftRegister();
}

Emulator::~Emulator() {
    delete memory;
    delete video;
    delete shiftReg;
}

bool Emulator::load_rom(const char* path) {
    std::streampos begin, end;
    std::ifstream rom(path, std::ios::binary);
    if (!rom.is_open()){
        std::cout << "Couldn't open file, check if the path is correct" << std::endl;
    }
    begin = rom.tellg();
    rom.seekg(0, std::ios::end);
    end = rom.tellg();
    size_t size = end - begin;

    char *contents = new char[end - begin];
    rom.seekg(0, std::ios::beg);
    rom.read(contents, size);
    rom.close();
    for (size_t i = 0; i < size; i ++){
        memory->write(i, (uint8_t)contents[i]);
    }
    delete[] contents;

    return 1;
}