#include"stdafx.h"
#include"pch.h"

#include"CObjComponent.h"

namespace DACViewTest {
  TEST(TestCObjectComponent, TestInitialize) {
    CString str = "nameCpt";
    CRect rect(100, 200, 300, 400), rect2;
    CObjectComponent c(str, rect), c2;

    rect2 = c.GetSize();
    EXPECT_EQ(100, rect2.left);
    EXPECT_EQ(200, rect2.top);
    EXPECT_EQ(300, rect2.right);
    EXPECT_EQ(400, rect2.bottom);
    EXPECT_STREQ(_T("nameCpt"), c.GetName());
    EXPECT_STREQ(_T(""), c2.GetName());
    EXPECT_TRUE(c.CanViewIn());
    EXPECT_TRUE(c2.CanViewIn());
  }

  TEST(TestCObjectComponent, TestIsNeedUpdate) {
  }

  TEST(TestCObjectComponent, TestSerialize) {
  }

  TEST(TestCObjectComponent, TestGetClassNameStr) {
    CObjectComponent c;

    EXPECT_STREQ(_T("Cpt"), c.GetClassNameStr());
  }

  TEST(TestCObjectComponent, TestCanViewIn) {
    CObjectComponent c;

    EXPECT_TRUE(c.CanViewIn());
  }
  TEST(TestCObjectComponent, TestCanInSymbol) {
    CObjectComponent c;

    EXPECT_FALSE(c.CanInSymbol());
  }
}