#include"stdafx.h"
#include"pch.h"

#include"CUnitXor.h"

namespace DACViewTest {

  TEST(TestCUnitXor, TestInitialize) {
    CPoint pt(100, 100);
    CUnitXor c("CUnitXor", pt);

    CRect rect = c.GetSize();
    EXPECT_STREQ("CUnitXor", c.GetName());
    EXPECT_EQ(100 - 15, rect.left);
    EXPECT_EQ(100 - 15, rect.top);
    EXPECT_EQ(100 + 15, rect.right);
    EXPECT_EQ(100 + 15, rect.bottom);

    EXPECT_STREQ("Input1", c.GetParaName(0));
    EXPECT_STREQ("Input2", c.GetParaName(1));
    EXPECT_STREQ("Output", c.GetParaName(2));
    EXPECT_STREQ("EAlarm", c.GetParaName(3));
    EXPECT_STREQ("Alarm", c.GetParaName(4));
    EXPECT_STREQ("AutoManual", c.GetParaName(5));
    EXPECT_STREQ("ScanRate", c.GetParaName(6));
    EXPECT_STREQ("", c.GetParaName(7));

    EXPECT_STREQ("Xor", c.GetClassName());

    CUnitXor c2;
    rect = c2.GetSize();
    EXPECT_STREQ("", c2.GetName());
    EXPECT_EQ(0, rect.left);
    EXPECT_EQ(0, rect.top);
    EXPECT_EQ(0, rect.right);
    EXPECT_EQ(0, rect.bottom);
  }

  TEST(TestCUnitXor, TestSerialize) {
    CUnitXor c;
    CFile cFile1, cFile2;
    char buffer[512];
    CString strFileName = "CUnitXor.tst";
    CUnitXor * pc = new CUnitXor;
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);

    cFile1.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
    CArchive ar(&cFile1, CArchive::store, 512, buffer);
    for (int i = 3; i < 4; i++) {
      pc->SetBool(i, false);
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
    EXPECT_FALSE(pc->GetBool(3));
    EXPECT_FALSE(pc->GetBool(4));
    delete pc;
  }

  TEST(TestCUnitXor, TestGetParaType) {
    CUnitXor c;
    EXPECT_EQ(c.GetParaType(0), tINPUT |tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(1), tINPUT |tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(2), tOUTPUT |tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(3), tINPUT |tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(4), tOUTPUT |tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(5), tINPUT |tMODIFIABLE | tBOOL);
    EXPECT_EQ(c.GetParaType(6), tINPUT | tOUTPUT | tWORD);

  }

  TEST(TestCUnitXor, TestExective) {
    CUnitXor c;
    c.SetBool(3, true); // enable alarm
    c.SetBool(0, false);
    c.SetBool(1, false);
    c.Exective();    
    EXPECT_FALSE(c.GetBool(2));
    EXPECT_FALSE(c.GetBool(4));
    c.SetBool(0, true);
    c.Exective();
    EXPECT_TRUE(c.GetBool(2));
    EXPECT_TRUE(c.GetBool(4));
    c.SetBool(1, true);    
    c.SetBool(0, false);
    c.Exective();
    EXPECT_TRUE(c.GetBool(2));
    EXPECT_TRUE(c.GetBool(4));
    c.SetBool(0, true);
    c.SetBool(1, true);
    c.Exective();
    EXPECT_FALSE(c.GetBool(2));
    EXPECT_FALSE(c.GetBool(4));

    c.SetInteger(6, 10000);
    EXPECT_EQ(10000, c.GetInteger(6));
  }

  TEST(TestCUnitXor, TestGetDynLinkType) {
    CUnitXor c;
    EXPECT_EQ(tBOOL, c.GetDynLinkType(0));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(1));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(2));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(3));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(4));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(5));
    EXPECT_EQ(tWORD, c.GetDynLinkType(6));
  }


  TEST(TestCUnitXor, TestSelectParameter) {
    CUnitXor c;

    c.SelectParameter(tINPUT | tBOOL);
    EXPECT_EQ(0, c.GetIndex(0));
    EXPECT_EQ(1, c.GetIndex(1));
    EXPECT_EQ(3, c.GetIndex(2));
    EXPECT_EQ(5, c.GetIndex(3));
    EXPECT_EQ(-1, c.GetIndex(4));
    c.SelectParameter(tOUTPUT | tBOOL);
    EXPECT_EQ(2, c.GetIndex(0));
    EXPECT_EQ(4, c.GetIndex(1));
    EXPECT_EQ(-1, c.GetIndex(2));
    c.SelectParameter(tOUTPUT | tWORD);
    EXPECT_EQ(6, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tINPUT | tWORD);
    EXPECT_EQ(6, c.GetIndex(0));
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