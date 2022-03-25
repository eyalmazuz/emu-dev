#include "../include/emulator.hpp"
#include <iostream>

int main(int argc, char **argv){
    
    if (argc != 2){
        std::cout << "Please run the emulator as ./emu <rom path>" << std::endl;;
        return 1;
    }
    char *path = argv[1];
    std::cout << path << std::endl;

    Emulator *emulator = new Emulator(0x43FF, 256, 224);

    if (!emulator->load_rom(path)){
        return 1;
    }

    return 0;

}
