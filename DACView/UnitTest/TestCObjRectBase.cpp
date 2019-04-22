#include"stdafx.h"
#include"pch.h"

#include"CObjRectBase.h"
#include"CObjSymbol.h"

namespace DACViewTest {
  TEST(TestCObjRectBase, TestInitialize) {
    CString strName = "abc";
    CRect rect(100, 100, 200, 200), rect2;

    CObjRectBase c(strName, rect), c1;
    EXPECT_STREQ("abc", c.GetName());
    EXPECT_EQ(RGB(0, 0, 0), c.GetBkGrdColor());
    EXPECT_EQ(RGB(255, 255, 255), c.GetForeGrdColor());
    rect2 = c.GetSize();
    EXPECT_EQ(100, rect2.left);
    EXPECT_EQ(100, rect2.top);
    EXPECT_EQ(200, rect2.bottom);
    EXPECT_EQ(200, rect2.right);
    EXPECT_FALSE(c.IsTransparent());

    EXPECT_STREQ("", c1.GetName());
    EXPECT_TRUE(c.IsRectShape());
  }

	TEST(TestCOjbRectBase, TestIsRectShape) {
		CObjRectBase c;
		EXPECT_TRUE(c.IsRectShape());
	}

  TEST(TestCObjRectBase, TestIsTransparent) {
    CObjRectBase c;

    EXPECT_FALSE(c.IsTransparent());
    c.SetTransparentFlag(true);
    EXPECT_TRUE(c.IsTransparent());
    c.SetTransparentFlag(false);
    EXPECT_FALSE(c.IsTransparent());
  }

  TEST(TestCObjRectBase, TestGetSize) {
    CObjRectBase c;
    CRect rect, rect2, rect3, rect4;

    rect = c.GetSize();
    EXPECT_TRUE(rect.IsRectEmpty());
    rect.left = 100;
    rect.right = 200;
    rect.top = 300;
    rect.bottom = 400;
    c.SetAllSize(rect);
    rect2 = c.GetSize();
    rect3 = c.GetOriginSize();
    rect4 = c.GetLastSize();
    EXPECT_EQ(rect.left, rect3.left);
    EXPECT_EQ(rect.right, rect4.right);
    EXPECT_EQ(rect.top, rect2.top);
    EXPECT_EQ(400, rect3.bottom);
    rect.left = 600;
    rect.right = 700;
    rect.top = 800;
    rect.bottom = 900;
    c.SetOriginSize(rect);
    rect3 = c.GetOriginSize();
    EXPECT_EQ(600, rect3.left);
    EXPECT_EQ(700, rect3.right);
    EXPECT_EQ(800, rect3.top);
    EXPECT_EQ(900, rect3.bottom);
    c.SetDynamicSize(rect3);
    rect2 = c.GetSize();
    EXPECT_EQ(600, rect2.left);
    EXPECT_EQ(700, rect2.right);
    EXPECT_EQ(800, rect2.top);
    EXPECT_EQ(900, rect2.bottom);

  }

  TEST(TestCObjRectBase, TestSetBkGrdColor) {
    CObjRectBase c;
    COLORREF bk = RGB(128, 128, 128);
    COLORREF fr = RGB(0, 0, 0);

    c.SetBkGrdColor(bk);
    EXPECT_EQ(bk, c.GetBkGrdColor());
    c.SetForeGrdColor(bk);
    EXPECT_EQ(bk, c.GetForeGrdColor());
  }

  TEST(TestCObjRectBase, TestSetRectPosition) {
    CObjRectBase c;
    CRect rect(100, 200, 300, 500);

    c.SetAllSize(rect);
    c.SetRectPosition(1, 100);
    c.SetRectPosition(2, 50);
    rect = c.GetSize();
    EXPECT_EQ(150, rect.left);
    EXPECT_EQ(350, rect.right);
    EXPECT_EQ(300, rect.top);
    EXPECT_EQ(600, rect.bottom);
  }

  TEST(TestCObjRectBase, TestSetBool) {
    CObjRectBase c;

    c.SetBool(0, false);
    EXPECT_FALSE(c.IsTransparent());
    c.SetBool(0, true);
    EXPECT_TRUE(c.IsTransparent());
    c.SetBool(0, 0);
    EXPECT_FALSE(c.IsTransparent());
    c.SetBool(0, 100);
    EXPECT_TRUE(c.IsTransparent());
  }

  TEST(TestCObjRectBase, TestSetInteger) {
    CObjRectBase c;
    CRect rect(100, 100, 400, 500);

    c.SetAllSize(rect);
    EXPECT_FALSE(c.IsDrawAll());
    c.SetInteger(1, -1);
    rect = c.GetSize();
    EXPECT_EQ(100, rect.top);
    c.SetInteger(1, 500);
    rect = c.GetSize();
    EXPECT_EQ(100, rect.top);
    c.SetInteger(1, 200);
    rect = c.GetSize();
    EXPECT_EQ(300, rect.top);
    EXPECT_TRUE(c.IsDrawAll());
    c.SetInteger(2, -1);
    rect = c.GetSize();
    EXPECT_EQ(400, rect.right);
    c.SetInteger(2, 500);
    rect = c.GetSize();
    EXPECT_EQ(400, rect.right);
    c.SetInteger(2, 200);
    rect = c.GetSize();
    EXPECT_EQ(300, rect.right);
  }

  TEST(TestCObjRectBase, TestSetDouble) {
    // do nothing
  }

  TEST(TestCObjRectBase, TestSetString) {
    CObjRectBase c;
    CString str = "abcde";

    c.SetString(5, str);
    EXPECT_STREQ("abcde", c.GetTitle());
  }

  TEST(TestCObjRectBase, TestSetColor) {
    CObjRectBase c;

    c.SetColor(4, RGB(125, 127, 120));
    EXPECT_EQ(RGB(125, 127, 120), c.GetForeGrdColor());
    c.SetColor(3, RGB(135, 147, 120));
    EXPECT_EQ(RGB(135, 147, 120), c.GetBkGrdColor());

  }

  TEST(TestCObjectBase, TestSetSymbolThatHaveMe) {
    CObjRectBase c;
    CObjectSymbol symbol;

    c.SetSymbolThatHaveMe(&symbol);
    EXPECT_EQ(&symbol, c.GetSymbolThatHaveMe());
  }

  TEST(TestCObjectBase, TestIsDrawAll) {
    CObjRectBase c;

    EXPECT_FALSE(c.IsDrawAll());
    c.SetDrawAll(true);
    EXPECT_TRUE(c.IsDrawAll());
    c.SetDrawAll(false);
    EXPECT_FALSE(c.IsDrawAll());
  }

  TEST(TestCObjRectBase, TestExectiveDynLink) {
    // need code
  }




}