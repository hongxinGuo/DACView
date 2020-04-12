#include"stdafx.h"
#include"pch.h"

#include"CUnitInvert.h"

namespace DACViewTest {
  TEST(TestCUnitInvert, TestUnitInvertInitialize) {
    CPoint pt(100, 100);
    CUnitInvert c("CUnitInvert", pt);

    CRect rect = c.GetSize();
    EXPECT_STREQ(_T("CUnitInvert"), c.GetName());
    EXPECT_EQ(100 - 15, rect.left);
    EXPECT_EQ(100 - 15, rect.top);
    EXPECT_EQ(100 + 15, rect.right);
    EXPECT_EQ(100 + 15, rect.bottom);

    EXPECT_STREQ(_T("Input"), c.GetParaName(0));
    EXPECT_STREQ(_T("Output"), c.GetParaName(1));
    EXPECT_STREQ(_T("EAlarm"), c.GetParaName(2));
    EXPECT_STREQ(_T("Alarm"), c.GetParaName(3));
    EXPECT_STREQ(_T("AutoManual"), c.GetParaName(4));
    EXPECT_STREQ(_T("ScanRate"), c.GetParaName(5));
    EXPECT_STREQ(_T(""), c.GetParaName(6));

    EXPECT_STREQ(_T("Invert"), c.GetClassNameStr());

    CUnitInvert c2;
    rect = c2.GetSize();
    EXPECT_STREQ(_T(""), c2.GetName());
    EXPECT_EQ(0, rect.left);
    EXPECT_EQ(0, rect.top);
    EXPECT_EQ(0, rect.right);
    EXPECT_EQ(0, rect.bottom);
  }

  TEST(TestCUnitInvert, TestSerialize) {
    CUnitInvert c;
    CFile cFile1, cFile2;
    char buffer[512];
    CString strFileName = "CUnitInvert.tst";
    CUnitInvert* pc = new CUnitInvert;
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);

    cFile1.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
    CArchive ar(&cFile1, CArchive::store, 512, buffer);
    pc->SetBool(2, false);

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
    EXPECT_FALSE(pc->GetBool(2));

    delete pc;
  }

  TEST(TestCUnitInvert, TestGetParaType) {
    CUnitInvert c;
    EXPECT_EQ(c.GetParaType(0), tINPUT | tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(1), tOUTPUT | tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(2), tOUTPUT | tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(3), tINPUT | tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(4), tINPUT | tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(5), tINPUT | tOUTPUT | tWORD);
  }

  TEST(TestCUnitInvert, TestExective) {
    CUnitInvert c;
    c.SetBool(2, true); // enable alarm
    c.SetBool(0, false);
    c.Exective();
    EXPECT_TRUE(c.GetBool(1));
    EXPECT_TRUE(c.GetBool(3)); // 输出为真时报警
    c.SetBool(0, true);
    c.Exective();
    EXPECT_FALSE(c.GetBool(1));
    EXPECT_FALSE(c.GetBool(3)); // 输出为假时不报警

    c.SetInteger(5, 10000);
    EXPECT_EQ(10000, c.GetInteger(5));
  }

  TEST(TestCUnitInvert, TestGetDynLinkType) {
    CUnitInvert c;
    EXPECT_EQ(tBOOL, c.GetDynLinkType(0));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(1));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(2));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(3));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(4));
    EXPECT_EQ(tWORD, c.GetDynLinkType(5));
    EXPECT_EQ(0, c.GetDynLinkType(6));
  }

  TEST(TestCUnitInvert, TestSelectParameter) {
    CUnitInvert c;

    c.SelectParameter(tINPUT | tBOOL);
    EXPECT_EQ(0, c.GetIndex(0));
    EXPECT_EQ(3, c.GetIndex(1));
    EXPECT_EQ(4, c.GetIndex(2));
    EXPECT_EQ(-1, c.GetIndex(3));
    c.SelectParameter(tOUTPUT | tBOOL);
    EXPECT_EQ(1, c.GetIndex(0));
    EXPECT_EQ(2, c.GetIndex(1));
    EXPECT_EQ(-1, c.GetIndex(2));
    c.SelectParameter(tINPUT | tWORD);
    EXPECT_EQ(5, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tOUTPUT | tWORD);
    EXPECT_EQ(5, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tINPUT | tDOUBLE);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tOUTPUT | tDOUBLE);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tOUTPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));
  }
}