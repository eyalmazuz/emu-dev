#include "../include/chip8.hpp"
#include <iostream>

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

    //TODO: init graphics

    while(true){
        chip8->cycle();

        //TODO: process graphics
    }

    return 0;
}