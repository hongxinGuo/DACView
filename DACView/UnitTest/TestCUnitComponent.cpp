#include"stdafx.h"
#include"pch.h"

#include"CUnitDictionary.h"
#include"CUnitcomponent.h"
#include"CUnitAdd.h"
#include"CUnitDynLink.h"

namespace DACViewTest {

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 


	TEST(TestCUnitComponent, TestInitialize) {
    CString str = "Para";
    char buffer[50];

		CPoint pt(100, 100);
		CUnitComponent c("UnitComponent", pt, true);// 可被编译部件
		EXPECT_STREQ("UnitComponent", c.GetName());
		CRect rect = c.GetSize();
		EXPECT_EQ(rect.top, 100 - 20);
		EXPECT_EQ(rect.bottom, 100 + 20);
		EXPECT_EQ(rect.left, 100 - 40);
		EXPECT_EQ(rect.right, 100 + 40);
    EXPECT_TRUE(c.IsEncapsulable()) << "可被编译部件";
    for (int i = 0; i < 16; i++) {
      _itoa_s(i, buffer, 10);
      EXPECT_STREQ(str + buffer, c.GetParaName(i));
    }

    CUnitComponent c1("UnitComponent", pt, false); // 不可被编译部件
    EXPECT_FALSE(c1.IsEncapsulable()) << "不可被编译部件";
    for (int i = 0; i < 16; i++) {
      _itoa_s(i, buffer, 10);
      EXPECT_STREQ(str + buffer, c.GetParaName(i));
    }
		CUnitComponent c3; // 默认为可被编译部件
		EXPECT_STREQ("", c3.GetName());
		CRect rect2 = c3.GetSize();
		EXPECT_EQ(rect2.top, 0);
		EXPECT_EQ(rect2.bottom, 0);
		EXPECT_EQ(rect2.left, 0);
		EXPECT_EQ(rect2.right, 0);
    EXPECT_TRUE(c3.IsEncapsulable()) << "可被编译部件";
    for (int i = 0; i < 16; i++) {
      _itoa_s(i, buffer, 10);
      EXPECT_STREQ(str + buffer, c.GetParaName(i));
    }

    CUnitComponent c4(true); //可被编译部件
    EXPECT_TRUE(c4.IsEncapsulable()) << "可被编译部件";
    for (int i = 0; i < 16; i++) {
      _itoa_s(i, buffer, 10);
      EXPECT_STREQ(str + buffer, c.GetParaName(i));
    }

    CUnitComponent c5(false); //不可被编译部件
    EXPECT_FALSE(c5.IsEncapsulable()) << "不可被编译部件";
    for (int i = 0; i < 16; i++) {
      _itoa_s(i, buffer, 10);
      EXPECT_STREQ(str + buffer, c.GetParaName(i));
    }
	}

	TEST(TestCUnitComponent, TestSerialize) {
		CFile cFile1, cFile2;
		char buffer[512];
		CString strFileName = "CUnitComponent.tst", str2 = "Para";
		CUnitComponent * pc = new CUnitComponent, * pc2;
		CPoint pt1(100, 100), pt2(1000, 1000);
		CRect rect(pt1, pt2);
		CUnitBasePtr cp2;

		cFile1.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
		CArchive ar(&cFile1, CArchive::store, 512, buffer);
		char buffer2[100];

		cp2 = new CUnitAdd;

		str2 = "Para";

		for (int i = 0; i < 16; i++) {
			_itoa_s(i, buffer2, 10);
			pc->SetParaName(i, str2 + buffer2);
			pc->SetParaSrcUnit(i, cp2); //
			pc->SetParaType(i, tDOUBLE);
			pc->SetParaDestUnit(i, cp2);
			pc->SetDouble(i, i);
			pc->SetParaSrcIndex(i, i + 1);
			pc->SetParaDestIndex(i, i + 1);
		}
		pc->SetName("Component2");
		ar << pc; // 
		ar.Flush(); // 必须flush，否则有可能没完成存储
		cFile1.Close();
		if (!cFile2.Open(strFileName, CFile::modeRead)) {
			pc->SetParaName(3, "aa");
		}
		CArchive ar2(&cFile2, CArchive::load, 512, buffer);
		ar2 >> pc2;	
		EXPECT_STREQ(pc->GetName(), pc2->GetName());	//由于在读入部件单元时是在系统中重新生成一个部件，故而部件的内存地址是不一样的，只能判断名称是否相同
		EXPECT_STREQ("Para3",pc2->GetParaName(3));
		EXPECT_EQ(pc->GetParaType(5), pc2->GetParaType(5));
		EXPECT_EQ(6, pc2->GetParaSrcIndex(5));
		EXPECT_EQ(6, pc2->GetParaDestIndex(5));

		delete pc;
		delete cp2;	
		cp2 = pc2->GetParaSrcUnit(0);
		delete cp2;
		
		delete pc2;
	}
	
