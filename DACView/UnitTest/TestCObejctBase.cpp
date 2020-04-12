#include"stdafx.h"
#include"pch.h"

#include"CObjectBase.h"
#include"CObjSymbol.h"

namespace DACViewTest {
  TEST(TestCObjectBase, TestInitialize) {
    CString strName = "abc";

    CObjectBase c(strName), c1;
    EXPECT_STREQ(_T("abc"), c.GetName());
    EXPECT_EQ(RGB(0, 0, 0), c.GetBkGrdColor());
    EXPECT_EQ(RGB(255, 255, 255), c.GetForeGrdColor());
    EXPECT_FALSE(c.IsTransparent());

    EXPECT_STREQ(_T(""), c1.GetName());
  }

  TEST(TestCObjectBase, TestCanInSymbol) {
    CObjectBase c;
    EXPECT_TRUE(c.CanInSymbol());
  }

  TEST(TestCObjectBase, TestIsRectShape) {
    CObjectBase c;
    //EXPECT_FALSE(c.IsRectShape());
  }

  TEST(TestCObjectBase, TestIsTransparent) {
    CObjectBase c;

    EXPECT_FALSE(c.IsTransparent());
    c.SetTransparentFlag(true);
    EXPECT_TRUE(c.IsTransparent());
    c.SetTransparentFlag(false);
    EXPECT_FALSE(c.IsTransparent());
  }

  TEST(TestCObjectBase, TestGetSize) {
    CObjectBase c;
    CRect rect;

    //rect = c.GetSize();
    //EXPECT_TRUE(rect.IsRectEmpty());
  }

  TEST(TestCUnitBase, TestSetSymbolThatHaveMe) {
    CObjectBase c;
    CObjectSymbol symbol;

    c.SetSymbolThatHaveMe(&symbol);
    EXPECT_EQ(&symbol, c.GetSymbolThatHaveMe());
  }

  TEST(TestCUnitBase, TestIsDrawAll) {
    CObjectBase c;

    EXPECT_FALSE(c.IsDrawAll());
    c.SetDrawAll(true);
    EXPECT_TRUE(c.IsDrawAll());
    c.SetDrawAll(false);
    EXPECT_FALSE(c.IsDrawAll());
  }
}