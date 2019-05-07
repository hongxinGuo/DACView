#include"stdafx.h"
#include"pch.h"

#include"CUnitFFLP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

namespace DACViewTest {
  TEST(TestCUnitFFLP, TestCUnitFFLPInitialize) {
    CPoint pt(100, 100);
    CUnitFFLP c("thisFFLP", pt);
    EXPECT_STREQ("thisFFLP", c.GetName());

    CRect rect = c.GetSize();
    EXPECT_EQ(100 - 15, rect.left);
    EXPECT_EQ(100 - 15, rect.top);
    EXPECT_EQ(100 + 15, rect.right);
    EXPECT_EQ(100 + 15, rect.bottom);

    EXPECT_STREQ("InputStatus", c.GetParaName(0));
    EXPECT_STREQ("InputFlip", c.GetParaName(1));
    EXPECT_STREQ("Output", c.GetParaName(2));
    EXPECT_STREQ("EAlarm", c.GetParaName(3));
    EXPECT_STREQ("Alarm", c.GetParaName(4));
    EXPECT_STREQ("AutoManual", c.GetParaName(5));
    EXPECT_STREQ("ScanRate", c.GetParaName(6));
    EXPECT_STREQ("", c.GetParaName(7));

    EXPECT_STREQ("FFLP", c.GetClassName());

    CUnitFFLP c2;
    rect = c2.GetSize();
    EXPECT_STREQ("", c2.GetName());
    EXPECT_EQ(0, rect.left);
    EXPECT_EQ(0, rect.top);
    EXPECT_EQ(0, rect.right);
    EXPECT_EQ(0, rect.bottom);
  }

  TEST(TestCUnitFFLP, TestSerialize) {
    CUnitFFLP c;
    CFile cFile1, cFile2;
    char buffer[512];
    CString strFileName = "CUnitFFLP.tst";
    CUnitFFLP * pc = new CUnitFFLP;
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);

    cFile1.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
    CArchive ar(&cFile1, CArchive::store, 512, buffer);
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
    rect = pc->GetSize();
    EXPECT_TRUE(pc->GetBool(3));

    delete pc;
  }
  TEST(TestCUnitFFLP, TestGetClassname) {
    CUnitFFLP c;
    EXPECT_STREQ("FFLP", c.GetClassName());
  }

  TEST(TestCUnitFFLP, TestGetParaName) {
    CUnitFFLP c;
    EXPECT_STREQ("InputStatus", c.GetParaName(0));
    EXPECT_STREQ("InputFlip", c.GetParaName(1));
    EXPECT_STREQ("Output", c.GetParaName(2));
    EXPECT_STREQ("EAlarm", c.GetParaName(3));
    EXPECT_STREQ("Alarm", c.GetParaName(4));
    EXPECT_STREQ("AutoManual", c.GetParaName(5));
    EXPECT_STREQ("ScanRate", c.GetParaName(6));
    EXPECT_STREQ("", c.GetParaName(7));
  }

  TEST(TestCUnitFFLP, TestGetParaType) {
    CUnitFFLP c;
    EXPECT_EQ(tINPUT |tMODIFIABLE | tBOOL, c.GetParaType(0));
    EXPECT_EQ(tINPUT |tMODIFIABLE | tBOOL, c.GetParaType(1));
    EXPECT_EQ(tOUTPUT |tMODIFIABLE | tBOOL, c.GetParaType(2));
    EXPECT_EQ(tINPUT |tMODIFIABLE | tBOOL, c.GetParaType(3));
    EXPECT_EQ(tOUTPUT |tMODIFIABLE | tBOOL, c.GetParaType(4));
    EXPECT_EQ(tINPUT |tMODIFIABLE | tBOOL, c.GetParaType(5));
    EXPECT_EQ(tINPUT | tOUTPUT | tWORD, c.GetParaType(6));
    EXPECT_EQ(0, c.GetParaType(7));
  }


  TEST(TestCUnitFFLP, TestGetInteger) {
    CUnitFFLP c;
    EXPECT_TRUE(c.SetInteger(6, 1000));
    EXPECT_EQ(1000, c.GetInteger(6));
  }

  TEST(TestCUnitFFLP, TestGetBool) {
    CUnitFFLP c;
    for (int i = 0; i < 6; i++) {
      c.SetBool(i, true);
    }
    for (int j = 0; j < 6; j++) {
      if (j == 2) EXPECT_FALSE(c.GetBool(2)) << "output只能输出，不能设置其状态";
      else EXPECT_TRUE(c.GetBool(j));
    }
  }

  TEST(TestCUnitFFLP, TestGetDynLinkType) {
    CUnitFFLP c;
    for (int i = 0; i < 6; i++) {
      EXPECT_EQ(tBOOL, c.GetDynLinkType(i) & tBOOL);
    }
    EXPECT_EQ(tWORD, c.GetDynLinkType(6) & tWORD);
  }

  TEST(TestCUnitFFLP, TestExective) {
    CUnitFFLP c;
    c.SetBool(0, false); // 输入为假
    c.Exective();
    EXPECT_FALSE(c.GetBool(2)) << "输入为假时，输出为假";
    c.SetBool(1, false); // 触发信号为假
    c.SetBool(0, true); // 输入为真
    c.Exective();
    EXPECT_FALSE(c.GetBool(2)) << "当触发信号为假时，输出为假";
    c.SetBool(1, true); // 触发信号为真
    c.Exective();
    EXPECT_TRUE(c.GetBool(2)) << "触发信号为真时，输入为真则输出为真";
  }

  TEST(TestCUnitFFLP, TestSelectParameter) {
    CUnitFFLP c;

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
    c.SelectParameter(tINPUT | tWORD);
    EXPECT_EQ(6, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tOUTPUT | tWORD);
    EXPECT_EQ(6, c.GetIndex(0));
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