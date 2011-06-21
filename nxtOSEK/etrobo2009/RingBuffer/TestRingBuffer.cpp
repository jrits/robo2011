#include <stdio.h>
#include "RingBuffer.hpp"

// /usr/bin/g++ TestRingBuffer.cpp
int main() {
    const int num = 3;
    int buf[num] = {0};
    RingBuffer<int> buffer(buf, num); // Note:  sizeof(buf) may not eqaul to num.
    buffer.push(1);
    buffer.push(2);
    buffer.push(3);
    for (int i = 0; i > -num; i--) {
        printf("%d\n", buffer.get(i));
        // 3 2 1
    }
    buffer.push(4);
    for (int i = 0; i > -num; i--) {
        printf("%d\n", buffer.get(i));
        // 4 3 2
    }
    //delete buffer;
}