	TEST(TestCUnitComponent, TestSetSelectParaFlag) {

	}

	TEST(TestCUnitComponent, TestGetClassNameStr) {
		CUnitComponent c;
		EXPECT_STREQ("Cpt", c.GetClassNameStr());
	}

	TEST(TestCUnitComponent, TestIsMe) {
		CString str = "CUnitComponent";
		CPoint pt(100, 100);
		CUnitComponent c(str, pt, true);	
		CString str1 = "aaa";
		CUnitAdd * pc2 = new CUnitAdd;
		pc2->SetName("aaa");
		CUnitList * plist;
		
		plist = c.GetUnitList();
		plist->push_back(pc2);
		c.SetViewInFlag(true);
		c.SetName(str);
		EXPECT_TRUE(c.IsMe(str));
		EXPECT_FALSE(c.IsMe("aa"));
		EXPECT_TRUE(c.IsMe("aaa"));
	}

	TEST(TestCUnitComponent, TestExective) {

	}

	TEST(TestCUnitComponent, TestExectiveDynLink) {

	}

	TEST(TestCUnitComponent, TestSetBool) {
		CUnitComponent c;
		CPoint pt(100, 100), pt2(200, 200);
		CUnitAdd * pc = new CUnitAdd("UnitAdd1", pt);
	
		c.SetParaDestIndex(1, 9);			// Add单元参数9是AutoManual，布尔值
		c.SetParaType(1,tBOOL);
		CString str = "UnitAdd1";
		c.SetParaName(1, str);
		c.SetParaDestUnit(1, pc);
		pc->SetBool(9, false);

		c.SetBool(1, true);
		EXPECT_TRUE(c.GetBool(1));
		c.SetBool(1, false);
		EXPECT_FALSE(c.GetBool(1));

		delete pc;
	}

	TEST(TestCUnitComponent, TestSetInteger) {
		CUnitComponent c;
		CPoint pt(100, 100), pt2(200, 200);
		CUnitAdd * pc = new CUnitAdd("UnitAdd1", pt);

		c.SetParaDestIndex(1, 10);		//Add单元参数10的位置是ScanRate，整型数值
		c.SetParaType(1, tWORD);
		CString str = "UnitAdd1";
		c.SetParaName(1, str);
		c.SetParaDestUnit(1, pc);
		
		pc->SetInteger(10, 1);
		c.SetInteger(1, 10000);
		EXPECT_EQ(10000, c.GetInteger(1));

		delete pc;
	}

	TEST(TestCUnitComponent, TestSetDouble) {
		CUnitComponent c;
		CPoint pt(100, 100), pt2(200, 200);
		CUnitAdd * pc = new CUnitAdd("UnitAdd1", pt);

		c.SetParaDestIndex(1, 0);		//Add单元参数0的位置是Input1，浮点型数值
		c.SetParaType(1, tDOUBLE);
		CString str = "UnitAdd1";
		c.SetParaName(1, str); 
		c.SetParaDestUnit(1, pc); 

		pc->SetDouble(0, 1.0);
		c.SetDouble(1, 10000);
		EXPECT_DOUBLE_EQ(10000, c.GetDouble(1));

		delete pc;
	}

