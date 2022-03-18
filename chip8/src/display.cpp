#include <../include/display.hpp>

Display::Display(size_t width, size_t height):width(width), height(height) {
    screen = new uint8_t*[height];
    for(size_t i = 0; i < height; i++){
        screen[i] = new uint8_t[width];
    }
}

Display::~Display() {
    for(size_t i = 0; i < height; i++){
        delete[] screen[i];
    }
    delete[] screen;
}

void Display::clear() {
    for(size_t i = 0; i < height; i++) {
        for(size_t j = 0; j < width; j++){
            screen[i][j] = 0;
        }
    }
}

void Display::draw(size_t y, size_t x) {
    screen[y][x] ^= 1;
} 

bool Display::isDrawn(size_t x, size_t y){
    return screen[x][y];
}