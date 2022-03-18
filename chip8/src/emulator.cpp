#include "../include/emulator.hpp"
#include <iostream>
#include <fstream>


#define fetchInstruction(m1, m2) (((m1) << 8) | (m2)); 
#define getAddr(opcode) ((opcode) & 0x0FFF)
#define Vx(opcode) (((opcode) & 0X0F00) >> 8)
#define Vy(opcode) (((opcode) & 0x00F0) >> 4)
#define Value(opcode) ((opcode) & 0x00FF)
#define getRand(opcode) (((opcode) & 0x00FF) & (rand() % (0xFF + 1)))

Emulator::Emulator(size_t registerCount, size_t memorySize, size_t stackSize, size_t displayHeight, size_t displayWidth) {
    cpu = new Cpu(registerCount);
    memory = new Memory(memorySize);
    stack = new Stack(stackSize);
    display = new Display(displayHeight, displayWidth);
    keyboard = new Keyboard();

}

Emulator::~Emulator(){
    delete cpu;
    delete memory;
    delete stack;
    delete display;
    delete keyboard;
}

void Emulator::init() {
    pc = 0x200;
    opcode = 0;

    cpu->clear();
    display->clear();
    stack->clear();

    keyboard->clear();

    loadFontSet();

    delay = 0;
    sound = 0; 

    draw = false;
}

void Emulator::loadFontSet() {
    unsigned char fontset[80] = {
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
    
    for (int i = 0; i < 80; i++){
        memory->write(i, fontset[i]);
    }

}

bool Emulator::loadRom(const char* path) {
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
        memory->write(pc + i, (uint8_t)contents[i]);
    }
    delete[] contents;

    return 1;
}

