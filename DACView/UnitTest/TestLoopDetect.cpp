/////////////////////////////////////////////////////////////////////////////////
//
// 将所有的LoopDetect函数的测试都放在这个文件中。
// 共有四个。三个类LoopDetect：CUnitBase、CUnitComponent和CUnitComponent，还有一个全局函数UnitListLoopDetect
// 采用参数化和事件机制两种方式测试这三个函数
//
//////////////////////////////////////////////////////////////////////////////////
#include"stdafx.h"
#include"pch.h"

#include"CompileUnitList.h"

#include"CUnitBase.h"
#include"CUnitComponent.h"

#include"SQIFileDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

class DACViewTestEnvironment : public testing::Test {
public:
	void SetUp()
	{
		int a = 0;
	}
	void TearDown()
	{

	}
		
};

TEST_F(DACViewTestEnvironment, TESt1) {
	int a = 0;
}

namespace DACViewTestLoopDetect {

	class TestLoopDetect : public::testing::TestWithParam<CString> {
	};

	INSTANTIATE_TEST_CASE_P(TestCUnitComponent, TestLoopDetect,
		testing::Values("c:\\DACViewTestFile\\LoopDetect\\简单单元.sqi"
			, "c:\\DACViewTestFile\\LoopDetect\\简单单元联入未封装的部件.sqi"
			, "c:\\DACViewTestFile\\LoopDetect\\未封装部件之间互联.sqi"
      , "c:\\DACViewTestFile\\LoopDetect\\未封装部件之间互联2.sqi"
      , "c:\\DACViewTestFile\\LoopDetect\\未封装部件与封装部件之间互联2.sqi"
      , "c:\\DACViewTestFile\\LoopDetect\\封装部件与封装部件之间互联1.sqi"
      , "c:\\DACViewTestFile\\LoopDetect\\未封装部件之间互联（嵌套四层）.sqi"

		));
  
	TEST_P(TestLoopDetect, TestUnitComponentLoopDetect) {
		CString strFileName = GetParam();

		CFile cFile;
		char buffer[512];

		CString strFileHeader;
		CUnitList unitlist;
    CObjectList objectlist;
		CUnitBase * pctemp, *pcunitTemp;
		INT64 i, iTotal;
		INT64 iCurrentUnit;

		if (!cFile.Open(strFileName, CFile::modeRead)) {
			EXPECT_FALSE(true) << "read file %s failed" << strFileName;
		}
		CArchive ar(&cFile, CArchive::load, 512, buffer);
		ar >> strFileHeader >> iCurrentUnit >> iTotal;
		for (i = 0; i < iTotal; i++) {
			ar >> pctemp;
			unitlist.push_back(pctemp);
		}
		ar.Flush();
		cFile.Close();

    ReSetCompileFlag(&unitlist);

    SetParaLockFlag(&unitlist, &objectlist);

		CUnitList rtUnitList;

		EXPECT_TRUE(UnitListLoopDetect(&unitlist));

		// clearup
		for (auto pcunitTemp : unitlist) {
			delete pcunitTemp;
		}
		// release list's memory
		rtUnitList.clear();
		unitlist.clear();
	}

  /*

	TEST_P(TestLoopDetect, TestUnitBaseLoopDetect) {
		CString strFileName = GetParam();

		CFile cFile;
		char buffer[512];

		CString strFileHeader;
		CUnitList unitlist;
		CUnitBase * pcunit = nullptr;
		INT64 i, iTotal;
		INT64 iCurrentUnit;

		if (!cFile.Open(strFileName, CFile::modeRead)) {
			EXPECT_FALSE(true) << "read file %s failed" << strFileName;
		}
		CArchive ar(&cFile, CArchive::load, 512, buffer);
		ar >> strFileHeader >> iCurrentUnit >> iTotal;
		for (i = 0; i < iTotal; i++) {
			ar >> pcunit;
			unitlist.AddTail(pcunit);
		}
		ar.Flush();
		cFile.Close();

    ReSetCompileFlag(&unitlist);

    SetParaLockFlag(&unitlist);

		CUnitList rtUnitList;

		poUnit = unitlist.GetHeadPosition();
		for (i = 0; i < iTotal; i++) {
			pcunit = unitlist.GetNext(poUnit);
			EXPECT_FALSE(pcunit->CheckCutOff(&rtUnitList)) << "looking for CutOff in " << strFileName << " failed"; // 检查是否存在截断标志，应该不存在。
			EXPECT_TRUE(pcunit->LoopDetect(&rtUnitList)) << "Loop detect in" << strFileName << " failed"; // 检查是否存在循环，应该存在。
		}
		// clearup
		POSITION Po = unitlist.GetHeadPosition();
		iTotal = unitlist.GetCount();
		for (i = 0; i < iTotal; i++) {
			pcunit = unitlist.GetNext(Po);
			delete pcunit;
			pcunit = nullptr;
		}
		// release list's memory
		rtUnitList.RemoveAll();
		unitlist.RemoveAll();
	}
  */

	TEST_P(TestLoopDetect, TestUnitListLoopDetect) {
		CString strFileName = GetParam();

		CFile cFile;
		char buffer[512];

		CString strFileHeader;
		CUnitList unitlist;
		CUnitBase *pcunitTemp;
		INT64 i, iTotal;

		if (!cFile.Open(strFileName, CFile::modeRead)) {
			EXPECT_FALSE(true) << "read file %s failed" << strFileName;
		}
		CArchive ar(&cFile, CArchive::load, 512, buffer);
    INT32 iObjectNumber = 0;
    INT64 iUnitNumber;
    CObjectList objectlist;
    LoadSQIFile(ar, &unitlist, &objectlist, &iUnitNumber, &iObjectNumber);

    ReSetCompileFlag(&unitlist);

    SetParaLockFlag(&unitlist, &objectlist);

		CUnitList rtUnitList;

		EXPECT_TRUE(UnitListLoopDetect(&unitlist));

		// clearup
		for (auto pcunitTemp : unitlist) {
			delete pcunitTemp;
			pcunitTemp = nullptr;
		}
		// release list's memory
		rtUnitList.clear();
		unitlist.clear();
	}

};
