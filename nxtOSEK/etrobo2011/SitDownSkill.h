#ifndef SITDOWNSKILL_H
#define SITDOWNSKILL_H

class SitDownSkill {
 public:
  SitDownSkill();
  
  ~SitDownSkill();

  void execute();

  signed int getAngle() const;

  void setAngle(signed int angle);

 private:
  signed int mSitDownAngle;
  void tail_control(signed int angle) const;

  bool isStable() const;
  bool isSeated() const;
  bool isInclined() const;

  enum eState{
    INIT = 0,
    STOP,
    INCLINED_BACKWARD,
    SIT_DOWN,
    DONE
  };

  eState mCurrentState;
};

#endif // SITDOWNSKILL_H
