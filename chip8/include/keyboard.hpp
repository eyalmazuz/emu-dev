#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>
#include <unordered_map>
#include <vector>
#include <SDL2/SDL.h>

class Keyboard {

    public:
        Keyboard();
        ~Keyboard();

        void keyPress(SDL_Keycode key);
        void keyRelease(SDL_Keycode key);
        bool isPressed(size_t index);
        bool anyPressed();

        void clear();

    private:
        std::unordered_map<SDL_Keycode, int> keymap;
        std::vector<SDL_Keycode> keys;
        bool pressed;
};

#endif