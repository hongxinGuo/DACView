#pragma once

#include"stdafx.h"
#include"pch.h"

#include"globedef.h"

using namespace testing;

using namespace std;
#include<memory>

namespace StockAnalysisTest {
  class TestEnvironment : public::testing::Environment {  // ȫ�ֳ�ʼ������main()�������á�
  public:
    TestEnvironment(void) {
    }
    virtual ~TestEnvironment() {
    }

    virtual void SetUp(void) override {
      // ����ȫ������ָ��Ϊʵ����
    }

    virtual void TearDown(void) override {
      // ����Ҫ���⽫�⼸��Mock��������Ϊnullptr���������ܹ��ڲ���������ڣ�����Test Expxplorer���ڣ��еõ����Խ����
    }
  };
}