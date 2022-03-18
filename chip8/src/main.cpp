#include "../include/emulator.hpp"
#include <iostream>

int main(int argc, char **argv){
    
    if (argc != 2){
        std::cout << "Please run the emulator as ./emu <rom path>" << std::endl;;
        return 1;
    }
    char *path = argv[1];
    std::cout << path << std::endl;

    Emulator *chip8 = new Emulator(16, 4096, 16, 32, 64);

    if (!chip8->loadRom(path)){
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
                    SDL_Quit();
                    exit(0);
                }
                chip8->keyPressed(event.key.keysym.sym);
            }
            if(event.type == SDL_KEYUP) {
               chip8->keyPressed(event.key.keysym.sym);
            }
        }
        SDL_RenderSetScale(renderer, 16, 16);
        if(chip8->canDraw()) {
            chip8->setDraw(false);

            //TODO: process graphics
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            for(int x = 0; x < 64; x++){
                for(int y = 0; y < 32; y++){
                    if(chip8->isDrawn(x, y)){
                        
                        SDL_RenderDrawPoint(renderer, x, y);
                    }
                }
            }
            SDL_RenderPresent(renderer); 
            
        }
        // Delay the game
        SDL_Delay(10);
    }

    return 0;
}