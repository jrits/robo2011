///////////////////////////////////////////////////////////
//  TestHistory.cpp
//  testcode of the Class Histrory
//  Created on:      29-9-2010 9:49:00
//  Original author: nseo
///////////////////////////////////////////////////////////

#include <iostream>
using namespace std;
#include <cassert>
#include "../History.h"
// /usr/bin/g++ TestHistory.cpp ../History.cpp -I.

// 単体テスト初期化関数(テストメソッド毎に先頭で呼ぶ)
void testHistory_setUp()
{
}

// 単体テスト終了関数(テストメソッド毎に末尾で呼ぶ)
void testHistory_tearDown()
{
}

void testHistory_updateAndGet()
{
    testHistory_setUp();
    float buffer[3];
    History history(buffer, sizeof(buffer) / sizeof(buffer[0]));
    history.update(1);
    assert(history.get(0) == 1);
    history.update(2);
    assert(history.get(0) == 2);
    history.update(3);
    assert(history.get(0) == 3);
    assert(history.get(-1) == 2);
    assert(history.get(-2) == 1);
    assert(history.get(-3) == 3);
    assert(history.get(1) == 1);
    assert(history.get(2) == 2);
    assert(history.get(3) == 3);
    history.update(4);
    assert(history.get(0) == 4);
    assert(history.get(-1) == 3);
    assert(history.get(-2) == 2);
    assert(history.get(-3) == 4);
    testHistory_tearDown();
}

void testHistory_calcDifference()
{
    testHistory_setUp();
    float buffer[3] = {0};
    History history(buffer, sizeof(buffer) / sizeof(buffer[0]));
    history.update(1);
    assert(history.calcDifference() == 1);
    history.update(2);
    assert(history.calcDifference() == 2);
    history.update(3);
    assert(history.calcDifference() == 2);
    history.update(4);
    assert(history.calcDifference() == 2);
    testHistory_tearDown();
}

void testHistory_calcAverage()
{
    testHistory_setUp();
    float buffer[3] = {0};
    History history(buffer, sizeof(buffer) / sizeof(buffer[0]));
    history.update(3);
    assert(history.calcAverage() == 1.0);
    history.update(6);
    assert(history.calcAverage() == 3.0);
    history.update(9);
    assert(history.calcAverage() == 6.0);
    history.update(12);
    assert(history.calcAverage() == 9.0);
    testHistory_tearDown();
}

int main()
{
    testHistory_updateAndGet();
    testHistory_calcDifference();
    testHistory_calcAverage();
    printf("OK!\n");
}
