#include"stdafx.h"
#include"pch.h"

#include"CUnitHPBG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

namespace DACViewTest {
  TEST(TestCUnitHighPassBargin, TestCUnitHighPassBarginInitialize) {
    CPoint pt(100, 100);
    CUnitHighPassBargin c("CUnitHPBG", pt);

    CRect rect = c.GetSize();
    EXPECT_STREQ("CUnitHPBG", c.GetName());
    EXPECT_EQ(100 - 15, rect.left);
    EXPECT_EQ(100 - 15, rect.top);
    EXPECT_EQ(100 + 15, rect.right);
    EXPECT_EQ(100 + 15, rect.bottom);

    EXPECT_STREQ("Input", c.GetParaName(0));
    EXPECT_STREQ("Bargin", c.GetParaName(1));
    EXPECT_STREQ("Output", c.GetParaName(2));
    EXPECT_STREQ("EAlarm", c.GetParaName(3));
    EXPECT_STREQ("Alarm", c.GetParaName(4));
    EXPECT_STREQ("AutoManual", c.GetParaName(5));
    EXPECT_STREQ("ScanRate", c.GetParaName(6));
    EXPECT_STREQ("", c.GetParaName(7));

    EXPECT_STREQ("HPBG", c.GetClassName());

    CUnitHighPassBargin c2;
    rect = c2.GetSize();
    EXPECT_STREQ("", c2.GetName());
    EXPECT_EQ(0, rect.left);
    EXPECT_EQ(0, rect.top);
    EXPECT_EQ(0, rect.right);
    EXPECT_EQ(0, rect.bottom);
  }

  TEST(TestCUnitHighPassBargin, TestSerialize) {
    CUnitHighPassBargin c;
    CFile cFile1, cFile2;
    char buffer[512];
    CString strFileName = "CUnitHighPassBargin.tst";
    CUnitHighPassBargin * pc = new CUnitHighPassBargin;
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);

    cFile1.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
    CArchive ar(&cFile1, CArchive::store, 512, buffer);
    pc->SetDouble(1, 10);
    pc->SetBool(3, true);
    ar << pc; // 
    ar.Flush(); // 必须flush，否则有可能没进行存储
    cFile1.Close();
    delete pc;

    if (!cFile2.Open(strFileName, CFile::modeRead)) {
      c.SetName("aa");
    }
    CArchive ar2(&cFile2, CArchive::load, 512, buffer);
    ar2 >> pc;
    EXPECT_DOUBLE_EQ(10.0, pc->GetDouble(1));
    EXPECT_TRUE(pc->GetBool(3));

    delete pc;
  }
  TEST(TestCUnitHighPassBargin, TestGetClassname) {
    CUnitHighPassBargin c;
    EXPECT_STREQ("HPBG", c.GetClassName());
  }

  TEST(TestCUnitHighPassBargin, TestGetParaName) {
    CUnitHighPassBargin c;
    EXPECT_STREQ("Input", c.GetParaName(0));
    EXPECT_STREQ("Bargin", c.GetParaName(1));
    EXPECT_STREQ("Output", c.GetParaName(2));
    EXPECT_STREQ("EAlarm", c.GetParaName(3));
    EXPECT_STREQ("Alarm", c.GetParaName(4));
    EXPECT_STREQ("AutoManual", c.GetParaName(5));
    EXPECT_STREQ("ScanRate", c.GetParaName(6));
    EXPECT_STREQ("", c.GetParaName(7));
  }

  TEST(TestCUnitHighPassBargin, TestGetParaType) {
    CUnitHighPassBargin c;
    EXPECT_EQ(tINPUT |tMODIFIABLE | tWORD |tDOUBLE, c.GetParaType(0));
    EXPECT_EQ(tINPUT |tMODIFIABLE | tWORD | tDOUBLE, c.GetParaType(1));
    EXPECT_EQ(tOUTPUT |tMODIFIABLE | tBOOL, c.GetParaType(2));
    EXPECT_EQ(tOUTPUT |tMODIFIABLE | tBOOL, c.GetParaType(3));
    EXPECT_EQ(tINPUT |tMODIFIABLE | tBOOL, c.GetParaType(4));
    EXPECT_EQ(tINPUT |tMODIFIABLE | tBOOL, c.GetParaType(5));
    EXPECT_EQ(tINPUT | tOUTPUT | tWORD, c.GetParaType(6));
    EXPECT_EQ(0, c.GetParaType(7));
  }

  TEST(TestCUnitHighPassBargin, TestGetBool) {
    CUnitHighPassBargin c;
    c.SetBool(3, true);
    EXPECT_TRUE(c.GetBool(3));
    c.SetBool(3, false);
    EXPECT_FALSE(c.GetBool(3));
  }

  TEST(TestCUnitHighPassBargin, TestGetInteger) {
    CUnitHighPassBargin c;
    EXPECT_TRUE(c.SetInteger(6, 1000));
    EXPECT_EQ(1000, c.GetInteger(6));
  }

  TEST(TestCUnitHighPassBargin, TestGetDouble) {
    CUnitHighPassBargin c;
    for (int i = 0; i < 1; i++) {
      EXPECT_TRUE(c.SetDouble(i, i * 10.5));
    }
    for (int j = 0; j < 1; j++) {
      EXPECT_DOUBLE_EQ(j*10.5, c.GetDouble(j));
    }
  }

  TEST(TestCUnitHighPassBargin, TestGetDynLinkType) {
    CUnitHighPassBargin c;
    for (int i = 0; i < 1; i++) {
      EXPECT_EQ(tDOUBLE, c.GetDynLinkType(i) & tDOUBLE);
    }
    for (int i = 2; i < 6; i++) {
      EXPECT_EQ(tBOOL, c.GetDynLinkType(i) & tBOOL);
    }
    EXPECT_EQ(tWORD, c.GetDynLinkType(6) & tWORD);
  }

  TEST(TestCUnitHighPassBargin, TestExective) {
    CUnitHighPassBargin c;
    c.SetDouble(0, 2.0); // input
    c.SetDouble(1, 5.0); // Bargin
    c.Exective();
    EXPECT_FALSE(c.GetBool(2));
    c.SetDouble(0, 10);
    c.Exective();
    EXPECT_TRUE(c.GetBool(2));

  }


  TEST(TestCUnitHighPassBargin, TestSelectParameter) {
    CUnitHighPassBargin c;

    c.SelectParameter(tINPUT | tDOUBLE);
    EXPECT_EQ(0, c.GetIndex(0));
    EXPECT_EQ(1, c.GetIndex(1));
    EXPECT_EQ(-1, c.GetIndex(2));
    c.SelectParameter(tOUTPUT | tBOOL);
    EXPECT_EQ(2, c.GetIndex(0));
    EXPECT_EQ(3, c.GetIndex(1));
    EXPECT_EQ(-1, c.GetIndex(2));
    c.SelectParameter(tINPUT | tBOOL);
    EXPECT_EQ(4, c.GetIndex(0));
    EXPECT_EQ(5, c.GetIndex(1));
    EXPECT_EQ(-1, c.GetIndex(2));
    c.SelectParameter(tOUTPUT | tWORD);
    EXPECT_EQ(6, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tOUTPUT | tDOUBLE);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tWORD);
    EXPECT_EQ(0, c.GetIndex(0));
    EXPECT_EQ(1, c.GetIndex(1));
    EXPECT_EQ(6, c.GetIndex(2));
    EXPECT_EQ(-1, c.GetIndex(3));
    c.SelectParameter(tINPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tOUTPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));

  }



}