void Emulator::cycle() {
    opcode = fetchInstruction(memory->read(pc), memory->read(pc + 1));

    switch(opcode & 0xF000){
        case 0x0000:
            switch(opcode & 0x000F){
                case 0x0000:
                    display->clear();
                    draw = true;
                    pc += 2;
                    break;
                
                case 0x000E:
                    pc = stack->pop();
                    pc += 2;
                    break;

                default:
                    break;
            }
            break;

        case 0x1000:
            pc = getAddr(opcode);
            break;

        case 0x2000:
            stack->push(pc);
            pc = getAddr(opcode);
            break;

        case 0x3000:
            if (cpu->getRegister(Vx(opcode)) == Value(opcode)){
                pc += 2;
            } 
            pc += 2;
            break;

        case 0x4000:
            if (cpu->getRegister(Vx(opcode)) != Value(opcode)){
                pc += 2;
            } 
            pc += 2;
            break;

        case 0x5000:
            if(cpu->getRegister(Vx(opcode)) == cpu->getRegister(Vy(opcode))){
                pc += 2;
            }
            pc += 2;
            break;
    
        case 0x6000:
            cpu->writeRegister(Vx(opcode), Value(opcode));
            pc += 2;
            break;
        
        case 0x7000:
            cpu->writeRegister(Vx(opcode), cpu->getRegister(Vx(opcode)) + Value(opcode));
            pc += 2;
            break;

        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0000:
                    cpu->writeRegister(Vx(opcode), cpu->getRegister(Vy(opcode)));
                    pc += 2;
                    break;
                
                case 0x0001:
                    cpu->writeRegister(Vx(opcode), cpu->getRegister(Vx(opcode)) | cpu->getRegister(Vy(opcode)));
                    pc += 2;
                    break;
                

               case 0x0002:
                    cpu->writeRegister(Vx(opcode), cpu->getRegister(Vx(opcode)) & cpu->getRegister(Vy(opcode)));
                    pc += 2;
                    break;
                

               case 0x0003:
                    cpu->writeRegister(Vx(opcode), cpu->getRegister(Vx(opcode)) ^ cpu->getRegister(Vy(opcode)));
                    pc += 2;
                    break;
                

               case 0x0004:
                    cpu->writeRegister(Vx(opcode), cpu->getRegister(Vx(opcode)) + cpu->getRegister(Vy(opcode)));
                    if (cpu->getRegister(Vy(opcode)) > (0xFF - cpu->getRegister(Vx(opcode)))) {
                        cpu->writeRegister(0xF, 1); 
                    }
                    else {
                        cpu->writeRegister(0xF, 0);
                    }
                    pc += 2;
                    break;
                

               case 0x0005:
                    if (cpu->getRegister(Vx(opcode)) > cpu->getRegister(Vy(opcode))) {
                        cpu->writeRegister(0xF, 1);
                    }
                    else {
                        cpu->writeRegister(0xF, 0);
                    }
                    cpu->writeRegister(Vx(opcode), cpu->getRegister(Vx(opcode)) - cpu->getRegister(Vy(opcode)));
                    pc += 2;
                    break;
                

               case 0x0006:
                    cpu->writeRegister(0xF, cpu->getRegister(Vx(opcode)) & 0x1);
                    cpu->writeRegister(Vx(opcode), cpu->getRegister(Vx(opcode)) >> 1);
                    pc += 2;
                    break;
                
                case 0x0007:
                    if (cpu->getRegister(Vy(opcode)) > cpu->getRegister(Vx(opcode))) {
                        cpu->writeRegister(0xF, 1);
                    }
                    else {
                        cpu->writeRegister(0xF, 0);
                    }
                    cpu->writeRegister(Vx(opcode), cpu->getRegister(Vy(opcode)) - cpu->getRegister(Vx(opcode)));
                    pc += 2;
                    break;

                case 0x000E:
                    cpu->writeRegister(0xF, cpu->getRegister(Vx(opcode)) >> 7);
                    cpu->writeRegister(Vx(opcode), cpu->getRegister(Vx(opcode)) << 1);
                    pc += 2;
                    break;
           
                default:
                    break; 
            }
            break;
        
        case 0x9000:
            if(cpu->getRegister(Vx(opcode)) != cpu->getRegister(Vy(opcode))){
                pc += 2;
            }
            pc += 2;
            break;    

        case 0xA000:
            cpu->setI(getAddr(opcode));
            pc += 2;
            break;
        
        case 0xB000:
            pc = cpu->getRegister(0x0) + getAddr(opcode);
            break;
        
        case 0xC000:
            cpu->writeRegister(Vx(opcode), getRand(opcode));
            pc += 2;
            break;
;
        // TODO: implement later
        case 0xD000: {
            
            unsigned short x = cpu->getRegister(Vx(opcode));
            unsigned short y = cpu->getRegister(Vy(opcode)); 
            unsigned short n = opcode & 0x000F; 
            unsigned short pixel;
            
            cpu->writeRegister(0xF, 0);
            for (int i = 0; i < n; i++){
                pixel = memory->read(cpu->getI() + i);
                
                for (int j = 0; j < 8; j++){
                    if((pixel & (0x80 >> j)) != 0){
                        if(display->isDrawn(x + j, y + i)) {
                            cpu->writeRegister(0xF, 1);
                        }
                        display->draw(x + j, y + i);
                    }
                }
            }
            pc += 2;
            draw = true;
        }
            break;

        case 0xE000:
            switch(opcode & 0x00FF){
                case 0X009E:
                    if (keyboard->isPressed(Vx(opcode)) != 0){
                        pc += 2;
                    }
                    pc += 2;
                    break;
                
                case 0X00A1:
                    if (keyboard->isPressed(Vx(opcode)) == 0){
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
                    cpu->writeRegister(Vx(opcode), delay);
                    pc += 2;
                    break;
                
                case 0x000A: {
                    if (!keyboard->anyPressed()){
                        return;
                    }
                    pc += 2;
                }
                    break;

                case 0x0015:
                    delay = cpu->getRegister(Vx(opcode));
                    pc += 2;
                    break;

                case 0x0018:
                    sound = cpu->getRegister(Vx(opcode));
                    pc += 2;
                    break;
                
                case 0x001E:
                    if(cpu->getI() + cpu->getRegister(Vx(opcode)) > 0xFFF) {
                        cpu->writeRegister(0xF, 1);
                    }
                    else {
                        cpu->writeRegister(0xF, 0);
                    }
                    cpu->setI(cpu->getI() + cpu->getRegister(Vx(opcode)));
                    pc += 2;
                    break;

                case 0x0029:
                    cpu->setI(cpu->getRegister(Vx(opcode)) * 0x5);
                    pc += 2;
                    break;
                
                case 0x0033:
                    memory->write(cpu->getI(), cpu->getRegister(Vx(opcode)) / 100);
                    memory->write(cpu->getI() + 1, (cpu->getRegister(Vx(opcode)) / 10)  % 10);
                    memory->write(cpu->getI() + 2, cpu->getRegister(Vx(opcode)) % 10);
                    pc += 2;
                    break;

                case 0x0055:
                    for (int i = 0; i <= Vx(opcode); i++){
                        memory->write(cpu->getI() + i, cpu->getRegister(i));
                    }
                    pc += 2;
                    break;
                
                case 0x0065:
                    for (int i = 0; i <= Vx(opcode); i++){
                        cpu->writeRegister(i, memory->read(cpu->getI() + i));
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


void Emulator::keyPressed(SDL_Keycode key) {
    keyboard->keyPress(key);
}

void Emulator::keyReleased(SDL_Keycode key) {
    keyboard->keyRelease(key);
}

bool Emulator::canDraw() {
    return draw;
}

void Emulator::setDraw(bool drawFlag) {
    draw = drawFlag;
}

uint8_t Emulator::isDrawn(size_t x, size_t y) {
    return display->isDrawn(x, y);
}