#include "../include/chip8.hpp"
#include <iostream>
#include <SDL2/SDL.h>

uint8_t keymap[16] = {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v,
};

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

    SDL_Renderer *renderer;
    SDL_Window *window;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Couldn't initialize SDL " << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 512, 0);
    if (!window){
        std::cout << "Failed to open window " << SDL_GetError() << std::endl;
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer){
        std::cout << "Failed to create renderer " << SDL_GetError() << std::endl;
        exit(1);
    }

    while(true){
        chip8->cycle();

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                exit(0);
            }

            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    exit(0);
                }
                for (int i = 0; i < 16; i++){
                    if(event.key.keysym.sym == keymap[i]) {
                        chip8->key[i] = 1;
                    }
                }
            }
            if(event.type == SDL_KEYUP) {
               for (int i = 0; i < 16; i++){
                    if(event.key.keysym.sym == keymap[i]) {
                        chip8->key[i] = 0;
                    }
                } 
            }
        }
        SDL_RenderSetScale(renderer, 16, 16);
        if(chip8->draw) {
            chip8->draw = false;

            //TODO: process graphics
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            for(int i = 0; i < 32; i++){
                for(int j = 0; j < 64; j++){
                    if(chip8->display[j + i * 64] == 1){
                        
                        SDL_RenderDrawPoint(renderer, j, i);
                    }
                }
            }
            SDL_RenderPresent(renderer); 
            
        }
        // Delay the game
        SDL_Delay(5);
    }

    return 0;
}