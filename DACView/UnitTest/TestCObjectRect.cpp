#include"stdafx.h"
#include "gtest/gtest.h"

#include"CObjectRect.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(TestCObjectRect, TestGetParaName) {
	CObjectRect c;
	EXPECT_STREQ("Visibility", c.GetParaName(0));
	EXPECT_STREQ("Height", c.GetParaName(1));
	EXPECT_STREQ("Width", c.GetParaName(2));
	EXPECT_STREQ("BackColor", c.GetParaName(3));
	EXPECT_STREQ("ForeColor", c.GetParaName(4));
	EXPECT_STREQ("Title", c.GetParaName(5));
}

TEST(TestCObjectRect, TestGetDynLinkType) {
	CObjectRect c;
	EXPECT_EQ(tBOOL, c.GetDynLinkType(0));
	EXPECT_EQ(tWORD | tDOUBLE, c.GetDynLinkType(1));
	EXPECT_EQ(tWORD | tDOUBLE, c.GetDynLinkType(2));
	EXPECT_EQ(tWORD, c.GetDynLinkType(3));
	EXPECT_EQ(tWORD, c.GetDynLinkType(4));
	EXPECT_EQ(tBOOL | tWORD | tDOUBLE | tSTRING, c.GetDynLinkType(5));
}

TEST(TestCObjectRect, TestCanInSymbol) {
	CObjectRect c;
	EXPECT_TRUE(c.CanInSymbol());
}

