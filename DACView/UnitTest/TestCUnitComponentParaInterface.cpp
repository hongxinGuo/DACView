#include"stdafx.h"

#include"gtest/gtest.h"

#include"CUnitAdd.h"
#include"CUnitComponentParaInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

namespace DACViewTest {
	TEST(TestCUCParaInterface, TestInitialize) {
			CUCParaInterface c;		
			CString str = "Para";
			char buffer[40];
			for (int i = 0; i < 16; i++) {
				_itoa(i, buffer, 10);
				EXPECT_STREQ(str + buffer, c.GetName(i));
				EXPECT_EQ(0, c.GetDynLinkType(i));
				EXPECT_EQ(-1, c.GetSrcIndex(i));
        EXPECT_EQ(-1, c.GetDestIndex(i));
				EXPECT_FALSE(c.IsLinked(i)); 
			}
	}

	TEST(TestCUCParaInterface, TestSerialize) {
		CUCParaInterface c, c2, *pc;
		CFile cFile1, cFile2;
		char buffer[512];
		CPoint pt1(100, 100);
		CUnitAdd c1("c1", pt1), c4("UnitAdd2", pt1), *cp2;
		CString strFileName = "CUCParaInterface.tst";
		CUnitBase * cp1;
	
		CString str = "Para";
		for (int i = 0; i < 16; i++) {
			_itoa(i, buffer, 10);
			c.SetName(i, str + buffer);
			c.SetSrcIndex(i, i);
			c.SetDestIndex(i, i);
			c.SetSrcUnit(i, &c1);
			c.SetDestUnit(i, &c4);
			c.SetLinkedFlag(i, true);
		}
		c.SetName(3, "Name");

		c.SetDynLinkType(0, tBOOL | tINPUT);
    EXPECT_EQ(tBOOL, c.GetDynLinkType(0));
		c.SetDynLinkType(1, tBOOL);
    EXPECT_EQ(tBOOL, c.GetDynLinkType(1));
		c.SetDynLinkType(4, tWORD | tOUTPUT);
    EXPECT_EQ(tWORD, c.GetDynLinkType(4));
		c.SetDynLinkType(8, tDOUBLE | tINPUT);
    EXPECT_EQ(tDOUBLE, c.GetDynLinkType(8));
		c.SetDynLinkType(12, tSTRING | tOUTPUT);
    EXPECT_EQ(tSTRING, c.GetDynLinkType(12));

    c.SetParaType(0, tINPUT | tBOOL);
    EXPECT_EQ(tBOOL | tINPUT, c.GetParaType(0));
    EXPECT_EQ(tBOOL, c.GetDynLinkType(0));
    c.SetParaType(3, tOUTPUT | tWORD);
    EXPECT_EQ(tWORD | tOUTPUT, c.GetParaType(3));
    EXPECT_EQ(tWORD, c.GetDynLinkType(3));
    c.SetParaType(5, tINPUT | tDOUBLE);
    EXPECT_EQ(tDOUBLE | tINPUT, c.GetParaType(5));
    EXPECT_EQ(tDOUBLE, c.GetDynLinkType(5));
    c.SetParaType(10, tOUTPUT | tSTRING);
    EXPECT_EQ(tSTRING | tOUTPUT, c.GetParaType(10));
    EXPECT_EQ(tSTRING, c.GetDynLinkType(10));


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
		EXPECT_EQ(5, pc->GetSrcIndex(5));
		EXPECT_EQ(tBOOL, pc->GetDynLinkType(0));
		EXPECT_EQ(tWORD, pc->GetDynLinkType(4));
		for (int i = 0; i < 16; i++) {
			EXPECT_TRUE(pc->IsLinked(i));
			EXPECT_STREQ("c1", pc->GetSrcUnit(i)->GetName());
			EXPECT_STREQ("UnitAdd2", pc->GetDestUnit(i)->GetName());
		}
		EXPECT_STREQ("Name", pc->GetName(3));
		EXPECT_STREQ("Para6", pc->GetName(6));
		
		cp1 = pc->GetDestUnit(0);
		delete cp1;
		cp1 = pc->GetSrcUnit(0);
		delete cp1;
		delete pc;
	}

	TEST(TestCUCParaInterface, TestGetSrcUnit) {
		CUnitAdd c1;
		CUCParaInterface c;

		for (int i = 0; i < 16; i++) {
			c.SetSrcUnit(i, &c1);
			EXPECT_EQ(&c1, c.GetSrcUnit(i));
		}
		EXPECT_FALSE(c.SetSrcUnit(-1, &c1));
		EXPECT_FALSE(c.SetSrcUnit(16, &c1));
	}

