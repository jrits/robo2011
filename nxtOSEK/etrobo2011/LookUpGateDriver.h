#ifndef LOOKUPGATEDRIVER_H
#define LOOKUPGATEDRIVER_H

#include "Driver.h"

class LookUpGateDriver : public Driver {
 public:
  LookUpGateDriver();
  ~LookUpGateDriver();
  bool drive();

 private:

  //! �Q�[�g��������B
  /**
     @return �Q�[�g����������true��Ԃ��B
   */
  bool foundGate() const;

  //! �Q�[�g�𒴂��������肷��B
  /**
     Check whether nxt passed the look-up gate.


     @return return true if Nxt passed the look-up gate.
   */
  bool passedGate();
  //! �Q�[�g��O�ō��荞�ށB
  bool sitDown();
  //! �Q�[�g�ʉߌ�ɗ����オ��B
  bool standUp();
  //! LookUpGateDriver�̏������I�����������肷��B
  /**
     @return �I�����Ă���ꍇ��true�B
  */
  bool isDone() const;

  //! �Q�[�g�̐^���ɂ��邩���肷��B
  bool isUnderGate() const;
  //! �T�u��Ԃ�\������enum�B
  enum eSubSection {
    INIT = 0,//!< ������ԁB
    IN_FRONT_OF_GATE, //!< �Q�[�g��O��ԁB
    UNDER_GATE, //!< �Q�[�g���B
    BEHIND_GATE,//!<�@�Q�[�g��
    DONE
  };

  //! �T�u��Ԃ�\������t�B�[���h�B
  eSubSection mCurrentSubSection;

  //! �X�^�[�g�n�_����Q�[�g�܂ł̋����B
  float mDistanceAtGate;
  //! �Q�[�g�ʉ݌�ɁA�I�[�o�[�������鋗���B
  float mOverrunDistance;
  //! �Q�[�g�ŁA���W�̏C�����s�������ǂ������肷��t���O�B
  bool mAdjustCoordinatesFlag;
};

#endif
