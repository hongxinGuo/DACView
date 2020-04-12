#include"stdafx.h"
#include"pch.h"

#include"CObjFillBar.h"

namespace DACViewTest {
  TEST(TestCObjectFillBar, TestInitialize) {
    CString str = "abcd";
    CRect rect(100, 200, 300, 400), rect2;
    CObjectFillBar c(str, rect), c2;

    EXPECT_STREQ(_T("abcd"), c.GetName());
    rect2 = c.GetSize();
    rect = c.GetSize();
    EXPECT_EQ(100, rect.left);
    EXPECT_EQ(200, rect.top);
    EXPECT_EQ(400, rect.bottom);
    EXPECT_EQ(300, rect.right);
    EXPECT_STREQ(_T(""), c2.GetName());
  }

  TEST(TestCObjectFillBar, TestSerialize) {
    CObjectFillBar c;
  }

  TEST(TestCObjectFillBar, TestGetClassNameStr) {
    CObjectFillBar c;

    EXPECT_STREQ(_T("FillBar"), c.GetClassNameStr());
  }

  TEST(TestCObjectFillBar, TestExectiveDynLink) {
    CObjectFillBar c;
  }

  TEST(TestCObjectFillBar, TestSetDouble) {
    CObjectFillBar c;

    c.SetDouble(0, 0.5);
    //EXPECT_DOUBLE_EQ(3.0, c.)
  }

  TEST(TestCObjectFillBar, TestGetParaName) {
    CObjectFillBar c;

    EXPECT_STREQ(_T("Bar"), c.GetParaName(0));
  }

  TEST(TestCObjectFillBar, TestGetDynLinkType) {
    CObjectFillBar c;

    EXPECT_EQ(tWORD | tDOUBLE, c.GetDynLinkType(0));
  }

  TEST(TestCObjectFillBar, TestSelectParameter) {
    CObjectFillBar c;

    c.SelectParameter(tOUTPUT | tWORD);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tWORD);
    EXPECT_EQ(0, c.GetIndex(0));
    c.SelectParameter(tINPUT | tBOOL);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tOUTPUT | tBOOL);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tDOUBLE);
    EXPECT_EQ(0, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tOUTPUT | tDOUBLE);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tOUTPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));
  }
}