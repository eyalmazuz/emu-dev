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

                default:
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
            if (V[opcode & 0x0F00] == opcode & 0x00FF){
                pc += 2;
            } 
            pc += 2;
            break;

        case 0x4000:
            if (V[opcode & 0x0F00] == opcode & 0x00FF){
                pc += 2;
            } 
            pc += 2;
            break;

        case 0x5000:
            if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]){
                pc += 2;
            }
            pc += 2;
            break;
    
        case 0x6000:
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
            break;
        
        case 0x7000:
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
            break;

        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0000:
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0X00F0) >> 4];
                    pc += 2;
                    break;
                
                case 0x0001:
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                

               case 0x0002:
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                

               case 0x0003:
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                

               case 0x0004:
                    if (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]> 0xFF) {
                        V[0xF] = 1; 
                    }
                    else {
                        V[0xF] = 0;
                    }
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                

               case 0x0005:
                    if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]) {
                        V[0xF] = 1;
                    }
                    else {
                        V[0xF] = 0;
                    }
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                    break;
                

               case 0x0006:
                    if (V[(opcode & 0x0F00) >> 8] & 0x1){
                        V[0xF] = 1;
                    }
                    else {
                        V[0xF] = 0;
                    }
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    pc += 2;
                    break;
                
                case 0x0007:
                    if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]) {
                        V[0xF] = 1;
                    }
                    else {
                        V[0xF] = 0;
                    }
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;

                case 0x000E:
                    if (V[(opcode & 0x0F00) >> 8] & (1 << 7)) {
                        V[0xF] = 1;
                    }
                    else {
                        V[0xF] = 0;
                    }
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    pc += 2;
                    break;
           
                default:
                    break; 
            }
            break;
        
        case 0x9000:
            if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]){
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
            V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF) >> 8 & (rand() % (0xFF + 1));
            pc += 2;
            break;

        // TODO: implement later
        case 0xD000: {
            
            unsigned short x = (opcode & 0x0F00) >> 8;
            unsigned short y = (opcode & 0x00F0) >> 4; 
            unsigned short n = opcode & 0x000F; 
            unsigned short pixel;
            for (int i = 0; i < n; i++){
                pixel = memory[I + i];

            }
            pc += 2;
            draw = true;
            break;
        }

        case 0xE000:
            switch(opcode & 0x00FF){
                case 0X009E:
                    if (key[(opcode & 0x0F00) >> 8] != 0){
                        pc += 2;
                    }
                    pc += 2;
                    break;
                
                case 0X0A1:
                    if (key[(opcode & 0x0F00) >> 8] == 0){
                        pc += 2;
                    }
                    pc += 2;
                    break;

                default:
                    break;
            }
            break;
    
        case 0xF000:
            switch(opcode & 0x00FF){
                case 0x0007:
                    V[(opcode & 0x0F00) >> 8] = delay;
                    pc += 2;
                    break;
                
                case 0x000A: {
                    bool pressed = false;
                    for (int i = 0; i < 16; i++){
                        if(key[i] != 0){
                            V[(opcode & 0x0F00) >> 8] = i;
                            pressed = true;
                        }
                    }
                    if (!pressed){
                        return;
                    }
                    pc += 2;
                    break;
                }
                case 0x0015:
                    delay = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;

                case 0x0018:
                    sound = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                
                case 0x001E:
                    I = I + V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;

                case 0x0029:
                    I = V[(opcode & 0x0F00) >> 8] * 0x5;
                    pc += 2;
                    break;
                
                case 0x0033:
                    memory[I] = V[(opcode & 0x0F00) >> 8] / 0x64;
                    memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 0xA)  % 0xA;
                    memory[I + 2] = V[(opcode & 0x0F00) >> 8] % 0xA;
                    pc += 2;
                    break;

                case 0x0055:
                    for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++){
                        memory[I + i] = V[i];
                    }
                    pc += 2;
                    break;
                
                case 0x0065:
                    for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++){
                        V[i] = memory[I + i]; 
                    }
                    pc += 2;
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }

    if (delay > 0){
        delay--;
    }

    if (sound > 0){
        sound--;
    }
}

