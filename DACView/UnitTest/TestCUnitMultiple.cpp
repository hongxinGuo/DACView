#include"stdafx.h"
#include"pch.h"

#include"CUnitMultiple.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace DACViewTest {
  TEST(TestCUnitMultiple, TestCUnitMultipleInitialize) {
    CPoint pt(100, 100);
    CUnitMultiple c("thisMutiple", pt);
    EXPECT_STREQ(_T("thisMutiple"), c.GetName());
  }

  TEST(TestCUnitMultiple, TestSerialize) {
    CUnitMultiple c;
    CFile cFile1, cFile2;
    char buffer[512];
    CString strFileName = "CUnitMultiple.tst";
    CUnitMultiple* pc = new CUnitMultiple;
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);

    cFile1.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
    CArchive ar(&cFile1, CArchive::store, 512, buffer);
    for (int i = 3; i < 9; i++) {
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
    EXPECT_DOUBLE_EQ(3, pc->GetDouble(3));
    EXPECT_DOUBLE_EQ(4, pc->GetDouble(4));
    EXPECT_DOUBLE_EQ(5, pc->GetDouble(5));
    EXPECT_DOUBLE_EQ(6, pc->GetDouble(6));
    EXPECT_DOUBLE_EQ(7, pc->GetDouble(7));
    EXPECT_DOUBLE_EQ(8, pc->GetDouble(8));
    delete pc;
  }
  TEST(TestCUnitMultiple, TestGetClassname) {
    CUnitMultiple c;
    EXPECT_STREQ(_T("Multiple"), c.GetClassNameStr());
  }

  TEST(TestCUnitMultiple, TestGetParaName) {
    CUnitMultiple c;
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
  }

  TEST(TestCUnitMultiple, TestGetParaType) {
    CUnitMultiple c;
    EXPECT_EQ(tINPUT | tMODIFIABLE | tDOUBLE, c.GetParaType(0));
    EXPECT_EQ(tINPUT | tMODIFIABLE | tDOUBLE, c.GetParaType(1));
    EXPECT_EQ(tOUTPUT | tMODIFIABLE | tDOUBLE, c.GetParaType(2));
    EXPECT_EQ(tINPUT | tOUTPUT | tDOUBLE, c.GetParaType(3));
    EXPECT_EQ(tINPUT | tOUTPUT | tDOUBLE, c.GetParaType(4));
    EXPECT_EQ(tINPUT | tOUTPUT | tDOUBLE, c.GetParaType(5));
    EXPECT_EQ(tINPUT | tOUTPUT | tDOUBLE, c.GetParaType(6));
    EXPECT_EQ(tINPUT | tOUTPUT | tDOUBLE, c.GetParaType(7));
    EXPECT_EQ(tINPUT | tOUTPUT | tDOUBLE, c.GetParaType(8));
    EXPECT_EQ(tINPUT | tMODIFIABLE | tBOOL, c.GetParaType(9));
    EXPECT_EQ(tINPUT | tOUTPUT | tWORD, c.GetParaType(10));
    EXPECT_EQ(0, c.GetParaType(11));
  }

  TEST(TestCUnitMultiple, TestGetBool) {
    CUnitMultiple c;
    c.SetBool(9, true);
    EXPECT_TRUE(c.GetBool(9));
    c.SetBool(9, false);
    EXPECT_FALSE(c.GetBool(9));
  }

  TEST(TestCUnitMultiple, TestGetInteger) {
    CUnitMultiple c;
    EXPECT_TRUE(c.SetInteger(10, 1000));
    EXPECT_EQ(1000, c.GetInteger(10));
  }

  TEST(TestCUnitMultiple, TestGetDouble) {
    CUnitMultiple c;
    for (int i = 0; i < 9; i++) {
      EXPECT_TRUE(c.SetDouble(i, i * 10.5));
    }
    for (int j = 0; j < 9; j++) {
      EXPECT_DOUBLE_EQ(j * 10.5, c.GetDouble(j));
    }
  }

  TEST(TestCUnitMultiple, TestGetDynLinkType) {
    CUnitMultiple c;
    for (int i = 0; i < 9; i++) {
      EXPECT_EQ(tDOUBLE, c.GetDynLinkType(i) & tDOUBLE);
    }
    EXPECT_EQ(tBOOL, c.GetDynLinkType(9) & tBOOL);
    EXPECT_EQ(tWORD, c.GetDynLinkType(10) & tWORD);
    EXPECT_EQ(0, c.GetDynLinkType(11) & tSTRING);
  }

  TEST(TestCUnitMultiple, TestExective) {
    CUnitMultiple c;
    c.SetDouble(0, 2.0); // input1
    c.SetDouble(1, 3.0); // input2
    c.SetDouble(3, 1.0); // K1
    c.SetDouble(4, 1.0); // K2
    c.SetDouble(8, 10);		// Lolimit
    c.SetDouble(7, 19.0); // HiLimit
    c.Exective();
    EXPECT_DOUBLE_EQ(6.0, c.GetDouble(2));
    EXPECT_TRUE(c.IsOverFlow());
    c.SetDouble(8, 0); // HiLimit
    c.SetDouble(1, 5);
    c.Exective();
    EXPECT_DOUBLE_EQ(10, c.GetDouble(2));
    EXPECT_FALSE(c.IsOverFlow());
  }

  TEST(TestCUnitMultiple, TestSelectParameter) {
    CUnitMultiple c;

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
    c.SelectParameter(tOUTPUT | tWORD);
    EXPECT_EQ(10, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tINPUT | tWORD);
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