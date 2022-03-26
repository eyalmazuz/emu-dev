#include "../include/ioPorts.hpp"

IO::IO() { }

IO::~IO() { }

uint8_t IO::readPort(size_t port) {
    return readPorts[port];
}

void IO::writePort(size_t port, uint8_t value) {
    writePorts[port - 2] = value;
}