///////////////////////////////////////////////////////////
//  SeesawDriver.cpp
//  Implementation of the Class SeesawDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: masui
///////////////////////////////////////////////////////////

#include "SeesawDriver.h"
#include "factory.h"
#include "constants.h"
#include "Speaker.h"
#include "Gps.h"

#define RETURNTIMING 375 //値を返すタイミング
#define LIGHTBUF 10 //光センサヒストリのバッファサイズ
#define LATEST 0
#define TARGET_ANGLE 356

/**
 * コンストラクタ
 */
SeesawDriver::SeesawDriver()    //初期値の設定
{
    mTimeCounter = 0;
    mState = SeesawDriver::INIT;  //ステート
    mInitState = true;
    mDoDetectWall = false;

}

/**
 * デストラクタ
 */
SeesawDriver::~SeesawDriver(){}

/**
 * 走行する。シーソーを攻略する。
 *
 * @retval true 最終状態。これ以降の状態遷移なし。
 * @retval false まだ役目が完了していません。
 */
bool SeesawDriver::drive()
{
#if 1
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
    LOGGER_DATAS16[0] = (S16)(mGps.getXCoordinate());
    LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
    LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
    LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
//    LOGGER_DATAS32[0] = (S32)(mGyroHistory.calcDifference());
#endif
    
    // 初期化関数を作るのが面倒なのでとりあえずここで
    if (mState == SeesawDriver::INIT) {
        gDoMaimai = false;
        mLightPid.reset(60,0,180);
        K_THETADOT = 7.5F;
        mState = SeesawDriver::BEFORELINETRACE;
//        mState = SeesawDriver::LINERETURN; /// ラインリターンだけしたい時
        mTimeCounter = 0;
        mDoDetectWall = false;
        mWallDetector.setThreshold(110);
    }
    // シーソーのライントレース。段差にぶつかるまで。
    else if (mState == SeesawDriver::BEFORELINETRACE) {
        // 段差検知なしでライントレース
        if (! mDoDetectWall) {
            mLineTrace.setForward(70);
            mLineTrace.execute();
            if(mGps.getXCoordinate() > 3000 && mGps.getYCoordinate() > -1100) {
                { Speaker s; s.playTone(480, 20, 100); }
                mDoDetectWall = true;
            }
        }
        // 段差検知しながらアングルトレース
        if (mDoDetectWall) {
	        { Speaker s; s.playTone(1976, 10, 100); }
            K_THETADOT = 8.5F;
            mAngleTrace.setTargetAngle(TARGET_ANGLE);
            mAngleTrace.setForward(100);
            mActivator.reset(USER_GYRO_OFFSET + 5); // ちょっと急発進
            mAngleTrace.execute();
            if (mWallDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                mInitState = true;
                mState = SeesawDriver::FIRSTRUN;
                // ぶつかったら座標補正2011
                mGps.adjustXCoordinate(3245.0);//ぶつかったら座標補正
                mGps.adjustYCoordinate(-903.0);//ぶつかったら座標補正
            }
        }
    }
//         // 段差検知しながらライントレース
//         if (mDoDetectWall) {
//             mLineTrace.setForward(80);
//             mLineTrace.execute();
//             if (mWallDetector.detect()) {
//                 { Speaker s; s.playTone(1976, 10, 100); }
//                 mState = SeesawDriver::ON0THSTAGE_BACK;
//                 mInitState = true;
//                 // ぶつかったら座標補正2011
//                 mGps.adjustXCoordinate(3350.0);//ぶつかったら座標補正
//                 mGps.adjustYCoordinate(-903.0);//ぶつかったら座標補正
//                 //mGps.adjustDirection(TARGET_ANGLE);
//             }
//         }
//     }
//     // ０段目(階段前)。一旦バックして助走距離をとる
//     if (mState == SeesawDriver::ON0THSTAGE_BACK) {
//         if (mInitState) {
//             mPrevMotor = mLeftMotor.getCount();
//             mDoDetectWall = false;
//             mInitState = false;
//             { Speaker s; s.playTone(1976, 10, 100); }
//         }
//         // 一旦バック(フラグ名が適切ではないが気にしないでください)
//         if (! mDoDetectWall) {
//             mAngleTrace.setForward(-50);
//             mAngleTrace.setTargetAngle(TARGET_ANGLE);
//             K_THETADOT = 7.5F;
//             mAngleTrace.execute();
//             if (mGps.getXCoordinate() < 3150) { // シーソー側マーカ始点
//                 mTimeCounter = 0;
//                 mDoDetectWall = true;
//             }
//         }
//         // 一旦ストップ(フラグ名が適切ではないが気にしないでください)
//         if (mDoDetectWall) {
//             mAngleTrace.setForward(0);
//             mAngleTrace.setTargetAngle(TARGET_ANGLE);
//             K_THETADOT = 7.5F;
//             mAngleTrace.execute();
//             if (mTimeCounter > 100) { // 100カウント間ストップ
//                 mState = SeesawDriver::ON0THSTAGE_GO;
//                 mInitState = true;
//             }
//             mTimeCounter++;
//         }
//     }
//     // ０段目(階段前)。助走距離をとったのでいざ突入
//     if (mState == SeesawDriver::ON0THSTAGE_GO) {
//         if (mInitState) {
//             mPrevMotor = mLeftMotor.getCount();
//             mDoDetectWall = false;
//             mInitState = false;
//         }
//         // 突入しようと思った直後に段差検知がtrueになってしまうことがあるのでちょっと進んでおく
//         if (! mDoDetectWall) {
//             mAngleTrace.setForward(100);
//             mAngleTrace.setTargetAngle(TARGET_ANGLE);
//             K_THETADOT = 7.5F; // Find! １段目１回で載る絶妙な値
//             mAngleTrace.execute();
//             if (mGps.getXCoordinate() < 3150.0) { // 階段側マーカ始点
//                 mDoDetectWall = true;
//             }
//         }
//         // いざ突入
//         if (mDoDetectWall) {
//             mActivator.reset(USER_GYRO_OFFSET + 5);
//             mAngleTrace.execute();
//             if (mWallDetector.detect()) {
//                 { Speaker s; s.playTone(1976, 10, 100); }
//                 mState = SeesawDriver::FIRSTRUN;
//                 mInitState = true;
//             }
//         }
//     }
    // シーソーに載ってからバタンとなるまで
    else if(mState == SeesawDriver::FIRSTRUN){
        if (mInitState) {
            mPrevMotor = mRightMotor.getCount();
            mInitState = false;
            mTimeCounter = 0;
        }
        K_THETADOT = 8.5F;
        mAngleTrace.setTargetAngle(TARGET_ANGLE);
        mAngleTrace.setForward(25);
        mAngleTrace.execute();
        // しばし急ブレーキ
        if(mTimeCounter < 100){
           mActivator.reset(USER_GYRO_OFFSET + 0);
        }
        // バタンとなりました
        //if(mRightMotor.getCount() - mPrevMotor >= 445) { // BACKRUN
        if(mGps.getXCoordinate() > 3800) { // LINERETURN
            { Speaker s; s.playTone(261, 40, 100); }
            //mState = SeesawDriver::BACKRUN;
            mState = SeesawDriver::LINERETURN;
            mInitState = true;
            mTimeCounter = 0;
        }
    }
    // ダブルクラッチのバック
    else if(mState == SeesawDriver::BACKRUN){
        // バック
        if(mTimeCounter <= 600){
            K_THETADOT = 7.5F;
            mActivator.reset(USER_GYRO_OFFSET - 5);
            mAngleTrace.setTargetAngle(TARGET_ANGLE);
            mAngleTrace.setForward(-30);
            mAngleTrace.execute();
        }
        // ちょっととまってろ
        else{
            mActivator.reset(USER_GYRO_OFFSET);
            mAngleTrace.setTargetAngle(TARGET_ANGLE);
            mAngleTrace.setForward(5); // 坂なので0でも後ろに下がってしまう
            mAngleTrace.execute();
        }
        // ある程度後ろにバックしました
        if(mRightMotor.getCount() - mPrevMotor <= 270){
            { Speaker s; s.playTone(261, 60, 100); }
            mState = SeesawDriver::SECONDRUN;
            mTimeCounter = 0;
        }
        //シーソーを超えてしまったらシングルで諦めてライン復帰
       // if(mGps.getXCoordinate() >= 3900) {
       //     { Speaker s; s.playTone(1987, 40, 100); }
       //     mState = SeesawDriver::LINERETURN;
       //     mTimeCounter = 0;
       // }
    }
    // ダブルクラッチの前進
    else if(mState == SeesawDriver::SECONDRUN){
        // ちょっととまってろ
        if(mTimeCounter <= 750){
            K_THETADOT = 7.5F;
            mAngleTrace.setTargetAngle(TARGET_ANGLE);
            mAngleTrace.setForward(10); // 坂なので0でも後ろに下がってしまう
            mAngleTrace.execute();
        }
        // 前進
        else{
            mAngleTrace.setTargetAngle(TARGET_ANGLE);
            mAngleTrace.setForward(50);
            mAngleTrace.execute();
        }
        //シーソー終了場所から300mm先で状態遷移
        if( 3636.0 + 500.0 < mGps.getXCoordinate()){//@todo 要調整
            { Speaker s; s.playTone(261, 60, 100); }
            mState = SeesawDriver::LINERETURN;
            mInitState = true;
            mTimeCounter = 0;
            K_THETADOT = 7.5F;
        }
    }
    // ライン復帰
    else if (mState == SeesawDriver::LINERETURN) {
        if (mInitState) {
            mInitState = false;
            mTimeCounter = 0;
            mDoDetectWall = false;
            mInitState = false;
            mIncrementAngle = 10.0;
        }
        // しばしまっすぐ進む
        if (! mDoDetectWall) {
            mAngleTrace.execute();
            if (mTimeCounter > 500) {
                mTimeCounter = 0;
                mDoDetectWall = true;
            }
        }
        // ライン検知(ちょっと右に向かってまっすぐ)
        if (mDoDetectWall) {
            K_THETADOT = 7.5F;
            mIncrementAngle = MAX(mIncrementAngle + 0.1, 45.0);
            mAngleTrace.setTargetAngle(TARGET_ANGLE - mIncrementAngle); // 突入角度よりちょっと右
            mAngleTrace.setForward(30);
            mAngleTrace.execute();
            //VectorT<float> command(15, -5);
            //mActivator.run(command);
            if (mLineDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                mState = SeesawDriver::AFTERLINETRACE;
                mInitState = true;
            }
        }
    }
    // ライン復帰後ライントレース
    else if (mState == SeesawDriver::AFTERLINETRACE) {
        if (mInitState) {
            K_THETADOT = 7.5F;
            mLineTrace.setForward(30);
            mTimeCounter = 0;
            mInitState = false;
        }
        mLineTrace.execute();
        if (mTimeCounter > 250) {
            mLineTrace.setForward(30);
        }
        if (mTimeCounter > 750) {
            mLineTrace.setForward(60);
        }
    }
    mTimeCounter++;
    return mState == SeesawDriver::AFTERLINETRACE; // 終了しました
}


