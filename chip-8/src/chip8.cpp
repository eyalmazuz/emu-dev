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
    opcode = 0;

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
    if (end - begin > 0xFFF - 0x200){
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

void CHIP8::cycle(){
    opcode = (memory[pc] << 8) | memory[pc + 1]; 

    switch(opcode & 0xF000){
        case 0x000:
            switch(opcode & 0x000F){
                case 0x000:
                    for (int i = 0; i < 64 * 32; i++){
                        display[i] = 0;
                    }
                    pc += 2;
                    break;
                
                case 0x000E:
                    sp--;
                    pc = stack[sp];
                    pc += 2;
                    break;
            }
            break;

        case 0x1000:
            pc = opcode & 0x0FFF;
            break;

        case 0x200:
            stack[sp] = pc;
            sp++;
            pc = opcode & 0x0FFF;
            break;

        case 0x3000:
            uint8_t val = opcode & 0x00FF;
            uint8_t x = opcode & 0x0F00;
            if (V[x] == val){
                pc += 2;
            } 
            pc += 2;
            break;

        case 0x4000:
            uint8_t val = opcode & 0x00FF;
            uint8_t x = (opcode & 0x0F00) >> 8;
            if (V[x] != val){
                pc += 2;
            } 
            pc += 2;
            break;

        case 0x5000:
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            if(V[x] == V[y]){
                pc += 2;
            }
            pc += 2;
            break;
    
        case 0x6000:
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t val = opcode & 0x00FF;
            V[x] = val;
            pc += 2;
            break;
        
        case 0x7000:
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t val = opcode & 0x00FF;
            V[x] = V[x] + val;
            pc += 2;
            break;

        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0000:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    V[x] = V[y];
                    pc += 2;
                    break;
                
                case 0x0001:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    V[x] = V[x] | V[y];
                    pc += 2;
                    break;
                

               case 0x0002:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    V[x] = V[x] & V[y];
                    pc += 2;
                    break;
                

               case 0x0003:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    V[x] = V[x] ^ V[y];
                    pc += 2;
                    break;
                

               case 0x0004:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    uint16_t result = V[x] + V[y];
                    if (result > 0xFF) {
                        V[0xF] = 1; 
                    }
                    else {
                        V[0xF] = 0;
                    }
                    V[x] = result & 0x00FF;
                    pc += 2;
                    break;
                

               case 0x0005:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    if (V[x] > V[y]) {
                        V[0xF] = 1;
                    }
                    else {
                        V[0xF] = 0;
                    }
                    V[x] = V[x] - V[y];
                    pc += 2;
                    break;
                

               case 0x0006:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    if (V[x] & 0x1){
                        V[0xF] = 1;
                    }
                    else {
                        V[0xF] = 0;
                    }
                    V[x] = V[x] >> 1;
                    pc += 2;
                    break;
                
                case 0x0007:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    if (V[y] > V[x]) {
                        V[0xF] = 1;
                    }
                    else {
                        V[0xF] = 0;
                    }
                    V[x] = V[y] - V[x];
                    pc += 2;
                    break;

                case 0x000E:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    if (V[x] & (1 << 7)) {
                        V[0xF] = 1;
                    }
                    else {
                        V[0xF] = 0;
                    }
                    V[x] = V[x] << 1;
                    pc += 2;
                    break;
            }
            break;
        
        case 0x9000:
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            if(V[x] != V[y]){
                pc += 2;
            }
            pc += 2;
            break;    

        case 0xA000:
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        
        case 0xB000:
            pc = V[0x0] + (opcode & 0x0FFF);
            break;
        
        case 0xC000:
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t val = opcode & 0x00FF;
            V[x] = val & (rand() % (0xFF + 1));
            pc += 2;
            break;

        // TODO: implement later
        case 0xD000:
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4; 
            uint8_t n = opcode & 0x000F; 
            unsigned short pixel;
            for (int i = 0; i < n; i++){
                pixel = memory[I + i];

            }
            pc += 2;
            break;

        case 0xE000:
            switch(opcode & 0x00FF){
                case 0X009E:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    if (key[x] != 0){
                        pc += 2;
                    }
                    pc += 2;
                    break;
                
                case 0X0A1:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    if (key[x] == 0){
                        pc += 2;
                    }
                    pc += 2;
                    break;
            }
            break;
    
        case 0xF000:
            switch(opcode & 0x00FF){
                case 0x0007:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    V[x] = delay;
                    pc += 2;
                    break;
                
                case 0x000A:
                    bool pressed = false;
                    for (int i = 0; i < 16; i++){
                        if(key[i] != 0){
                            uint8_t x = (opcode & 0x0F00) >> 8;
                            V[x] = i;
                            pressed = true;
                        }
                    }
                    if (!pressed){
                        return;
                    }
                    pc += 2;
                    break;

                case 0x0015:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    delay = V[x];
                    pc += 2;
                    break;

                case 0x0018:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    sound = V[x];
                    pc += 2;
                    break;
                
                case 0x001E:
                    uint8_t x = (opcode & 0X0F00) >> 8;
                    I = I + V[x];
                    pc += 2;
                    break;

                case 0x0029:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    I = memory[V[x] * 4];
                    pc += 2;
                    break;
                
                case 0x0033:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    memory[I] = V[x] / 0x64;
                    memory[I + 1] = (V[x] / 0xA)  % 0xA;
                    memory[I + 2] = V[x] % 0xA;
                    pc += 2;
                    break;

                case 0x0055:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    for (int i = 0; i < x; i++){
                        memory[I + i] = V[i];
                    }
                    pc += 2;
                    break;
                
                case 0x0065:
                    uint8_t x = (opcode & 0x0F00) >> 8;
                    for (int i = 0; i < x; i++){
                        V[i] = memory[I + i]; 
                    }
                    pc += 2;
                    break;
            }
            break;
    }
    if (delay > 0){
        delay--;
    }

    if (sound > 0){
        sound--;
    }
}

