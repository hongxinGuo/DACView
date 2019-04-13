#include"stdafx.h"
#include "gtest/gtest.h"

#include"SQIFileDoc.h"

namespace DACViewTest {
  TEST(TestCSQIFileDoc, InitializeTest) {
    //CSQIFileDoc c;
    // SQIFileDoc类无法直接调用，是被windows系统函数调用的，故而如何测试，需要研究。
    // 一种方法是将具体执行过程提出来，做为全局函数存在，这样就能测试了。
  }


}