  TEST(TestCUnitComponent, TestSetString) {
    CUnitComponent c;
    CPoint pt(100, 100), pt2(200, 200);
    CUnitAdd * pc = new CUnitAdd("UnitAdd1", pt);

    c.SetParaDestIndex(1, 0);		//Add单元参数0的位置是Input1，浮点型数值
    c.SetParaType(1, tSTRING);
    CString str = "UnitAdd1";
    c.SetString(1, str);
    EXPECT_STREQ(str, c.GetString(1));
    c.SetString(1, "abc");
    EXPECT_STREQ("abc", c.GetString(1));
  
    delete pc;
  }

	TEST(TestCUnitComponent, TestGetIndex) {

	}

	TEST(TestCUnitComponent, TestGetDynLinkType1) {
		CUnitComponent c;
		CUnitAdd * pc = new CUnitAdd;
		CUnitAdd * pc2 = new CUnitAdd;

    c.SetParaDestUnit(1, pc);
    c.SetParaDestIndex(1, 9);		// 9的位置为AutoManual，布尔值
    c.SetParaSrcUnit(1, pc2);
		c.SetParaSrcIndex(1, 9);		// 9的位置为AutoManual，布尔值
		c.SetParaType(1, tBOOL);
		EXPECT_EQ(tBOOL, c.GetDynLinkType(1) & tBOOL);
		
		c.SetParaDestUnit(0, pc); 
		c.SetParaDestIndex(0, 0);		// 9的位置为Input1，浮点型数值
		c.SetParaSrcUnit(0, pc2);
		c.SetParaSrcIndex(0, 0);		// 9的位置为Input1，浮点型数值
		c.SetParaType(0, tDOUBLE);
		EXPECT_EQ(tDOUBLE, c.GetDynLinkType(0) & tDOUBLE);

		delete pc;
		delete pc2;

	}

	TEST(TestCUnitComponent, TestAddList) {

	}

	TEST(TestCUnitComponent, TestDeleteDynLink) {

	}

	TEST(TestCUnitComponent, CreateUniName) {

	}

	TEST(TestCUnitComponent, TestSetDeleteDynLinkFlag) {

	}

	TEST(TestCUnitComponent, TestClearDeleteDynLinkFlag) {

	}

	TEST(TestCUnitComponent, TestClearLoopDetectFlag) {

	}

	TEST(TestCUnitComponent, TestCanViewIn) {
		CUnitComponent c;

		if (!c.IsEncapsulated()) {
			if (c.CanViewIn()) {
				c.SetViewInFlag(false);
				EXPECT_FALSE(c.CanViewIn());
				EXPECT_FALSE(c.IsEncapsulated());
			}
			else {
				c.SetViewInFlag(true);
				EXPECT_FALSE(c.IsEncapsulated());
				EXPECT_TRUE(c.CanViewIn());
			}
		}
		else {
			EXPECT_TRUE(c.IsEncapsulated());
			EXPECT_FALSE(c.CanViewIn());
			if (c.CanViewIn()) {
				c.SetViewInFlag(false);
				EXPECT_FALSE(c.CanViewIn());
				EXPECT_FALSE(c.IsEncapsulated());
			}
			else {
				c.SetViewInFlag(true);
				EXPECT_FALSE(c.IsEncapsulated());
				EXPECT_TRUE(c.CanViewIn());
			}
		}
	}


