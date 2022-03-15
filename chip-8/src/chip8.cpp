#include "../include/chip8.hpp"
#include <iostream>
#include <fstream>
#include <stdint.h>

uint8_t fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

CHIP8::CHIP8(){}

CHIP8::~CHIP8(){}

void CHIP8::init() {
    for (int i = 0; i < 16; i++){
        stack[i] = 0;
        V[i] = 0;
        key[i] = 0;
    }
    sp = 0;

    for (int i = 0; i < 4096; i++){
        memory[i] = 0;
    }
    for (int i = 0; i < 64*32; i++){
        display[i] = 0;
    }
    
    for (int i = 0; i < 80; i++){
        memory[i] = fontset[i];
    }

    I = 0;
    pc = 0x200;

    delay = 0;
    sound = 0;
}


bool CHIP8::load_rom(const char* path) {

    init();    

    std::streampos begin, end;
    std::ifstream rom(path, std::ios::binary);
    if (!rom.is_open()){
        std::cout << "Couldn't open file, check if the path is correct" << std::endl;
    }
    begin = rom.tellg();
    rom.seekg(0, std::ios::end);
    end = rom.tellg();
    if (end - begin > 0xfff - 0x200){
        std::cout << "File is too big" << std::endl;
        return 0;
    }

    char *contents = new char[end - begin];
    rom.seekg(0, std::ios::beg);
    rom.read(contents, end - begin);
    rom.close();
    for (int i = 0; i < end - begin; i ++){
        memory[pc + i] = (uint8_t)contents[i];
    }
    delete[] contents;

    return 1;
}

int main(int argc, char **argv){
    
    if (argc != 2){
        std::cout << "Please run the emulator as ./emu <rom path>" << std::endl;;
    }
    char *path = argv[1];
    std::cout << path << std::endl;

    CHIP8 *chip8 = new CHIP8();
    int result = chip8->load_rom(path);
    if (result == 0){
        return 1;
    }

    return 0;
}
