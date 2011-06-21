#include <stdio.h>
#include "SmartRingBuffer.hpp"
#include "boost/scoped_ptr.hpp"

// /usr/bin/g++ -I/usr/include/boost-1_33_1 TestSmartRingBuffer.cpp
int main() {
    int num = 3;
    boost::scoped_ptr< SmartRingBuffer<int> > buf_ptr(new SmartRingBuffer<int>(num));
    SmartRingBuffer<int> *buffer = buf_ptr.get();
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
    //delete buffer;
}