	TEST(TestCUnitComponent, TestPrepareParaDictionary) {

	}
	/*
	TEST(TestCUnitComponent, TestSelectParameter) {
		CUnitComponent c;
		ULONG * pl = c.GetArrayIndex();
		CUnitAdd cc, cc1;

		for (int i = 0; i < 16; i++) {
			c.SetParaSrcUnit(i, &cc1);
			c.SetParaDestUnit(i, &cc);
		}
		for (int i = 0; i < 16; i = i + 4) {
			c.SetParaType(i, tBOOL | tINPUT | tOUTPUT);
			c.SetParaLinkedFlag(i, true);
			c.SetParaType(i + 1, tWORD | tINPUT | tOUTPUT);
			c.SetParaLinkedFlag(i + 1, false);
			c.SetParaType(i + 2, tDOUBLE | tINPUT | tOUTPUT);
			c.SetParaLinkedFlag(i + 2, true);
			c.SetParaType(i + 3, tSTRING | tINPUT | tOUTPUT);
			c.SetParaLinkedFlag(i + 3, false); 
		}
		c.SelectParameter(tINPUT, true); // 选择输入型（tBOOL和tDOUBLE符合）
		for (int i = 0; i < 16; i += 2) {
			EXPECT_EQ(c.GetParaType(pl[i]), tBOOL | tINPUT | tOUTPUT);
			EXPECT_EQ(c.GetParaType(pl[i+1]), tDOUBLE | tINPUT | tOUTPUT);		
			EXPECT_EQ(pl[i], i*2);
		}
		c.SelectParameter(tDOUBLE | tINPUT, true);   // 选择tDOUBLE并且是输入型
		for (int i = 0; i < 8; i++) {
			EXPECT_EQ(c.GetParaType(pl[i]), tDOUBLE | tINPUT | tOUTPUT);
			EXPECT_EQ(pl[i], i * 4 + 2);
		}
	}
	
	TEST(TestCUnitComponent, TestCreateParaDictionary) {
		CUnitComponent c;
		ULONG * pl = c.GetArrayIndex();
		CUnitAdd cc, cc1;

		CDicList CListDic;
		
		for (int i = 0; i < 16; i = i + 4) {
			c.SetParaType(i, tBOOL | tINPUT | tOUTPUT);
			c.SetParaLinkedFlag(i, true);			
			c.SetParaSrcUnit(i, &cc1);
			c.SetParaDestUnit(i, &cc);
			c.SetParaType(i + 1, tWORD | tINPUT | tOUTPUT);
			c.SetParaLinkedFlag(i + 1, false);
			c.SetParaType(i + 2, tDOUBLE | tINPUT | tOUTPUT);
			c.SetParaLinkedFlag(i + 2, true);
			c.SetParaSrcUnit(i + 2, &cc1);
			c.SetParaDestUnit(i + 2, &cc);
			c.SetParaType(i + 3, tSTRING | tINPUT | tOUTPUT);
			c.SetParaLinkedFlag(i + 3, false);
		}

		c.CreateParaDictionary(CListDic, tINPUT, true); // 选择输入型并且已经存在动态链接的参数（tBOOL和tDOUBLE符合）
		EXPECT_EQ(CListDic.GetCount(), 16);
		POSITION po = CListDic.GetHeadPosition();
		CUnitDictionary * pDic;
		for (int i = 0; i < 16; i += 2) {
			pDic = CListDic.GetNext(po);
			EXPECT_EQ(pDic->GetUnit(), &c);
			EXPECT_EQ(pDic->GetType(), tBOOL | tINPUT | tOUTPUT);
			EXPECT_EQ(pDic->GetIndex(), i * 2);
			pDic = CListDic.GetNext(po);
			EXPECT_EQ(pDic->GetUnit(), &c);
			EXPECT_EQ(pDic->GetType(), tDOUBLE | tINPUT | tOUTPUT);
			EXPECT_EQ(pDic->GetIndex(), i*2 + 2);
		}
		po = CListDic.GetHeadPosition();
		int iTotal = CListDic.GetCount();
		for (int i = 0; i < iTotal; i++) {
			pDic = CListDic.GetNext(po);
			delete pDic;
		}

		CListDic.RemoveAll();
		c.CreateParaDictionary(CListDic, tOUTPUT, false); // 选择输出型并且不存在动态链接的参数（tWORD和tSTRING符合）
		EXPECT_EQ(CListDic.GetCount(), 16);
		po = CListDic.GetHeadPosition();
		for (int i = 0; i < 16; i += 2) {
			pDic = CListDic.GetNext(po);
			EXPECT_EQ(pDic->GetUnit(), &c);
			EXPECT_EQ(pDic->GetType(), tWORD | tINPUT | tOUTPUT);
			EXPECT_EQ(pDic->GetIndex(), i*2 + 1);
			pDic = CListDic.GetNext(po);
			EXPECT_EQ(pDic->GetUnit(), &c);
			EXPECT_EQ(pDic->GetType(), tSTRING | tINPUT | tOUTPUT);
			EXPECT_EQ(pDic->GetIndex(), i * 2 + 3);
		}
		po = CListDic.GetHeadPosition();
 		iTotal = CListDic.GetCount();
		for (int i = 0; i < iTotal; i++) {
			pDic = CListDic.GetNext(po);
			delete pDic;
		}
		CListDic.RemoveAll();
		c.CreateParaDictionary(CListDic, tWORD | tOUTPUT, false); // 选择输出型并且不存在动态链接的参数（tWORD符合）
		EXPECT_EQ(CListDic.GetCount(), 8);
		po = CListDic.GetHeadPosition();
		for (int i = 0; i < 8; i++) {
			pDic = CListDic.GetNext(po);
			EXPECT_EQ(pDic->GetUnit(), &c);
			EXPECT_EQ(pDic->GetType(), tWORD | tINPUT | tOUTPUT);
			EXPECT_EQ(pDic->GetIndex(), i * 4 + 1);
		}
		po = CListDic.GetHeadPosition();
		iTotal = CListDic.GetCount();
		for (int i = 0; i < iTotal; i++) {
			pDic = CListDic.GetNext(po);
			delete pDic;
		}
	}

	TEST(TestCUnitComponent, TestGetParaName) {
		CUnitComponent c;
		for (int i = 0; i < 16; i++) {
			c.SetParaName(i, "Parameter");
			EXPECT_STREQ("Parameter", c.GetParaName(i));
		}
	}
*/


