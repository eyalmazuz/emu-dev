#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <stddef.h>
#include <stdint.h>

class Video {

    public:
        Video(size_t height, size_t width);
        ~Video();

        void clear();

    private:
        uint8_t **screen;
        size_t height;
        size_t width;

};

#endif