#ifndef _IO_PORTS_H_
#define _IO_PORTS_H_

#include <stdint.h>
#include <stddef.h>

class IO {

    public:
        IO();
        ~IO();

        uint8_t readPort(size_t port);
        void writePort(size_t port, uint8_t value);

    private:
        uint8_t readPorts[4];
        uint8_t writePorts[5];
};

#endif