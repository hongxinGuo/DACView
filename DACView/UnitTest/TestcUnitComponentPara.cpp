#include"stdafx.h"
#include"pch.h"

#include"CUnitComponentPara.h"
#include"CUnitAdd.h"

namespace DACViewTest {

	TEST(TestCUnitComponentPara, TestInitialize) {
		CUCPara c;

		EXPECT_STREQ("Para", c.GetName());
		EXPECT_EQ(nullptr, c.GetSrcUnit());
		EXPECT_EQ(nullptr, c.GetDestUnit());
		EXPECT_EQ(c.GetDynLinkType(), 0);
		EXPECT_EQ(-1, c.GetSrcIndex());     // 索引位置不能为负值，故设置初始值为-1
		EXPECT_EQ(-1, c.GetDestIndex());    // 索引位置不能为负值，故设置初始值为-1
		EXPECT_FALSE( c.IsLinked());

    for (int i = 0; i < 16; i++) {
      EXPECT_FALSE(c.IsDataLinked(i));
    }
	}

	TEST(TestCUnitComponentPara, TestSerialize) {
		CUCPara c, c2, *pc;
		CFile cFile1, cFile2;
		char buffer[512];
		CPoint pt1(100, 100);
		CUnitAdd c1("c1", pt1), c4("UnitAdd2", pt1);
		CString strFileName = "CUCPara.tst";
		CUnitBase * cp1;

		c.SetSrcIndex(2);
		c.SetDestIndex(1);
		c.SetSrcUnit(&c1);
		c.SetDestUnit(&c4);
		c.SetLinkedFlag(true);
		c.SetName("Name");
		c.SetDynLinkType(tWORD);

		cFile1.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
		CArchive ar(&cFile1, CArchive::store, 512, buffer);

		pc = &c;

		ar << pc; // 
		ar.Flush(); // 必须flush，否则有可能没进行存储
		cFile1.Close();

		if (!cFile2.Open(strFileName, CFile::modeRead)) {
			ASSERT(0);
		}
		CArchive ar2(&cFile2, CArchive::load, 512, buffer);
		ar2 >> pc;
		EXPECT_EQ(2, pc->GetSrcIndex());
		EXPECT_EQ(tWORD, pc->GetDynLinkType());
		EXPECT_TRUE(pc->IsLinked());
		EXPECT_STREQ("c1", pc->GetSrcUnit()->GetName());
		EXPECT_STREQ("UnitAdd2", pc->GetDestUnit()->GetName());
		EXPECT_STREQ("Name", pc->GetName());

		cp1 = pc->GetDestUnit();
		delete cp1;
		cp1 = pc->GetSrcUnit();
		delete cp1;
		delete pc;
	}

	TEST(TestCUnitComponentPara, TestGetSrcUnit) {
		CUCPara c;
		CUnitBase cp, *pc;
		cp.SetName("CUnitBase1");
		c.SetSrcUnit(&cp);
		pc = c.GetSrcUnit();
		EXPECT_STREQ("CUnitBase1", pc->GetName());
	}

	TEST(TestCUnitComponentPara, TestGetDestUnit) {
		CUCPara c;
		CUnitBase cp, *pc;
		cp.SetName("CUnitBase1");
		c.SetDestUnit(&cp);
		pc = c.GetDestUnit();
		EXPECT_STREQ("CUnitBase1", pc->GetName());
	}

	TEST(TestCUnitComponentPara, TestGetName) {
		CUCPara c;
		c.SetName("CUCP1");
		EXPECT_STREQ("CUCP1", c.GetName());
	}

