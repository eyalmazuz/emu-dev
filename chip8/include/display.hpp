#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdint.h>
#include <stddef.h>

class Display {

    public:
        Display(size_t width, size_t height);
        ~Display();

        void clear();
        void draw(size_t y, size_t x);
        bool isDrawn(size_t y, size_t x);
    private:
        size_t width;
        size_t height;
        uint8_t **screen;

};

#endif