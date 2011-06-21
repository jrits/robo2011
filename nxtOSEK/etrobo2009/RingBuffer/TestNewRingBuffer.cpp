#include <stdio.h>
#include "NewRingBuffer.hpp"

// /usr/bin/g++ TestNewRingBuffer.cpp
int main() {
    int num = 3;
    NewRingBuffer<int> *buffer = new NewRingBuffer<int>(num);
    buffer->push(1);
    buffer->push(2);
    buffer->push(3);
    for (int i = 0; i < num; i++) {
        printf("%d\n", buffer->get(i));
        // 3 2 1
    }
    buffer->push(4);
    for (int i = 0; i < num; i++) {
        printf("%d\n", buffer->get(i));
        // 4 3 2
    }
    delete buffer;
}
