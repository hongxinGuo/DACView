#include"stdafx.h"

#include"pch.h"

#include"CompileUnitList.h"

#include"CUnitBase.h"
#include"CUnitComponent.h"

#include"SQIFileDoc.h"

namespace DACViewTestCUnitComponent {

  class TestCUnitComponentSetInnerDataLinkFlag : public::testing::TestWithParam<CString> {
  };

  INSTANTIATE_TEST_CASE_P(TestSetInnerDataLinkFlag, TestCUnitComponentSetInnerDataLinkFlag,
    testing::Values("c:\\DACViewTestFile\\UnitComponent\\TestSetInnerDataLinkFlag.sqi"
      , "c:\\DACViewTestFile\\UnitComponent\\TestSetInnerDataLinkFlag2.sqi"
      //, "c:\\DACViewTestFile\\UnitComponent\\TestLoopDetect3.sqi"
      //, "c:\\DACViewTestFile\\UnitComponent\\TestLoopDetect4.sqi"
    ));

  TEST_P(TestCUnitComponentSetInnerDataLinkFlag, TestSetInnerDataLinkFlag) {
    CString strFileName = GetParam();

    CFile cFile;
    char buffer[512];

    CString strFileHeader;
    CUnitList unitlist, rtUnitList;

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

    CompileUnitList(&unitlist, &rtUnitList);

    for (const auto pctemp : unitlist) {
      if (pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        CUnitComponentPtr pCpt = dynamic_pointer_cast<CUnitComponent>(pctemp);
        EXPECT_FALSE(pCpt->IsCutoff()) << "参数有内部数据链接的截断标志在执行任务后未设置为假";
      }
    }

    // release list's memory
    rtUnitList.clear();
    unitlist.clear();
  }

}
 