  TEST(TestCUnitComponent, TestGetParaSrcUnit) {
    CUnitComponent c;
    CUnitAdd cAdd;
    CUnitBasePtr pc;

    cAdd.SetName("abc");
    EXPECT_ANY_THROW(c.SetParaSrcUnit(-1, &cAdd));
    EXPECT_ANY_THROW(c.SetParaSrcUnit(16, &cAdd));
    c.SetParaSrcUnit(1, &cAdd);
    EXPECT_ANY_THROW(c.GetParaSrcUnit(-1));
    EXPECT_ANY_THROW(c.GetParaSrcUnit(16));
    pc = c.GetParaSrcUnit(1);
    EXPECT_STREQ("abc", pc->GetName());
  }

  TEST(TestCUnitComponent, TestGetParaDestUnit) {
    CUnitComponent c;
    CUnitAdd cAdd;
    CUnitBasePtr pc;

    cAdd.SetName("abc");
    EXPECT_ANY_THROW(c.SetParaDestUnit(-1, &cAdd));
    EXPECT_ANY_THROW(c.SetParaDestUnit(16, &cAdd));
    c.SetParaDestUnit(1, &cAdd);
    EXPECT_ANY_THROW(c.GetParaDestUnit(-1));
    EXPECT_ANY_THROW(c.GetParaDestUnit(16));
    pc = c.GetParaDestUnit(1);
    EXPECT_STREQ("abc", pc->GetName());
  }

  TEST(TestCUnitComponent, TestGetParaSrcIndex) {
    CUnitComponent c;
    CUnitAdd cAdd;

    EXPECT_ANY_THROW(c.SetParaSrcIndex(-1, 12));
    EXPECT_ANY_THROW(c.SetParaSrcIndex(16, 15));
    EXPECT_ANY_THROW(c.SetParaSrcIndex(-4, 12));
    EXPECT_ANY_THROW(c.SetParaSrcIndex(30, 15));
    for (int i = 0; i < 16; i++) {
      c.SetParaSrcIndex(i, 50 * i);
    }
    EXPECT_ANY_THROW(c.GetParaSrcIndex(-1));
    EXPECT_ANY_THROW(c.GetParaSrcIndex(16));
    EXPECT_ANY_THROW(c.GetParaSrcIndex(-100));
    EXPECT_ANY_THROW(c.GetParaSrcIndex(1600));
    for (int i = 0; i < 16; i++) {
      EXPECT_EQ(50 * i, c.GetParaSrcIndex(i));
    }
  }

