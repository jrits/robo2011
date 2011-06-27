#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "Vector.h"

#include "MockMotor.h"
#include "MockGyroSensor.h"
#include "MockNxt.h"
#include "Activator.h"

using ::testing::AtLeast;
using ::testing::Return;

typedef float F32;

TEST(NxtTest, CanRun){
  // Mockオブジェクトの生成。
  MockMotor left_motor;// 左モータ。
  MockMotor right_motor;// 右モータ。
  MockGyroSensor gyro_sensor;// ジャイロ・センサ。
  MockNxt nxt;// Nxt本体。

  // 倒立走行プログラムへ渡すために、エンコーダ、ジャイロ・センサ、
  // 残バッテリの値が取得される。

  // モータのエンコーダ値を1回、取得する。
  EXPECT_CALL(right_motor, getCount()).Times(1).WillOnce(Return(200));
  EXPECT_CALL(left_motor, getCount()).Times(1).WillOnce(Return(100));

  // 呼び出し回数を明示的に指定しないときは、WillOnceの数から呼び出し回数が推測される。
  EXPECT_CALL(gyro_sensor,get()).WillOnce(Return(760));

  // 最低でも1度バッテリの残量を取得する。バッテリの残量は何度取得されても80。
  EXPECT_CALL(nxt,getBattMv()).Times(AtLeast(1)).WillRepeatedly(Return(80));

  // Motorに1度だけ電圧を掛ける。
  EXPECT_CALL(right_motor, setPWM(80)).Times(1);
  EXPECT_CALL(left_motor, setPWM(80)).Times(1);

  // テスト対象のオブジェクトを作成。
  Activator activator(left_motor,right_motor,gyro_sensor,nxt);

  // 実装したいメソッドの呼び出し。
  // EXPECT_TRUE(activator.run);
  activator.run(VectorT<F32>(10.0,10.0));
}

// テスト用のメイン関数
int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc,argv);
  return RUN_ALL_TESTS();
}
 
