#include"stdafx.h"
#include"pch.h"

#include"CUnitDivide.h"

namespace DACViewTest {
  TEST(TestUnitDivide, TestInitialize) {
    CPoint pt(100, 100);
    CUnitDivide c("CUnitDiv", pt);

    CRect rect = c.GetSize();
    EXPECT_STREQ(_T("CUnitDiv"), c.GetName());
    EXPECT_EQ(100 - 15, rect.left);
    EXPECT_EQ(100 - 15, rect.top);
    EXPECT_EQ(100 + 15, rect.right);
    EXPECT_EQ(100 + 15, rect.bottom);

    EXPECT_STREQ(_T("Input1"), c.GetParaName(0));
    EXPECT_STREQ(_T("Input2"), c.GetParaName(1));
    EXPECT_STREQ(_T("Output"), c.GetParaName(2));
    EXPECT_STREQ(_T("K1"), c.GetParaName(3));
    EXPECT_STREQ(_T("K2"), c.GetParaName(4));
    EXPECT_STREQ(_T("HiRange"), c.GetParaName(5));
    EXPECT_STREQ(_T("LowRange"), c.GetParaName(6));
    EXPECT_STREQ(_T("HiLimit"), c.GetParaName(7));
    EXPECT_STREQ(_T("LowLimit"), c.GetParaName(8));
    EXPECT_STREQ(_T("AutoManual"), c.GetParaName(9));
    EXPECT_STREQ(_T("ScanRate"), c.GetParaName(10));
    EXPECT_STREQ(_T(""), c.GetParaName(11));

    EXPECT_STREQ(_T("Divide"), c.GetClassNameStr());

    CUnitDivide c2;
    rect = c2.GetSize();
    EXPECT_STREQ(_T(""), c2.GetName());
    EXPECT_EQ(0, rect.left);
    EXPECT_EQ(0, rect.top);
    EXPECT_EQ(0, rect.right);
    EXPECT_EQ(0, rect.bottom);
  }

  TEST(TestUnitDivide, TestSerialize) {
    CUnitDivide c;
    CFile cFile1, cFile2;
    char buffer[512];
    CString strFileName = "CUnitDiv.tst";
    CUnitDivide* pc = new CUnitDivide;
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);

    cFile1.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
    CArchive ar(&cFile1, CArchive::store, 512, buffer);
    for (int i = 3; i < 8; i++) {
      pc->SetDouble(i, i * 2.0);
    }
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
    for (int i = 3; i < 8; i++) {
      EXPECT_DOUBLE_EQ(i * 2.0, pc->GetDouble(i));
    }
    delete pc;
  }

  TEST(TestUnitDivide, TestGetParaType) {
    CUnitDivide c;
    EXPECT_EQ(c.GetParaType(0), tINPUT | tMODIFIABLE | tDOUBLE);
    EXPECT_EQ(c.GetParaType(1), tINPUT | tMODIFIABLE | tDOUBLE);
    EXPECT_EQ(c.GetParaType(2), tOUTPUT | tMODIFIABLE | tDOUBLE);
    EXPECT_EQ(c.GetParaType(3), tINPUT | tOUTPUT | tDOUBLE);
    EXPECT_EQ(c.GetParaType(4), tINPUT | tOUTPUT | tDOUBLE);
    EXPECT_EQ(c.GetParaType(5), tINPUT | tOUTPUT | tDOUBLE);
    EXPECT_EQ(c.GetParaType(6), tINPUT | tOUTPUT | tDOUBLE);
    EXPECT_EQ(c.GetParaType(7), tINPUT | tOUTPUT | tDOUBLE);
    EXPECT_EQ(c.GetParaType(8), tINPUT | tOUTPUT | tDOUBLE);
    EXPECT_EQ(c.GetParaType(9), tINPUT | tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(10), tINPUT | tOUTPUT | tWORD);
  }

  TEST(TestUnitDivide, TestExective) {
    CUnitDivide c;
    c.SetDouble(0, 20.0); // input1
    c.SetDouble(1, 5.0); // input2
    c.SetDouble(3, 1.0); // K1
    c.SetDouble(4, 1.0); // K2
    c.SetDouble(8, 0);		// Lolimit
    c.SetDouble(7, 3.0); // HiLimit
    c.Exective();
    EXPECT_DOUBLE_EQ(4.0, c.GetDouble(2));
    EXPECT_TRUE(c.IsOverFlow());
    c.SetDouble(7, 5.0); // HiLimit
    c.Exective();
    EXPECT_DOUBLE_EQ(4, c.GetDouble(2));
    EXPECT_FALSE(c.IsOverFlow());
    c.SetDouble(1, 0.0); // input2
    c.Exective();
    EXPECT_TRUE(c.IsOverFlow());
    c.SetDouble(1, 2.0);
    c.SetDouble(7, 20.0); // HiLimit
    c.SetDouble(8, 11.0); // LowLimit
    c.Exective();
    EXPECT_DOUBLE_EQ(10.0, c.GetDouble(2));
    EXPECT_TRUE(c.IsOverFlow());

    c.SetInteger(10, 10000);
    EXPECT_EQ(10000, c.GetInteger(10));
  }

  TEST(TestUnitDivide, TestGetDynLinkType) {
    CUnitDivide c;
    EXPECT_EQ(tDOUBLE, c.GetDynLinkType(0));
    EXPECT_EQ(tDOUBLE, c.GetDynLinkType(1));
    EXPECT_EQ(tDOUBLE, c.GetDynLinkType(2));
    EXPECT_EQ(tDOUBLE, c.GetDynLinkType(3));
    EXPECT_EQ(tDOUBLE, c.GetDynLinkType(4));
    EXPECT_EQ(tDOUBLE, c.GetDynLinkType(5));
    EXPECT_EQ(tDOUBLE, c.GetDynLinkType(6));
    EXPECT_EQ(tDOUBLE, c.GetDynLinkType(7));
    EXPECT_EQ(tDOUBLE, c.GetDynLinkType(8));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(9));
    EXPECT_EQ(tWORD, c.GetDynLinkType(10));
  }

  TEST(TestCUnitDivide, TestSelectParameter) {
    CUnitDivide c;

    c.SelectParameter(tINPUT | tDOUBLE);
    EXPECT_EQ(0, c.GetIndex(0));
    EXPECT_EQ(1, c.GetIndex(1));
    EXPECT_EQ(3, c.GetIndex(2));
    EXPECT_EQ(4, c.GetIndex(3));
    EXPECT_EQ(5, c.GetIndex(4));
    EXPECT_EQ(6, c.GetIndex(5));
    EXPECT_EQ(7, c.GetIndex(6));
    EXPECT_EQ(8, c.GetIndex(7));
    EXPECT_EQ(-1, c.GetIndex(8));
    c.SelectParameter(tOUTPUT | tDOUBLE);
    EXPECT_EQ(2, c.GetIndex(0));
    EXPECT_EQ(3, c.GetIndex(1));
    EXPECT_EQ(4, c.GetIndex(2));
    EXPECT_EQ(5, c.GetIndex(3));
    EXPECT_EQ(6, c.GetIndex(4));
    EXPECT_EQ(7, c.GetIndex(5));
    EXPECT_EQ(8, c.GetIndex(6));
    EXPECT_EQ(-1, c.GetIndex(7));
    c.SelectParameter(tINPUT | tBOOL);
    EXPECT_EQ(9, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tINPUT | tWORD);
    EXPECT_EQ(10, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tOUTPUT | tWORD);
    EXPECT_EQ(10, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tOUTPUT | tBOOL);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tOUTPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));
  }
}