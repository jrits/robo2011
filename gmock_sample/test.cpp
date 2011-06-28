#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "MockTurtle.h"
#include "Painter.h"

using ::testing::AtLeast;

TEST(PainterTest, CanDrawSomething){
  MockTurtle turtle;// Mockオブジェクトの生成。

  EXPECT_CALL(turtle, PenDown())
      .Times(AtLeast(1));//PenDown()メソッドが最低でも1度呼ばれるはず。

  Painter painter(&turtle);

  EXPECT_TRUE(painter.DrawCircle(0,0,10));// DrawCircleメソッドの呼び出し。
}

// テスト用のメイン関数
int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc,argv);
  return RUN_ALL_TESTS();
}
 
