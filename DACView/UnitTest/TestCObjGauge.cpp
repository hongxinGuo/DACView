#include"stdafx.h"
#include"pch.h"

#include"CObjGauge.h"

namespace DACViewTest {

  TEST(TestCObjectGauge, TestInitialize) {
    CString str = "abcd";
    CRect rect(100, 200, 300, 400), rect2;
    CObjectGauge c(str, rect), c2;

    EXPECT_STREQ("abcd", c.GetName());
    rect2 = c.GetSize();
    rect = c.GetSize();
    EXPECT_EQ(100, rect.left);
    EXPECT_EQ(200, rect.top);
    EXPECT_EQ(400, rect.bottom);
    EXPECT_EQ(300, rect.right);
    EXPECT_STREQ("", c2.GetName());
  }

  TEST(TestCObjectGauge, TestSerialize) {
    CObjectGauge c;
  }

  TEST(TestCObjectGauge, TestGetClassNameStr) {
    CObjectGauge c;

    EXPECT_STREQ("Gauge", c.GetClassName());
  }

  TEST(TestCObjectGauge, TestExectiveDynLink) {
    CObjectGauge c;
  }

  TEST(TestCObjectGauge, TestSetDouble) {
    CObjectGauge c;

    c.SetParameterSelected(0, true);
    c.SetDouble(0, 0.5);
    //EXPECT_DOUBLE_EQ(3.0, c.)
  }

  TEST(TestCObjectGauge, TestGetParaName) {
    CObjectGauge c;

    EXPECT_STREQ("Value", c.GetParaName(0));
    EXPECT_STREQ("BackColor", c.GetParaName(1));
    EXPECT_STREQ("ForeColor", c.GetParaName(2));
  }

  TEST(TestCObjectGauge, TestGetDynLinkType) {
    CObjectGauge c;

    EXPECT_EQ(tDOUBLE, c.GetDynLinkType(0));
    EXPECT_EQ(tWORD, c.GetDynLinkType(1));
    EXPECT_EQ(tWORD, c.GetDynLinkType(2));
  }

  TEST(TestCObjectGauge, TestSelectParameter) {
    CObjectGauge c;

    c.SelectParameter(tOUTPUT | tWORD);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tWORD);
    EXPECT_EQ(1, c.GetIndex(0));
    EXPECT_EQ(2, c.GetIndex(1));
    EXPECT_EQ(-1, c.GetIndex(2));
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