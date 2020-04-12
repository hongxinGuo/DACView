#include"stdafx.h"
#include"pch.h"

#include "CObjDynLink.h"
#include"CObjectRect.h"
#include"CUnitAdd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace DACViewTest {
  TEST(TestCObjectDynLink, TestInitialize) {
    CObjectDynLink cODL("abcde"), cODL2;

    EXPECT_STREQ(_T("abcde"), cODL.GetName());
    EXPECT_EQ(nullptr, cODL.GetUnit());
    EXPECT_EQ(100000000, cODL.GetScanRate());
    EXPECT_EQ(-1, cODL.GetObjectIndex());
    EXPECT_EQ(-1, cODL.GetUnitIndex());
    EXPECT_EQ(0, cODL.GetLinkMethod());
    EXPECT_FALSE(cODL.IsDeleteMe());
    EXPECT_STREQ(_T(""), cODL2.GetName());
    EXPECT_EQ(nullptr, cODL2.GetUnit());
    EXPECT_EQ(100000000, cODL2.GetScanRate());
    EXPECT_EQ(-1, cODL2.GetObjectIndex());
    EXPECT_EQ(-1, cODL2.GetUnitIndex());
    EXPECT_EQ(0, cODL2.GetLinkMethod());
    EXPECT_FALSE(cODL2.IsDeleteMe());
  }

  TEST(TestCObjectDynLink, TestSerilize) {
  }

  TEST(TestCObjectDynLink, TestGetAndSet) {
    CObjectDynLink cODL;
    CUnitAdd cu;
    CObjectRect co;

    cu.SetName("asdf");
    cODL.SetUnit(&cu);
    EXPECT_EQ(&cu, cODL.GetUnit());
    EXPECT_STREQ(_T("asdf"), cODL.GetUnitName());

    co.SetName("asdf");
    cODL.SetObject(&co);
    EXPECT_EQ(&co, cODL.GetObject());

    cODL.SetObjectIndex(1);
    EXPECT_EQ(1, cODL.GetObjectIndex());
    cODL.SetUnitIndex(2);
    EXPECT_EQ(2, cODL.GetUnitIndex());
    cODL.SetLinkMethod(3);
    EXPECT_EQ(3, cODL.GetLinkMethod());
    cODL.SetObjectIndex(1); // Height
    EXPECT_EQ(tWORD | tDOUBLE, cODL.GetObjectDynLinkType());
    EXPECT_EQ(tDOUBLE, cODL.GetUnitDynLinkType());

    EXPECT_FALSE(cODL.IsUnitToObject());
    cODL.SetDataFlowUnitToObject(false);
    EXPECT_FALSE(cODL.IsUnitToObject());
    cODL.SetDataFlowUnitToObject(true);
    EXPECT_TRUE(cODL.IsUnitToObject());
    EXPECT_FALSE(cODL.IsDeleteMe());
    cODL.SetDeleteMeFlag(true);
    EXPECT_TRUE(cODL.IsDeleteMe());
    cODL.SetDeleteMeFlag(false);
    EXPECT_FALSE(cODL.IsDeleteMe());
  }
  TEST(TestCObjectDynLink, TestDestIndex) {}
  TEST(TestCObjectDynLink, TestGetType) {}
  TEST(TestCObjectDynLink, TestDenLinkClass) {}
  TEST(TestCObjectDynLink, TestLinkPointList) {}
  TEST(TestCObjectDynLink, TestIsDeleteMe) {}
  TEST(TestCObjectDynLink, TestIsSetLoopDetectFlag) {}
  TEST(TestCObjectDynLink, TestSetLoopDetectFlag) {}
  TEST(TestCObjectDynLink, TestSetDleteMeFlag) {}
  TEST(TestCObjectDynLink, TestSetSrcUnit) {}
  TEST(TestCObjectDynLink, TestSetDestUnit) {}
  TEST(TestCObjectDynLink, TestSetSrcIndex) {}
  TEST(TestCObjectDynLink, TestSetDestIndex) {}
  TEST(TestCObjectDynLink, TestSetDynLinkType) {}
  TEST(TestCObjectDynLink, TestSetDynLinkClass) {}
  TEST(TestCObjectDynLink, TestSetLinkPointList) {}
}