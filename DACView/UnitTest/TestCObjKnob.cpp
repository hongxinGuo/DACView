#include"stdafx.h"
#include"pch.h"

#include"CObjKnob.h"

namespace DACViewTest {

  TEST(TestCObjectKnob, TestInitialize) {
    CString str = "abcd";
    CRect rect(100, 200, 300, 400), rect2;
    CObjectKnob c(str, rect), c2;

    EXPECT_STREQ("abcd", c.GetName());
    rect2 = c.GetSize();
    rect = c.GetSize();
    EXPECT_EQ(100, rect.left);
    EXPECT_EQ(200, rect.top);
    EXPECT_EQ(400, rect.bottom);
    EXPECT_EQ(300, rect.right);
    EXPECT_STREQ("", c2.GetName());
  }

  TEST(TestCObjectKnob, TestSerialize) {
    CObjectKnob c;
  }

  TEST(TestCObjectKnob, TestGetClassNameStr) {
    CObjectKnob c;

    EXPECT_STREQ("Knob", c.GetClassName());
  }

  TEST(TestCObjectKnob, TestExectiveDynLink) {
    CObjectKnob c;
  }


  TEST(TestCObjectKnob, TestGetParaName) {
    CObjectKnob c;

    EXPECT_STREQ("Value", c.GetParaName(0));
  }

  TEST(TestCObjectKnob, TestGetDynLinkType) {
    CObjectKnob c;

    EXPECT_EQ(tDOUBLE, c.GetDynLinkType(0));
  }

  TEST(TestCObjectKnob, TestSelectParameter) {
    CObjectKnob c;

    c.SelectParameter(tOUTPUT | tWORD);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tWORD);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tBOOL);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tOUTPUT | tBOOL);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tDOUBLE);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tOUTPUT | tDOUBLE);
    EXPECT_EQ(0, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tINPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tOUTPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));
  }


}