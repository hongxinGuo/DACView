#pragma once

#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

using namespace testing;

using namespace std;
#include<memory>

namespace StockAnalysisTest {
  class TestEnvironment : public::testing::Environment {  // 全局初始化，由main()函数调用。
  public:
    TestEnvironment(void) {
    }
    virtual ~TestEnvironment() {
    }

    virtual void SetUp(void) override {
      // 下列全局智能指针为实际类
    }

    virtual void TearDown(void) override {
      // 这里要故意将这几个Mock变量设置为nullptr，这样就能够在测试输出窗口（不是Test Expxplorer窗口）中得到测试结果。
    }
  };
}