#include"stdafx.h"
#include"pch.h"

#include"CUnitAdd.h"

namespace DACViewTest {
  TEST(TestCUnitDictionary, TestCUnitDictionary) {
    CPoint pt(100, 100);
    CUnitAdd cunitAdd("abcde", pt);
    CUnitBasePtr punit;
    CString strName, strParaName;
    INT64 ulIndex = 0, ulType = 0;
    CUnitDictionary c(&cunitAdd, 2, tOUTPUT | tDOUBLE);

    punit = c.GetUnit();
    EXPECT_STREQ(punit->GetName(), "abcde");
    strName = c.GetUnitName();
    EXPECT_STREQ("abcde", strName);
    ulIndex = c.GetUnitIndex();
    EXPECT_EQ(2, ulIndex);
    ulType = c.GetType();
    EXPECT_EQ(tOUTPUT | tDOUBLE, ulType);
    EXPECT_STREQ("Output", c.GetParaName());
  }
}