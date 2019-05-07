#include"afx.h"
 
#include"pch.h"

#include"CObjectPrimitive.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

namespace DACViewTest {
	TEST(TestCObjectPrimitive, TestGetClassNameStr){
		// TODO: Your test code here
		CObjectPrimitive c;
		CString str = "CObjectPrimitive";
		CString str2 = c.GetClassName();
		EXPECT_STREQ(str, str2);
	}

	TEST(TestCObjectPrimitive,TestCObjPrimitiveInitialize){
		CObjectPrimitive c;
		CString str = "";
		EXPECT_STREQ(str, c.GetName());
		EXPECT_EQ(100, c.GetScanRate());
		ASSERT_EQ(false, c.IsNeedUpdate());
		ASSERT_EQ(false, c.IsSelect());
		ASSERT_EQ(false, c.CanViewIn());
		ASSERT_EQ(true, c.CheckSelf());

		c.SetScanRate(1);
		ASSERT_EQ(1, c.GetScanRate());
		c.SetUpdateFlag(true);
		EXPECT_TRUE(c.IsNeedUpdate());
		str = "any name";
		c.SetName(str);
		ASSERT_STREQ(str, c.GetName());

	}
	TEST(TestCObjectPrimitive, TestCObjectPrimitiveCountDown) {
		CObjectPrimitive c;
		c.SetScanRate(50);
		c.CountDown(1);
		EXPECT_FALSE(c.CountDown(48));
		EXPECT_TRUE(c.CountDown(2));
	}

	TEST(TestCObjectPrimitive, TestCObjectPrimitiveSerialize) {
		CObjectPrimitive c;
		CFile cFile1, cFile2;
		char buffer[512];
		CString strFileName = "CObjectPrimitve.tst";
		CObjectPrimitive * pc = new CObjectPrimitive, * pc2;

		cFile1.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
		CArchive ar(&cFile1, CArchive::store, 512, buffer);
		pc->SetScanRate(150);
		pc->SetName(strFileName);
		ar << pc; // 似乎是缓存区的问题，存一个pc的话无法读出，只能存一千个，然后读一个。
		ar.Flush(); // 实际上是flush的问题。
		cFile1.Close();	

		if (!cFile2.Open(strFileName, CFile::modeRead)) {
			c.SetName("aa");
		}
		CArchive ar2(&cFile2, CArchive::load, 512, buffer);
		ar2 >> pc2;
		EXPECT_EQ(150, pc->GetScanRate());
		EXPECT_STREQ(strFileName, pc2->GetName());
		delete pc;
		delete pc2;
	}

  TEST(TestCObjectPrimitive, TestSetSelect) {
    CObjectPrimitive c;

    EXPECT_FALSE(c.IsSelect());
    c.SetSelect(true);
    EXPECT_TRUE(c.IsSelect());
    c.SetSelect(false);
    EXPECT_FALSE(c.IsSelect());
  }

}