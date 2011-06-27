#ifndef NXTIF_H
#define NXTIF_H

typedef short S16;

class NxtIF {
 public:
  enum eButton {
    RUN_ON = 0x01,
    ENTR_ON = 0x02,
    RUN_ENTR_ON = (RUN_ON|ENTR_ON),
    BUTTONS_OFF = 0x00
  };
  
  virtual ~NxtIF(){};
  virtual eButton getButtons() = 0;
  virtual S16 getBattMv() = 0;
};

#endif
