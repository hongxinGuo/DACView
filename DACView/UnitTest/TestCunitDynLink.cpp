#include"stdafx.h"
#include"pch.h"

#include "CUnitDynLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace DACViewTest {
  TEST(TestCUnitDynLink, TestInitialize) {
    CUnitDynLink c;
    EXPECT_STREQ(_T(""), c.GetName());
    EXPECT_EQ(100000000, c.GetScanRate());
    EXPECT_EQ(0, c.GetDynLinkClass());
    EXPECT_EQ(nullptr, c.GetDestUnit());
    EXPECT_EQ(nullptr, c.GetSrcUnit());
    EXPECT_EQ(-1, c.GetDestIndex());
    EXPECT_EQ(-1, c.GetSrcIndex());
    EXPECT_FALSE(c.IsDeleteMe());
    EXPECT_EQ(nullptr, c.mTest_pDestUnitSaved);
  }

  TEST(TestCUnitDynLink, TestSerilize) {
  }

  TEST(TestCUnitDynLink, TestGetSrcUnit) {
  }
  TEST(TestCUnitDynLink, TestDestUnit) {}
  TEST(TestCUnitDynLink, TestSrcIndex) {}
  TEST(TestCUnitDynLink, TestDestIndex) {}
  TEST(TestCUnitDynLink, TestGetType) {}
  TEST(TestCUnitDynLink, TestDenLinkClass) {}
  TEST(TestCUnitDynLink, TestLinkPointList) {}
  TEST(TestCUnitDynLink, TestIsDeleteMe) {}
  TEST(TestCUnitDynLink, TestIsSetLoopDetectFlag) {}
  TEST(TestCUnitDynLink, TestSetLoopDetectFlag) {}
  TEST(TestCUnitDynLink, TestSetDleteMeFlag) {}
  TEST(TestCUnitDynLink, TestSetSrcUnit) {}
  TEST(TestCUnitDynLink, TestSetDestUnit) {}
  TEST(TestCUnitDynLink, TestSetSrcIndex) {}
  TEST(TestCUnitDynLink, TestSetDestIndex) {}
  TEST(TestCUnitDynLink, TestSetDynLinkType) {}
  TEST(TestCUnitDynLink, TestSetDynLinkClass) {}
  TEST(TestCUnitDynLink, TestSetLinkPointList) {}
}