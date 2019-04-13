#include"stdafx.h"
#include"pch.h"

#include"CUnitDelayTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

namespace DACViewTest {
  TEST(TestCUnitDelayTime, TestCUnitDelayTimeInitialize) {
    CPoint pt(100, 100);
    CUnitDelayTime c("thisDelayTime", pt);
    EXPECT_STREQ("thisDelayTime", c.GetName());
  }

  TEST(TestCUnitDelayTime, TestSerialize) {
    CUnitDelayTime c;
    CFile cFile1, cFile2;
    char buffer[512];
    CString strFileName = "CUnitDelayTime.tst";
    CUnitDelayTime * pc = new CUnitDelayTime;
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);

    cFile1.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
    CArchive ar(&cFile1, CArchive::store, 512, buffer);
    for (int i = 2; i < 6; i++) {
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
    EXPECT_DOUBLE_EQ(2, pc->GetDouble(2));    
    EXPECT_DOUBLE_EQ(3, pc->GetDouble(3));
    EXPECT_DOUBLE_EQ(4, pc->GetDouble(4));
    EXPECT_DOUBLE_EQ(5, pc->GetDouble(5));

    delete pc;
  }
  TEST(TestCUnitDelayTime, TestGetClassname) {
    CUnitDelayTime c;
    EXPECT_STREQ("DelayTime", c.GetClassNameStr());
  }

  TEST(TestCUnitDelayTime, TestGetParaName) {
    CUnitDelayTime c;
    EXPECT_STREQ("Input", c.GetParaName(0));
    EXPECT_STREQ("Output", c.GetParaName(1));
    EXPECT_STREQ("HiRange", c.GetParaName(2));
    EXPECT_STREQ("LowRange", c.GetParaName(3));
    EXPECT_STREQ("HiLimit", c.GetParaName(4));
    EXPECT_STREQ("LowLimit", c.GetParaName(5));
    EXPECT_STREQ("AutoManual", c.GetParaName(6));
    EXPECT_STREQ("ScanRate", c.GetParaName(7));
    EXPECT_STREQ("", c.GetParaName(8));
  }

  TEST(TestCUnitDelayTime, TestGetParaType) {
    CUnitDelayTime c;
    EXPECT_EQ(tINPUT |tMODIFIABLE | tDOUBLE, c.GetParaType(0));
    EXPECT_EQ(tOUTPUT |tMODIFIABLE | tDOUBLE, c.GetParaType(1));
    EXPECT_EQ(tINPUT | tOUTPUT | tDOUBLE, c.GetParaType(2));
    EXPECT_EQ(tINPUT | tOUTPUT | tDOUBLE, c.GetParaType(3));
    EXPECT_EQ(tINPUT | tOUTPUT | tDOUBLE, c.GetParaType(4));
    EXPECT_EQ(tINPUT | tOUTPUT | tDOUBLE, c.GetParaType(5));
    EXPECT_EQ(tINPUT |tMODIFIABLE | tBOOL, c.GetParaType(6));
    EXPECT_EQ(tINPUT | tOUTPUT | tWORD, c.GetParaType(7));
    EXPECT_EQ(0, c.GetParaType(8));
  }

  TEST(TestCUnitDelayTime, TestGetBool) {
    CUnitDelayTime c;
    c.SetBool(6, true);
    EXPECT_TRUE(c.GetBool(6));
    c.SetBool(6, false);
    EXPECT_FALSE(c.GetBool(6));
  }

  TEST(TestCUnitDelayTime, TestGetInteger) {
    CUnitDelayTime c;
    EXPECT_TRUE(c.SetInteger(7, 100));
    EXPECT_EQ(100, c.GetInteger(7));
  }

  TEST(TestCUnitDelayTime, TestGetDouble) {
    CUnitDelayTime c;
    for (int i = 0; i < 5; i++) {
      EXPECT_TRUE(c.SetDouble(i, i * 10.5));
    }
    for (int j = 0; j < 5; j++) {
      EXPECT_DOUBLE_EQ(j*10.5, c.GetDouble(j));
    }
  }

  TEST(TestCUnitDelayTime, TestGetDynLinkType) {
    CUnitDelayTime c;
    for (int i = 0; i < 5; i++) {
      EXPECT_EQ(tDOUBLE, c.GetDynLinkType(i) & tDOUBLE);
    }
    EXPECT_EQ(tBOOL, c.GetDynLinkType(6) & tBOOL);
    EXPECT_EQ(tWORD, c.GetDynLinkType(7) & tWORD);
    EXPECT_EQ(0, c.GetDynLinkType(8) & tSTRING);
  }

  TEST(TestCUnitDelayTime, TestExective) {
    CUnitDelayTime c;


  }

  TEST(TestCUnitDelayTime, TestSelectParameter) {
    CUnitDelayTime c;

    c.SelectParameter(tINPUT | tDOUBLE);
    EXPECT_EQ(0, c.GetIndex(0));
    EXPECT_EQ(2, c.GetIndex(1));
    EXPECT_EQ(3, c.GetIndex(2));
    EXPECT_EQ(4, c.GetIndex(3));
    EXPECT_EQ(5, c.GetIndex(4));
    EXPECT_EQ(-1, c.GetIndex(5));
    c.SelectParameter(tOUTPUT | tDOUBLE);
    EXPECT_EQ(1, c.GetIndex(0));
    EXPECT_EQ(2, c.GetIndex(1));
    EXPECT_EQ(3, c.GetIndex(2));
    EXPECT_EQ(4, c.GetIndex(3));
    EXPECT_EQ(5, c.GetIndex(4));
    EXPECT_EQ(-1, c.GetIndex(5));
    c.SelectParameter(tINPUT | tBOOL);
    EXPECT_EQ(6, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tOUTPUT | tWORD);
    EXPECT_EQ(7, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tINPUT | tWORD);
    EXPECT_EQ(7, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tOUTPUT | tBOOL);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tOUTPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));

  }

}