	TEST(TestCUCParaInterface, TestGetDestUnit) {
		CUnitAdd c1;
		CUCParaInterface c;

		for (int i = 0; i < 16; i++) {
			c.SetDestUnit(i, &c1);
			EXPECT_EQ(&c1, c.GetDestUnit(i));
		}
		EXPECT_FALSE(c.SetDestUnit(-1, &c1));
		EXPECT_FALSE(c.SetDestUnit(16, &c1));
	}

	TEST(TestCUCParaInterface, TestGetName) {
		CUCParaInterface c;

		for (int i = 0; i < 16; i++) {
			c.SetName(i, "Name");
			EXPECT_STREQ("Name", c.GetName(i));
		}
		EXPECT_FALSE(c.SetName(-1, "Name"));
		EXPECT_FALSE(c.SetName(16, "Name"));
	}

	TEST(TestCUCParaInterface, TestGetDynLinkType) {
		CUCParaInterface c;

		for (int i = 0; i < 16; i++) {
			c.SetDynLinkType(i, tDOUBLE);
			EXPECT_DOUBLE_EQ(tDOUBLE, c.GetDynLinkType(i) & tDOUBLE);
		}
//		EXPECT_ANY_THROW(c.SetDynLinkType(-1, tDOUBLE));
		EXPECT_FALSE(c.SetDynLinkType(-1,tDOUBLE));
		EXPECT_FALSE(c.SetDynLinkType(16,tDOUBLE));
	}

	TEST(TestCUCParaInterface, TestGetIndex) {
		CUCParaInterface c;

		for (int i = 0; i < 16; i++) {
			c.SetSrcIndex(i, i);
			EXPECT_EQ(i, c.GetSrcIndex(i));
			c.SetDestIndex(i, i);
			EXPECT_EQ(i, c.GetSrcIndex(i));
		}
		EXPECT_FALSE(c.SetSrcIndex(-1, 1));
		EXPECT_FALSE(c.SetSrcIndex(16, 2));
		EXPECT_FALSE(c.SetDestIndex(-1, 1));
		EXPECT_FALSE(c.SetDestIndex(16, 2));
	}

	TEST(TestCUCParaInterface, TestSetValue) {
		CUCParaInterface c;
		bool f1 = true, f2 = false;
		int i1 = 10, i2 = -10;
		long ll1 = 100, ll2 = -100;
		double d1 = 1000, d2 = -1000;
		CString str1 = "CString1", str2="CStr2";

		c.SetDynLinkType(0, tBOOL);
		c.SetBoolValue(0, f1);
		EXPECT_TRUE(c.GetBoolValue(0));
		EXPECT_FALSE(f2);

		c.SetDynLinkType(1, tWORD);
		c.SetIntegerValue(1, i1);
		EXPECT_TRUE(c.GetIntegerValue(1));

		c.SetDynLinkType(2, tDOUBLE);
		c.SetDoubleValue(2, d1);
		EXPECT_DOUBLE_EQ(1000, c.GetDoubleValue(2));
		EXPECT_DOUBLE_EQ(-1000,d2);

		c.SetDynLinkType(3, tSTRING);
		c.SetStringValue(3, str1);
		EXPECT_TRUE(str2 = c.GetStringValue(3));
		EXPECT_STREQ(str1, str2);

		EXPECT_FALSE(c.SetDynLinkType(-1, tDOUBLE));
		EXPECT_FALSE(c.SetDynLinkType(16, tDOUBLE));
	}

  TEST(TestCUCParaInterface, TestSetExectivePriority) {
    CUCParaInterface c;
    int i;

    for (i = 0; i < 16; i++) {
      c.SetExectivePriority(i, i);
      EXPECT_EQ(c.GetExectivePriority(i), i);
    }
  }

	TEST(TestCUCParaInterface, TestIsLinkeed) {
		CUCParaInterface c;
		int i;
		
		for (i = 0; i < 16; i++) {
			c.SetLinkedFlag(i, true);
			EXPECT_TRUE(c.IsLinked(i));
		}
		for (i = 0; i < 16; i++) {
			c.SetLinkedFlag(i, false);
			EXPECT_FALSE(c.IsLinked(i));
		}
	}

  TEST(TestCUCParaInterface, TestPrepareINterfacepara) {
    CUCParaInterface c;
    int i, j;

    for (i = 0; i < 16; i++) {
      for (j = 0; j < 16; j++) {
        c.PrepareInterfacePara(i, j, true);
        EXPECT_TRUE(c.IsInnerDataLinked(i, j));
      }
      for (j = 0; j < 16; j++) {
        c.PrepareInterfacePara(i, j, false);
        EXPECT_FALSE(c.IsInnerDataLinked(i, j));
      }
    }
  }


}



