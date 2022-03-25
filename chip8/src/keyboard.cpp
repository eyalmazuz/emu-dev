#include "../include/keyboard.hpp"

Keyboard::Keyboard():pressed(false) {
    SDL_Keycode keyCodes[16] = {
        SDLK_x, SDLK_1, SDLK_2, SDLK_3,
        SDLK_q, SDLK_w, SDLK_e, SDLK_a,
        SDLK_s, SDLK_d, SDLK_z, SDLK_c,
        SDLK_4, SDLK_r, SDLK_f, SDLK_v,
    };

    for(auto key : keyCodes) {
        keys.push_back(key);
        keymap[key] = 0;
    }
};

Keyboard::~Keyboard() {
    keymap.clear();
}

void Keyboard::keyPress(SDL_Keycode key) {
    keymap[key] = 1;
    pressed = true;
}

void Keyboard::keyRelease(SDL_Keycode key) {
    keymap[key] = 0;
    pressed = false;
}

void Keyboard::clear() {
    for(auto &it: keymap) {
        keymap[it.first] = 0;
    }
}

bool Keyboard::isPressed(size_t index) {
    SDL_Keycode key = keys[index];
    return keymap[key];
}

bool Keyboard::anyPressed(){
    return pressed;
}