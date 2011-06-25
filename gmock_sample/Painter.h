#ifndef PAINTER_H
#define PAINTER_H

#include "Turtle.h"

class Painter {
 public:
  Painter(Turtle* turtle){
    this->turtle = turtle;
  }
  virtual ~Painter(){
  }

  bool DrawCircle(int x, int y, int radius){
    //メソッドをテストしながら、実装したい。
    return true;
  }

 private:
  Turtle* turtle;
};

#endif
