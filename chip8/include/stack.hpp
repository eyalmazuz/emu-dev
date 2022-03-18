#ifndef _STACK_H_
#define _STACK_H_

#include <stdint.h>
#include <stddef.h>

class Stack {

    public:
        Stack(size_t size);
        ~Stack();
        
        void push(size_t addr);
        uint16_t pop();
        size_t getSize();
        void clear();

    private:
        uint16_t sp;
        size_t size;
        uint16_t *stack;
};


#endif