  TEST(TestCUnitComponent, TestGetParaDestIndex) {
    CUnitComponent c;
    CUnitAdd cAdd;

    EXPECT_ANY_THROW(c.SetParaDestIndex(-1, 10));
    EXPECT_ANY_THROW(c.SetParaDestIndex(16, 10));
    c.SetParaDestIndex(1, 100);
    EXPECT_ANY_THROW(c.GetParaDestIndex(-1));
    EXPECT_ANY_THROW(c.GetParaDestIndex(16));
    EXPECT_EQ(100, c.GetParaDestIndex(1));
  }  
  
  TEST(TestCUnitComponent, TestGetParaType) {
    CUnitComponent c;
    CUnitAdd cAdd;

    EXPECT_ANY_THROW(c.SetParaType(-1, 10));
    EXPECT_ANY_THROW(c.SetParaType(16, 10));
    for (int i = 0; i < 16; i++) {
      c.SetParaType(i, tINPUT | tDOUBLE);
      EXPECT_EQ(tINPUT | tDOUBLE, c.GetParaType(i));
    }
    EXPECT_ANY_THROW(c.GetParaType(-1));
    EXPECT_ANY_THROW(c.GetParaType(16));
  }  
  
  TEST(TestCUnitComponent, TestGetDynLinkType) {
    CUnitComponent c;
    CUnitAdd cAdd;

    EXPECT_ANY_THROW(c.SetParaType(-1, tOUTPUT | tDOUBLE));
    EXPECT_ANY_THROW(c.SetParaType(16, tOUTPUT | tDOUBLE));
    for (int i = 0; i < 16; i++) {
      c.SetParaType(i, tINPUT | tDOUBLE);
      EXPECT_EQ(tDOUBLE, c.GetDynLinkType(i));
    }
    EXPECT_ANY_THROW(c.GetDynLinkType(-1));
    EXPECT_ANY_THROW(c.GetDynLinkType(16));
  }



	TEST(TestCUnitComponent, TestPrepareRunTimeList) {
	
	}
	
	TEST(TestCUnitComponent, TestResetComileFlag) {
	
	}
	
	TEST(TestCUnitComponent, TestGetUnitList) {
	
	}
	
	TEST(TestCUnitComponent, TestCheckCutOff) {
	
	}
	
	TEST(TestCUnitComponent, TestLinkParameterIntoComponent) {
	
	}

	TEST(TestCUnitComponent, TestHaveParameter) {
		CUnitComponent c;

		for (int i = 0; i < 16; i++) {
			c.SetParaLinkedFlag(i, false);	
		}
		EXPECT_FALSE(c.HaveParameter());
		c.SetParaLinkedFlag(12, true);
		EXPECT_TRUE(c.HaveParameter());
	}
	TEST(TestCUnitComponent, TestGetParaUnitListAddress) {
	
	}
	
	TEST(TestCUnitComponent, TestGetParaNameAdr) {
	
	}
	TEST(TestCUnitComponent, TestUnitListLoopDetect) {
	
	}

  TEST(TestCUnitComponent, TestSetInnerDataLinkedFlag) {
    CUnitComponent c;

    for (int i = 0; i < 16; i++) {
      EXPECT_FALSE(c.IsParaLinked(i));
    }
    for (int i = 0; i < 16; i++) {
      c.SetParaLinkedFlag(i, true);
      EXPECT_TRUE(c.IsParaLinked(i));
      c.SetParaLinkedFlag(i, false);
      EXPECT_FALSE(c.IsParaLinked(i));
    }
  }


}

