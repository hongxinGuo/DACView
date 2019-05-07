#include"stdafx.h"
#include"pch.h"

#include"CUnitDelay.h"

namespace DACViewTest {

  TEST(TestCUnitDelay, TestInitialize) {
    CPoint pt(100, 100);
    CUnitDelay c("CUnitDelay", pt);

    CRect rect = c.GetSize();
    EXPECT_STREQ("CUnitDelay", c.GetName());
    EXPECT_EQ(100 - 15, rect.left);
    EXPECT_EQ(100 - 15, rect.top);
    EXPECT_EQ(100 + 15, rect.right);
    EXPECT_EQ(100 + 15, rect.bottom);

    EXPECT_STREQ("Input", c.GetParaName(0));
    EXPECT_STREQ("Output", c.GetParaName(1));
    EXPECT_STREQ("AutoManual", c.GetParaName(2));
    EXPECT_STREQ("ScanRate", c.GetParaName(3));
    EXPECT_STREQ("", c.GetParaName(4));

    EXPECT_STREQ("UnitDelay", c.GetClassNameStr());

    CUnitDelay c2;
    rect = c2.GetSize();
    EXPECT_STREQ("", c2.GetName());
    EXPECT_EQ(0, rect.left);
    EXPECT_EQ(0, rect.top);
    EXPECT_EQ(0, rect.right);
    EXPECT_EQ(0, rect.bottom);
  }

  TEST(TestCUnitDelay, TestSerialize) {
    CUnitDelay c;
    CFile cFile1, cFile2;
    char buffer[512];
    CString strFileName = "CUnitDelay.tst";
    CUnitDelay * pc = new CUnitDelay;
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);

    cFile1.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
    CArchive ar(&cFile1, CArchive::store, 512, buffer);
    ar << pc; // 
    ar.Flush(); // 必须flush，否则有可能没进行存储
    cFile1.Close();
    delete pc;

    if (!cFile2.Open(strFileName, CFile::modeRead)) {
      c.SetName("aa");
    }
    CArchive ar2(&cFile2, CArchive::load, 512, buffer);
    ar2 >> pc;
    rect = pc->GetSize();
    delete pc;
  }

  TEST(TestCUnitDelay, TestGetParaType) {
    CUnitDelay c;
    EXPECT_EQ(c.GetParaType(0), tINPUT |tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(1), tOUTPUT |tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(2), tINPUT |tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(3), tINPUT | tOUTPUT | tWORD);

  }

  TEST(TestCUnitDelay, TestExective) {
    CUnitDelay c;


    c.SetInteger(3, 10000);
    EXPECT_EQ(10000, c.GetInteger(3));
  }

  TEST(TestCUnitDelay, TestGetDynLinkType) {
    CUnitDelay c;
    EXPECT_EQ(tBOOL, c.GetDynLinkType(0));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(1));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(2));
    EXPECT_EQ(tWORD, c.GetDynLinkType(3));
  }

  TEST(TestCUnitDelay, TestSelectParameter) {
    CUnitDelay c;

    c.SelectParameter(tINPUT | tBOOL);
    EXPECT_EQ(0, c.GetIndex(0));
    EXPECT_EQ(2, c.GetIndex(1));
    EXPECT_EQ(-1, c.GetIndex(2));
    c.SelectParameter(tOUTPUT | tBOOL);
    EXPECT_EQ(1, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tOUTPUT | tWORD);
    EXPECT_EQ(3, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tINPUT | tWORD);
    EXPECT_EQ(3, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tINPUT | tDOUBLE);
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tOUTPUT | tDOUBLE);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tOUTPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));

  }


}