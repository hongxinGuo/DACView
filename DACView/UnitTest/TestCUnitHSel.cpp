#include"stdafx.h"
#include"pch.h"

#include"CUnitHSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace DACViewTest {
  TEST(TestCUnitHighSelect, TestCUnitHighSelectInitialize) {
    CPoint pt(100, 100);
    CUnitHighSelect c("thisHSel", pt);
    EXPECT_STREQ(_T("thisHSel"), c.GetName());

    CRect rect = c.GetSize();
    EXPECT_EQ(100 - 15, rect.left);
    EXPECT_EQ(100 - 15, rect.top);
    EXPECT_EQ(100 + 15, rect.right);
    EXPECT_EQ(100 + 15, rect.bottom);

    EXPECT_STREQ(_T("HighSelect"), c.GetClassNameStr());

    CUnitHighSelect c2;
    rect = c2.GetSize();
    EXPECT_STREQ(_T(""), c2.GetName());
    EXPECT_EQ(0, rect.left);
    EXPECT_EQ(0, rect.top);
    EXPECT_EQ(0, rect.right);
    EXPECT_EQ(0, rect.bottom);
  }

  TEST(TestCUnitHighSelect, TestSerialize) {
    CUnitHighSelect c;
    CFile cFile1, cFile2;
    char buffer[512];
    CString strFileName = "CUnitHighSelect.tst";
    CUnitHighSelect* pc = new CUnitHighSelect;
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);

    cFile1.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
    CArchive ar(&cFile1, CArchive::store, 512, buffer);
    for (int i = 4; i < 8; i++) {
      pc->SetDouble(i, i);
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
    EXPECT_DOUBLE_EQ(4, pc->GetDouble(4));
    EXPECT_DOUBLE_EQ(5, pc->GetDouble(5));
    EXPECT_DOUBLE_EQ(6, pc->GetDouble(6));
    EXPECT_DOUBLE_EQ(7, pc->GetDouble(7));
    delete pc;
  }
  TEST(TestCUnitHighSelect, TestGetClassname) {
    CUnitHighSelect c;
    EXPECT_STREQ(_T("HighSelect"), c.GetClassNameStr());
  }

  TEST(TestCUnitHighSelect, TestGetParaName) {
    CUnitHighSelect c;
    EXPECT_STREQ(_T("Input1"), c.GetParaName(0));
    EXPECT_STREQ(_T("Input2"), c.GetParaName(1));
    EXPECT_STREQ(_T("Input3"), c.GetParaName(2));
    EXPECT_STREQ(_T("Output"), c.GetParaName(3));
    EXPECT_STREQ(_T("HiRange"), c.GetParaName(4));
    EXPECT_STREQ(_T("LowRange"), c.GetParaName(5));
    EXPECT_STREQ(_T("HiLimit"), c.GetParaName(6));
    EXPECT_STREQ(_T("LowLimit"), c.GetParaName(7));
    EXPECT_STREQ(_T("AutoManual"), c.GetParaName(8));
    EXPECT_STREQ(_T("ScanRate"), c.GetParaName(9));
    EXPECT_STREQ(_T(""), c.GetParaName(10));
  }

  TEST(TestCUnitHighSelect, TestGetParaType) {
    CUnitHighSelect c;
    EXPECT_EQ(tINPUT | tMODIFIABLE | tDOUBLE, c.GetParaType(0));
    EXPECT_EQ(tINPUT | tMODIFIABLE | tDOUBLE, c.GetParaType(1));
    EXPECT_EQ(tINPUT | tMODIFIABLE | tDOUBLE, c.GetParaType(2));
    EXPECT_EQ(tOUTPUT | tMODIFIABLE | tDOUBLE, c.GetParaType(3));
    EXPECT_EQ(tINPUT | tOUTPUT | tDOUBLE, c.GetParaType(4));
    EXPECT_EQ(tINPUT | tOUTPUT | tDOUBLE, c.GetParaType(5));
    EXPECT_EQ(tINPUT | tOUTPUT | tDOUBLE, c.GetParaType(6));
    EXPECT_EQ(tINPUT | tOUTPUT | tDOUBLE, c.GetParaType(7));
    EXPECT_EQ(tINPUT | tMODIFIABLE | tBOOL, c.GetParaType(8));
    EXPECT_EQ(tINPUT | tOUTPUT | tWORD, c.GetParaType(9));
    EXPECT_EQ(0, c.GetParaType(10));
  }

  TEST(TestCUnitHighSelect, TestGetBool) {
    CUnitHighSelect c;
    c.SetBool(8, true);
    EXPECT_TRUE(c.GetBool(8));
    c.SetBool(8, false);
    EXPECT_FALSE(c.GetBool(8));
  }

  TEST(TestCUnitHighSelect, TestGetInteger) {
    CUnitHighSelect c;
    EXPECT_TRUE(c.SetInteger(9, 1900));
    EXPECT_EQ(1900, c.GetInteger(9));
  }

  TEST(TestCUnitHighSelect, TestGetDouble) {
    CUnitHighSelect c;
    for (int i = 0; i < 8; i++) {
      EXPECT_TRUE(c.SetDouble(i, i * 10.5));
    }
    for (int j = 0; j < 8; j++) {
      EXPECT_DOUBLE_EQ(j * 10.5, c.GetDouble(j));
    }
  }

  TEST(TestCUnitHighSelect, TestGetDynLinkType) {
    CUnitHighSelect c;
    for (int i = 0; i < 8; i++) {
      EXPECT_EQ(tDOUBLE, c.GetDynLinkType(i) & tDOUBLE);
    }
    EXPECT_EQ(tBOOL, c.GetDynLinkType(8) & tBOOL);
    EXPECT_EQ(tWORD, c.GetDynLinkType(9) & tWORD);
    EXPECT_EQ(0, c.GetDynLinkType(10) & tSTRING);
  }

  TEST(TestCUnitHighSelect, TestExective) {
    CUnitHighSelect c;
    c.SetDouble(0, 2.0); // input1
    c.SetDouble(1, 5.0); // input2
    c.SetDouble(2, 7.0); // input3
    c.SetDouble(7, 8.0); // LoLimit
    c.Exective();
    EXPECT_DOUBLE_EQ(7.0, c.GetDouble(3));
    EXPECT_TRUE(c.IsOverFlow());
    c.SetDouble(7, 0);		// LoLimit
    c.SetDouble(6, 19.0); // HiLimit
    c.SetDouble(2, 21.0); // input3
    c.Exective();
    EXPECT_DOUBLE_EQ(21.0, c.GetDouble(3));
    EXPECT_TRUE(c.IsOverFlow());
  }

  TEST(TestCUnitHighSelect, TestSelectParameter) {
    CUnitHighSelect c;

    c.SelectParameter(tINPUT | tDOUBLE);
    EXPECT_EQ(0, c.GetIndex(0));
    EXPECT_EQ(1, c.GetIndex(1));
    EXPECT_EQ(2, c.GetIndex(2));
    EXPECT_EQ(4, c.GetIndex(3));
    EXPECT_EQ(5, c.GetIndex(4));
    EXPECT_EQ(6, c.GetIndex(5));
    EXPECT_EQ(7, c.GetIndex(6));
    EXPECT_EQ(-1, c.GetIndex(7));
    c.SelectParameter(tOUTPUT | tDOUBLE);
    EXPECT_EQ(3, c.GetIndex(0));
    EXPECT_EQ(4, c.GetIndex(1));
    EXPECT_EQ(5, c.GetIndex(2));
    EXPECT_EQ(6, c.GetIndex(3));
    EXPECT_EQ(7, c.GetIndex(4));
    EXPECT_EQ(-1, c.GetIndex(5));
    c.SelectParameter(tINPUT | tBOOL);
    EXPECT_EQ(8, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tINPUT | tWORD);
    EXPECT_EQ(9, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tOUTPUT | tWORD);
    EXPECT_EQ(9, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tOUTPUT | tBOOL);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tOUTPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));
  }
}