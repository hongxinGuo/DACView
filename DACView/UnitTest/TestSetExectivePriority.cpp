/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 设置单元序列的运行时序。
//
//
//
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include"stdafx.h"
#include "gtest/gtest.h"

#include"CompileUnitList.h"

#include"CUnitBase.h"
#include"CUnitComponent.h"

namespace DACViewTest {

  class TestCUnitBaseSetExectivePriority : public::testing::TestWithParam<CString>
  {
    void SetUp() {
      int a = 0;
    }

  };

  INSTANTIATE_TEST_CASE_P(TestSetExectivePriority, TestCUnitBaseSetExectivePriority,
    testing::Values("C:\\DACViewTestFile\\TestUnitBaseSetExectivePriority1.sqi"
      //,"C:\\DACViewTestFile\\TestUnitBaseSetExectivePriority1.sqi"
    ));

  TEST_P(TestCUnitBaseSetExectivePriority, TestSetExectivePriority) {
    CString strFileName = GetParam();

    CFile cFile;
    char buffer[512];

    CString strFileHeader;
    CUnitList unitlist;
    CUnitBase * pctemp, *pcunitTemp;
    INT64 i, iTotal;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;

    if (!cFile.Open(strFileName, CFile::modeRead)) {
      EXPECT_FALSE(true) << "read file %s failed" << strFileName;
    }
    CArchive ar(&cFile, CArchive::load, 512, buffer);
    ar >> strFileHeader >> iCurrentUnit >> iTotal;
    for (i = 0; i < iTotal; i++) {
      ar >> pctemp;
      if (pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)pctemp;
        EXPECT_FALSE(pCpt->IsEncapsulated()) << "before compile component";
      }
      unitlist.AddTail(pctemp);
      pctemp->SetUpperUnitList(&unitlist);
    }

    ReSetCompileFlag(&unitlist);

    SetParaLockFlag(&unitlist);

    CUnitList rtUnitList;

    EXPECT_FALSE(UnitListLoopDetect(&unitlist));  // 如果没有发现动态链接循环
    CompileUnitList(&unitlist, &rtUnitList);

    POSITION po = unitlist.GetHeadPosition();
    iTotal = unitlist.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pctemp = unitlist.GetNext(po);
      if (pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)pctemp;
        if (pCpt->IsEncapsulable()) {
          EXPECT_TRUE(pCpt->IsEncapsulated()) << "after component compiled";
        }
      }
    }

    po = unitlist.GetHeadPosition();
    POSITION poDL;
    CUnitBase * pcunittemp;
    CString strName;
    CUDLList * pDLList;
    CUnitDynLink * pUnitDynLink;
    iTotal = unitlist.GetCount();
    INT32 a, b;
    for (i < 0; i < iTotal; i++) {
      pcunittemp = unitlist.GetNext(po);
      pDLList = pcunittemp->GetDynLinkList();
      poDL = pDLList->GetHeadPosition();
      pUnitDynLink = pDLList->GetNext(po);
      a = pcunittemp->GetExectivePriority();
      b = pUnitDynLink->GetDestUnit()->GetExectivePriority();
      EXPECT_LE(a, b) << "Found Error in File " << strFileName << "'s SetExectivePriority's Unit "
        << pcunittemp->GetName() << " -> " << pUnitDynLink->GetDestUnit()->GetName();
    }

    // clearup
    POSITION Po = unitlist.GetHeadPosition();
    iTotal = unitlist.GetCount();
    for (i = 0; i < iTotal; i++) {
      pcunitTemp = unitlist.GetNext(Po);
      delete pcunitTemp;
      pcunitTemp = nullptr;
    }
    // release list's memory
    unitlist.RemoveAll();
    rtUnitList.RemoveAll();
  }

}