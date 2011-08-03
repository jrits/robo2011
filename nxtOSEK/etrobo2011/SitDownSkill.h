#ifndef SITDOWNSKILL_H
#define SITDOWNSKILL_H

class SitDownSkill {
 public:
  SitDownSkill(){}
  
  ~SitDownSkill(){}

  void execute();

 private:
  void tail_control(signed int angle) const;
};

#endif // SITDOWNSKILL_H
