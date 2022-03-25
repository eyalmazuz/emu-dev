#include "../include/video.hpp"

Video::Video(size_t height, size_t width):height(height), width(width) {
    screen = new uint8_t*[height];
    for(size_t i = 0; i < height; i++){
        screen[i] = new uint8_t[width];
    }
}

Video::~Video() {
    for(size_t i = 0; i < height; i++){
        delete[] screen[i];
    }
    delete[] screen;
}

void Video::clear() {
    for(size_t i = 0; i < height; i++) {
        for(size_t j = 0; j < width; j++){
            screen[i][j] = 0;
        }
    }
}