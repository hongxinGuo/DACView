#include"stdafx.h"
#include"pch.h"

#include"CUnitPulse.h"

namespace DACViewTest {

  TEST(TestCUnitPulse, TestInitialize) {
    CPoint pt(100, 100);
    CUnitPulse c("CUnitPulse", pt);

    CRect rect = c.GetSize();
    EXPECT_STREQ("CUnitPulse", c.GetName());
    EXPECT_EQ(100 - 15, rect.left);
    EXPECT_EQ(100 - 15, rect.top);
    EXPECT_EQ(100 + 15, rect.right);
    EXPECT_EQ(100 + 15, rect.bottom);

    EXPECT_STREQ("Input", c.GetParaName(0));
    EXPECT_STREQ("Output", c.GetParaName(1));
    EXPECT_STREQ("AutoManual", c.GetParaName(2));
    EXPECT_STREQ("Width", c.GetParaName(3));
    EXPECT_STREQ("ScanRate", c.GetParaName(4));
    EXPECT_STREQ("", c.GetParaName(5));

    EXPECT_STREQ("Pulse", c.GetClassNameStr());

    CUnitPulse c2;
    rect = c2.GetSize();
    EXPECT_STREQ("", c2.GetName());
    EXPECT_EQ(0, rect.left);
    EXPECT_EQ(0, rect.top);
    EXPECT_EQ(0, rect.right);
    EXPECT_EQ(0, rect.bottom);
  }

  TEST(TestCUnitPulse, TestSerialize) {
    CUnitPulse c;
    CFile cFile1, cFile2;
    char buffer[512];
    CString strFileName = "CUnitPulse.tst";
    CUnitPulse * pc = new CUnitPulse;
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);

    cFile1.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
    CArchive ar(&cFile1, CArchive::store, 512, buffer);
    pc->SetInteger(3, 1000);

    ar << pc; // 
    ar.Flush(); // ����flush�������п���û���д洢
    cFile1.Close();
    delete pc;

    if (!cFile2.Open(strFileName, CFile::modeRead)) {
      c.SetName("aa");
    }
    CArchive ar2(&cFile2, CArchive::load, 512, buffer);
    ar2 >> pc;
    EXPECT_EQ(1000, pc->GetInteger(3));

    delete pc;
  }

  TEST(TestCUnitPulse, TestGetParaType) {
    CUnitPulse c;
    EXPECT_EQ(c.GetParaType(0), tINPUT |tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(1), tOUTPUT |tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(2), tINPUT |tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(3), tINPUT |tMODIFIABLE | tWORD);
    EXPECT_EQ(c.GetParaType(4), tINPUT | tOUTPUT | tWORD);
  }

  TEST(TestCUnitPulse, TestExective) {
    CUnitPulse c;

  }

  TEST(TestCUnitPulse, TestGetDynLinkType) {
    CUnitPulse c;
    EXPECT_EQ(tBOOL, c.GetDynLinkType(0));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(1));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(2));
    EXPECT_EQ(tWORD, c.GetDynLinkType(3));
    EXPECT_EQ(tWORD, c.GetDynLinkType(4));
    EXPECT_EQ(0, c.GetDynLinkType(5));
  }


  TEST(TestCUnitPulse, TestSelectParameter) {
    CUnitPulse c;

    c.SelectParameter(tINPUT | tBOOL);
    EXPECT_EQ(0, c.GetIndex(0));
    EXPECT_EQ(2, c.GetIndex(1));
    EXPECT_EQ(-1, c.GetIndex(2));
    c.SelectParameter(tOUTPUT | tBOOL);
    EXPECT_EQ(1, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tOUTPUT | tWORD);
    EXPECT_EQ(4, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tINPUT | tWORD);
    EXPECT_EQ(3, c.GetIndex(0));
    EXPECT_EQ(4, c.GetIndex(1));
    EXPECT_EQ(-1, c.GetIndex(2));
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