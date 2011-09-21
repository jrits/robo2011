///////////////////////////////////////////////////////////
//  Detector.h
//  Implementation of the Interface Detector
//  Created on:      27-9-2010 10:12:12
//  Original author: nseo
///////////////////////////////////////////////////////////

#if !defined(EA_3E87576D_66A3_4a67_87CD_9668CD4E1E41__INCLUDED_)
#define EA_3E87576D_66A3_4a67_87CD_9668CD4E1E41__INCLUDED_

/**
 * 検出系クラスインターフェース
 */
class Detector
{
public:
    /**
     * コンストラクタ
     */
    Detector(){}
    /**
     * デストラクタ
     */
    ~Detector(){}
    /**
     * 検出する
     *
     * @retval true 検出
     * @retval false 未検出
     */
    bool detect();
};

#endif // !defined(EA_3E87576D_66A3_4a67_87CD_9668CD4E1E41__INCLUDED_)