  TEST(TestCUnitComponentPara, TestGetDynLinkType) {
    CUCPara c;
    c.SetParaType(tINPUT | tWORD);
    EXPECT_EQ(tINPUT | tWORD, c.GetParaType());
    c.SetDynLinkType(tWORD);
    EXPECT_EQ(tWORD, c.GetDynLinkType());
		c.SetParaType(tINPUT | tOUTPUT | tWORD);
		EXPECT_EQ(tINPUT | tOUTPUT | tWORD, c.GetParaType());
    EXPECT_EQ(tWORD, c.GetDynLinkType());
    c.SetDynLinkType(tINPUT | tWORD);
    EXPECT_EQ(tWORD, c.GetDynLinkType());
    c.SetParaType(tOUTPUT | tDOUBLE);
    EXPECT_EQ(tDOUBLE, c.GetDynLinkType());
    EXPECT_EQ(tDOUBLE | tOUTPUT, c.GetParaType());
	}

	TEST(TestCUnitComponentPara, TestGetSrcIndex) {
		CUCPara c;
		c.SetSrcIndex(10);
		EXPECT_EQ(10, c.GetSrcIndex());
	}
	
	TEST(TestCUnitComponentPara, TestGetDestIndex) {
    CUCPara c;
    c.SetDestIndex(10);
    EXPECT_EQ(10, c.GetDestIndex());
	}

	TEST(TestCUnitComponentPara, TestHaveDataLink) {
		CUCPara c;
		c.SetDestIndex(10);
		EXPECT_EQ(10, c.GetDestIndex());
	}

	TEST(TestCUnitComponentPara, TestIsDataLinked) {
		CUCPara c;
		for (int i = 0; i < 16; i++) {
			EXPECT_FALSE(c.IsDataLinked(i));
		}
		EXPECT_FALSE(c.HaveDataLink());
		c.SetDataLinked(10, true);
		EXPECT_TRUE(c.IsDataLinked(10));
		EXPECT_TRUE(c.HaveDataLink());
	}
	
	// 运行时无法分辨各类型的差别，都归到bool型函数上去了，需要改变实现方法。
	TEST(TestCUnitComponentPara, TestSetValue) {
	
		CUCPara c;
		bool * pf = new bool;
		*pf = false;
		c.SetDynLinkType(tBOOL);
		c.SetBoolValue(true);
		*pf = c.GetBoolValue();
		EXPECT_TRUE(*pf);
		c.SetBoolValue(false);
		*pf = c.GetBoolValue();
		EXPECT_FALSE(*pf);
		delete pf;

		int *pi = new int, *pi1 = new int;
		*pi = 15;
		*pi1 = 30;
		c.SetDynLinkType(tWORD);
		c.SetIntegerValue(*pi);
		*pi1 = c.GetIntegerValue();
		EXPECT_EQ(15, *pi1);
		delete pi;
		delete pi1;

		double * pd = new double, * pd2 = new double;
		*pd = 300.1;
		*pd2 = 400.2;
		c.SetDynLinkType(tDOUBLE);
		c.SetDoubleValue(*pd);
		*pd2 = c.GetDoubleValue();
		EXPECT_DOUBLE_EQ(300.1, *pd2);
		delete pd;
		delete pd2;

		CString str = "aaa", str2 = "bbb";
		c.SetDynLinkType(tSTRING);
		c.SetStringValue(str);
		str2 = c.GetStringValue();
		EXPECT_STREQ("aaa", str2);
	}

	TEST(TestCUnitComponentPara, TestIsLinked) {
    CUCPara c;

    EXPECT_FALSE(c.IsLinked());
    c.SetLinkedFlag(true);
    EXPECT_TRUE(c.IsLinked());
    c.SetLinkedFlag(false);
    EXPECT_FALSE(c.IsLinked());
	}

  TEST(TestCUnitComponentPara, TestIsInnerLinked) {
    CUCPara c;
    int i;
    for (i = 0; i < 16; i++) {
      EXPECT_FALSE(c.IsInnerDataLinked(i));
    }
    for (i = 0; i < 16; i++) {
      c.SetInnerDataLinked(i, true);
    }
    for (i = 0; i < 16; i++) {
      EXPECT_TRUE(c.IsInnerDataLinked(i));
    }
    for (i = 0; i < 16; i++) {
      c.SetInnerDataLinked(i, false);
    }
    for (i = 0; i < 16; i++) {
      EXPECT_FALSE(c.IsInnerDataLinked(i));
    }
  }

}