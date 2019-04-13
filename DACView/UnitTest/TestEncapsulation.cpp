#include"stdafx.h"
#include "gtest/gtest.h"

#include"CompileUnitList.h"

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


  class TestCUnitComponentEncapsulation : public::testing::TestWithParam<CString>
  {
  };

  INSTANTIATE_TEST_CASE_P(TestEncapsulation, TestCUnitComponentEncapsulation, testing::Values(
      "c:\\DACViewTestFile\\CompileUnitList\\可封装部件.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套两层.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层参数有链接.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层部件之间链接参数有链接.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\封装后的部件.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层参数有链接有封装后的部件.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层参数有链接有封装后的多个部件.sqi"
  ));

  TEST_P(TestCUnitComponentEncapsulation, TestComponentEncapsulation) {
    CString strFileName = GetParam();
    CFile cFile;
    char buffer[512];
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);
    CUnitBase *cp2;
    CString strFileHeader;
    CUnitBase * pctemp, *pcunitTemp;
    INT64 iTotal;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;
    CUnitList unitlist, unitListRunTime;

    if (!cFile.Open(strFileName, CFile::modeRead)) {
      EXPECT_FALSE(true) << "read file " << strFileName << "  failed";
    }
    CArchive ar(&cFile, CArchive::load, 512, buffer);
    ar >> strFileHeader >> iCurrentUnit >> iTotal;

    for (int i = 0; i < iTotal; i++) { 
      ar >> pctemp;
      unitlist.AddTail(pctemp);
      pctemp->SetUpperUnitList(&unitlist);
    }

    ReSetCompileFlag(&unitlist);

    SetParaLockFlag(&unitlist);

    // 所有与编译有关的测试，都需要编译整体文件。由于系统数据关联的原因，无法单独部件本身，故而需要编译整体文件，最后再测试封装。
    CompileUnitList(&unitlist, &unitListRunTime);

    POSITION poUnit = unitlist.GetHeadPosition();
    for (int i = 0; i < iTotal; i++) {
      pcunitTemp = unitlist.GetNext(poUnit);
      if (pcunitTemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)pcunitTemp;
        if (pCpt->IsEncapsulable()) {
          for (int j = 0; j < 16; j++) {
            if (pCpt->IsParaLinked(i)) {
              // 顺道测试此函数。
              EXPECT_TRUE((pCpt->GetDynLinkType(i) & pCpt->GetParaType(i)));
            }
          }
          EXPECT_TRUE(pCpt->IsEncapsulated());
          EXPECT_TRUE(pCpt->HaveParameter());
        }
      }
    }

    POSITION po = unitlist.GetHeadPosition();
    for (int i = 0; i < iTotal; i++) {
      cp2 = unitlist.GetNext(po);
      delete cp2;
    }
    // delete pCpt;
    unitlist.RemoveAll();
  }

  ///////////////////////////////////////////////////////////////////////////////////////
  //
  // 所有与封装有关的测试都在这里，按部就班地一步步测试验证。
  //
  ////////////////////////////////////////////////////////////////////////////////////////
  TEST_P(TestCUnitComponentEncapsulation, TestEncapsulation1) {
    CString strFileName = GetParam();

    CFile cFile;
    char buffer[512];

    CString strFileHeader;
    CUnitList unitlist, listTotalUnit, runtimeUnitList, *pUnitList3;
    CUnitBase * pctemp, *pcunitTemp, *pcunit;
    INT64 i, iTotal, jTotal, lTotal;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;
    CUDLList * pDLList = nullptr, *pDLList2 = nullptr;
    CUnitDynLink * pDL = nullptr, *pDL2 = nullptr;
    CUnitComponent * pSrcComponent = nullptr, *pDestComponent = nullptr;
    CUnitComponent * pCUCP = nullptr;

    if (!cFile.Open(strFileName, CFile::modeRead)) {
      EXPECT_FALSE(true) << "read file %s failed" << strFileName;
    }
    CArchive ar(&cFile, CArchive::load, 512, buffer);
    ar >> strFileHeader >> iCurrentUnit >> iTotal;
    for (int i = 0; i < iTotal; i++) {
      ar >> pctemp;
      unitlist.AddTail(pctemp);
      pctemp->SetUpperUnitList(&unitlist);
    }

    // 设置编译所需之前置数据
    ReSetCompileFlag(&unitlist);

    SetParaLockFlag(&unitlist);

    CreateUniUnitList(&unitlist, listTotalUnit);

    // 计算从部件输入和输出的动态链接数据的数量，以备封装部件时测试接口参数的设置是否正确
    POSITION poUnit3, poDL, poDL2, poUnit = listTotalUnit.GetHeadPosition();
    iTotal = listTotalUnit.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pcunitTemp = listTotalUnit.GetNext(poUnit);
      pDLList = pcunitTemp->GetDynLinkList();
      jTotal = pDLList->GetCount();
      POSITION poDL = pDLList->GetHeadPosition();
      for (int j = 0; j < jTotal; j++) {
        pDL = pDLList->GetNext(poDL);
        switch (pDL->GetDynLinkClass()) {
        case COMPONENT_TO_UNIT:
          pSrcComponent = pDL->GetSrcUnit()->GetComponentUpper();
          ASSERT(pSrcComponent != nullptr);
          if (pSrcComponent->IsEncapsulable()) pSrcComponent->m_lOutputParaNumber++;
          break;
        case UNIT_TO_COMPONENT:
          pDestComponent = pDL->GetDestUnit()->GetComponentUpper();
          ASSERT(pDestComponent != nullptr);
          if (pDestComponent->IsEncapsulable())pDestComponent->m_lInputParaNumber++;
          break;
        case COMPONENT_TO_COMPONENT:
          pSrcComponent = pDL->GetSrcUnit()->GetComponentUpper();
          ASSERT(pSrcComponent != nullptr);
          if (pSrcComponent->IsEncapsulable())pSrcComponent->m_lOutputParaNumber++;
          pDestComponent = pDL->GetDestUnit()->GetComponentUpper();
          ASSERT(pDestComponent != nullptr);
          if (pDestComponent->IsEncapsulable())pDestComponent->m_lInputParaNumber++;
          break;
        default:
          break;
        } // switch 
      }
    }
    //再加上计算部件本身参数的输入输出参数个数，就得出部件总共的输入输出数据的数量
    poUnit = listTotalUnit.GetHeadPosition();
    iTotal = listTotalUnit.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pcunitTemp = listTotalUnit.GetNext(poUnit);
      if (pcunitTemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCUCP = (CUnitComponent *)pcunitTemp;
        if (pCUCP->IsEncapsulable()) {
          for (int j = 0; j < 16; j++) {
            if (pCUCP->IsParaLinked(j)) {
              if ((pCUCP->GetParaType(j) & (tINPUT | tOUTPUT)) == tOUTPUT) {
                pCUCP->m_lOutputParaNumber++;
              }
              else {
                pCUCP->m_lInputParaNumber++;
              }
            }
          }
        }
      }
    }

    SetNoSrcUnitExectivePriority(&listTotalUnit);

    ExectiveCompilation(listTotalUnit, &runtimeUnitList);

    // 将Encapsulation展开，分段执行和测试
    poUnit = unitlist.GetHeadPosition();
    iTotal = unitlist.GetCount();
    CUnitList * pUnitList;
    POSITION po;
    CUnitBase * punit;
    for (int i = 0; i < iTotal; i++) {
      pcunit = unitlist.GetNext(poUnit);
      if (pcunit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCUCP = (CUnitComponent *)pcunit;
        if (pCUCP->IsEncapsulable()) { // 可封装部件的动作
          if (pCUCP->IsEncapsulated()) continue;
          // 首先封装下层部件
          pCUCP->EncapsulateBelowComponent(listTotalUnit);

          // 验证下层部件都封装了（目前只是验证下面一层，再往下的尚未能测试，只能靠最后的测试所有的部件都封装了来间接验证）
          pUnitList = pCUCP->GetUnitList();
          po = pUnitList->GetHeadPosition();
          jTotal = pUnitList->GetCount();
          for (int j = 0; j < jTotal; j++) {
            punit = pUnitList->GetNext(po);
            if (punit->IsEncapsulable()) {
              EXPECT_TRUE(punit->IsEncapsulated()) << "下面一层的部件都封装了";
            }
          }

          // 检查部件状态是否正确， 此函数本身就是用来验证的，不需要再次检验
          pCUCP->CheckComponentSelf();

          // 生成内部单元序列的运行时单元序列。由于之前下层部件都已经封装了，此时的运行时单元序列的个数就是内部单元的个数。
          pCUCP->CreateRunTimeUnitList();

          // 测试生成的运行时单元序列与内部单元序列的数量是相同的（封装后既是），且其执行优先级按升序排列（允许相同级别）
          CUnitList * pRTUnitList = pCUCP->GetRunTimeUnitList();
          EXPECT_EQ(pUnitList->GetCount(), pRTUnitList->GetCount()) << "下层部件封装后的运行时单元序列，其单元数量与内部单元序列的相等";
          POSITION poRT = pRTUnitList->GetHeadPosition();
          INT64 kTotal = pRTUnitList->GetCount();
          CUnitBase * punitTemp3, *punitTemp2 = pRTUnitList->GetNext(poRT);
          if (kTotal > 1) {
            for (int k = 0; k < kTotal - 1; k++) {
              punitTemp3 = pRTUnitList->GetNext(poRT);
              EXPECT_LE(punitTemp2->GetExectivePriority(), punitTemp3->GetExectivePriority()) << "单元序列封装后其执行优先级升序排列";
              punitTemp2 = punitTemp3;
            }
          }

          // 将部件参数中输出型的在内部源单元处建立一个新的动态链接，目的单元为本部件。
          // 此步骤要先于处理联入联出的过程执行。
          pCUCP->CreateNewDynLinkFromInterfaceOutputTypePara();

          // 测试输出型参数生成新动态链接的情况
          for (int l = 0; l < 16; l++) {
            if (pCUCP->IsParaLinked(l)) {
              if ((pCUCP->GetParaType(l) & (tINPUT | tOUTPUT)) == tOUTPUT) {
                CUnitBase * punit11 = pCUCP->GetParaSrcUnit(l);
                CUDLList * pUDLList = punit11->GetDynLinkList();
                kTotal = pUDLList->GetCount();
                POSITION poDL1 = pUDLList->GetHeadPosition();
                CUnitDynLink * pDL = pUDLList->GetNext(poDL1);
                while (pDL->GetDestUnit() != pCUCP) {
                  pDL = pUDLList->GetNext(poDL1);
                }
                EXPECT_EQ(pDL->GetDestUnit(), pCUCP) << "输出型参数生成新动态链接时其目的单元就是本部件";
                EXPECT_EQ(pDL->GetSrcUnit(), punit11);
                EXPECT_EQ(pDL->GetDestIndex(), l);
                EXPECT_EQ(pDL->GetSrcIndex(), pCUCP->GetParaSrcIndex(l));
                EXPECT_EQ(pDL->GetDynLinkType(), pCUCP->GetDynLinkType(l));
                EXPECT_EQ(pDL->GetDynLinkClass(), UNIT_TO_UNIT) << "生成的新动态链接其类型为UNIT_TO_UNIT";
                EXPECT_EQ(pCUCP->GetParaDestIndex(l), -1) << "输出型参数的目的索引不需设置";
                EXPECT_EQ(pCUCP->GetParaDestUnit(l), nullptr) << "输出型参数的目的单元为空";
              }
            }
          }

          ////////////////////////////////////////////////////////////////////////////////////////////////////
          // 处理接口参数，将内部联出的和外部联入的动态链接连接到部件参数中

          // 寻找是否存在联入本部件的动态链接
          pCUCP->HandleTheDynLinkedInComponent(listTotalUnit);

          // 测试输入型参数的封装情况
          for (int k = 0; k < 16; k++) {
            if (pCUCP->IsParaLinked(k)) {
              if (((pCUCP->GetParaType(k) & (tINPUT | tOUTPUT)) == tINPUT) && (pCUCP->GetParaSrcUnit(k) != nullptr)) {
                CUDLList *pDLList = pCUCP->GetParaSrcUnit(k)->GetDynLinkList();
                INT64 lTotal = pDLList->GetCount();
                POSITION poDL = pDLList->GetHeadPosition();
                CUnitDynLink * pDL11;
                for (int l = 0; l < lTotal; l++) {
                  pDL11 = pDLList->GetNext(poDL);
                  if (pDL11->GetDestUnit() == pCUCP) { // 找到源单元的动态链接了
                    CUnitBase * pSrcUnit = pDL11->GetSrcUnit();
                    if (pSrcUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
                      EXPECT_EQ(pDL11->GetDynLinkClass(), UNIT_TO_UNIT) << "源单元为部件的其动态链接类型为COMPONENT_TO_COMPONENT";
                    }
                    else if (unitlist.Find(pSrcUnit->GetComponentUpper())) { // 包含单元的部件位于本单元序列中
                      EXPECT_EQ(pDL11->GetDynLinkClass(), COMPONENT_TO_UNIT) << "包含源单元的部件位于最上层单元序列中";
                      if (pSrcUnit->GetComponentUpper()->IsEncapsulable()) {
                        EXPECT_FALSE(pSrcUnit->GetComponentUpper()->IsEncapsulated()) << "部件尚未封装";
                      }
                    }
                    else {
                      EXPECT_EQ(pDL11->GetDynLinkClass(), UNIT_TO_UNIT) << "源单元为简单单元";
                      EXPECT_EQ(pSrcUnit->GetComponentUpper(), nullptr) << "源单元位于最上层，没有部件包含它";
                    }
                  }
                }
              }
            }
          }

          // 设置本部件的执行优先级
          pCUCP->SetMyselfExectivePriority();

          // 测试是否设置好了本部件的执行优先级
          INT32 iComponentExectivePriority = 0;
          for (int i = 0; i < 16; i++) {
            if (pCUCP->IsParaLinked(i)) {
              if (((pCUCP->GetParaType(i)) & tINPUT) && (pCUCP->IsParameterLocked(i))) { // 输入型参数且存在源单元：只从上层单元序列中找
                if ((pCUCP->GetParaSrcUnit(i))->GetExectivePriority() > iComponentExectivePriority) {
                  iComponentExectivePriority = (pCUCP->GetParaSrcUnit(i))->GetExectivePriority();
                }
              }
            }
          }
          EXPECT_EQ(iComponentExectivePriority + 1, pCUCP->GetExectivePriority());

          EXPECT_EQ((pCUCP->GetDynLinkList())->GetCount(), 0); // 处理输出型动态链接前，部件本身的动态链接序列是零
          // 寻找是否存在联出本部件的动态链接。
          // 在本部件中生成一个新的动态链接，将联出的动态链接赋予此新的动态链接，其源单元改为本部件，
          // 并将内部源单元的目的单元设置为本部件，重置部件参数中的目的单元指针和目的参数索引。
          pCUCP->HandleTheDynLinkedfromComponent();

          // 测试处理输出型动态链接是否正确
          bool fFound = false;
          pUnitList3 = pCUCP->GetUnitList();
          poUnit3 = pUnitList3->GetHeadPosition();
          lTotal = pUnitList3->GetCount();
          for (int l = 0; l < lTotal; l++) {
            pcunitTemp = pUnitList3->GetNext(poUnit3);
            pDLList = pcunitTemp->GetDynLinkList();
            poDL = pDLList->GetHeadPosition();
            jTotal = pDLList->GetCount();
            for (int j = 0; j < jTotal; j++) {
              pDL = pDLList->GetNext(poDL);
              if (pDL->GetDestUnit() == pCUCP) {
                if (pDL->mTest_pDestUnitSaved != nullptr) {
                  pDLList2 = pCUCP->GetDynLinkList();
                  poDL2 = pDLList2->GetHeadPosition();
                  kTotal = pDLList2->GetCount();
                  for (int k = 0; k < kTotal; k++) {
                    pDL2 = pDLList2->GetNext(poDL2);
                    if (pDL2->GetDestUnit() == pDL->mTest_pDestUnitSaved) {
                      fFound = true;
                      break;
                    }
                  }
                  if (!fFound) {
                    EXPECT_FALSE(1); // 没找到拆分后的目的单元，有错误
                  }
                  else {
                    EXPECT_EQ(pDL->GetDynLinkType(), pDL2->GetDynLinkType());
                    if (pDL->GetDynLinkClass() == COMPONENT_TO_UNIT) {
                      EXPECT_EQ(pDL2->GetDynLinkClass(), UNIT_TO_UNIT);
                    }
                    else {
                      EXPECT_EQ(pDL2->GetDynLinkClass(), UNIT_TO_COMPONENT) << "处理后由COMPONENT_TO_COMPONENT变为UNIT_TO_COMPONENT";
                    }
                  }
                }
              }
            }
          }

          // 寻找是否内部两参数之间有数据链接，并相应设置部件本身的截断标志.此时尚未设置封装标志
          // 这个函数调用必须是最后一步，否则内部动态链接尚未设置好，结果不对。
          pCUCP->SetInnerDataLinkFlag();

          // 测试。就差这步了，但如何测试呢？

          pCUCP->SetCompiledFlag(true);
          pCUCP->SetEncapsulateFlag(true);
        }
        else { // 不可封装部件的动作
          // 首先封装下层部件
          pCUCP->EncapsulateBelowComponent(listTotalUnit);

          // 测试是否下层部件都封装了
          pUnitList = pCUCP->GetUnitList();
          po = pUnitList->GetHeadPosition();
          jTotal = pUnitList->GetCount();
          for (int j = 0; j < jTotal; j++) {
            punit = pUnitList->GetNext(po);
            if (punit->IsEncapsulable()) {
              EXPECT_TRUE(punit->IsEncapsulated()) << "下面一层的部件都封装了";
            }
          }

          // 检查部件状态是否正确
          pCUCP->CheckComponentSelf();

          // 生成内部单元序列的运行时单元序列
          pCUCP->CreateRunTimeUnitList();

          // 测试是否生成的运行时单元序列个数与整个单元序列的个数相同，且按升序排列
          CUnitList * pRTUnitList = pCUCP->GetRunTimeUnitList();
          EXPECT_EQ(pUnitList->GetCount(), pRTUnitList->GetCount()) << "下层部件封装后的运行时单元序列，其单元数量与内部单元序列的相等";
          POSITION poRT = pRTUnitList->GetHeadPosition();
          INT64 kTotal = pRTUnitList->GetCount();
          CUnitBase * punitTemp3, *punitTemp2 = pRTUnitList->GetNext(poRT);
          if (kTotal > 1) {
            for (int k = 0; k < kTotal - 1; k++) {
              punitTemp3 = pRTUnitList->GetNext(poRT);
              EXPECT_LE(punitTemp2->GetExectivePriority(), punitTemp3->GetExectivePriority()) << "单元序列封装后其执行优先级升序排列";
              punitTemp2 = punitTemp3;
            }
          }
        }
      }
    }

    // 测试可封装部件都封装了（除不可封装的部件外）
    po = listTotalUnit.GetHeadPosition();
    jTotal = listTotalUnit.GetCount();
    for (int j = 0; j < jTotal; j++) {
      punit = listTotalUnit.GetNext(po);
      if (punit->IsEncapsulable()) { // 可封装部件？
        EXPECT_TRUE(punit->IsEncapsulated()) << "可封装部件应该已经都被封装了";
      }
    }

    EXPECT_EQ(runtimeUnitList.GetCount(), listTotalUnit.GetCount()) << "运行时单元序列与所有单元的数量不符";

    poUnit = listTotalUnit.GetHeadPosition();
    iTotal = listTotalUnit.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pcunitTemp = listTotalUnit.GetNext(poUnit);
      if (pcunitTemp->GetExectivePriority() == 1) {
        EXPECT_TRUE(!pcunitTemp->IsHaveSourceUnit()) << "执行优先级为1的单元没有数据输入";
      }
      if (pcunitTemp->GetExectivePriority() > 1) {
        EXPECT_TRUE(pcunitTemp->IsHaveSourceUnit());
      }
      EXPECT_GT(pcunitTemp->GetExectivePriority(), 0) << "执行优先级大于1的单元有源单元（数据输入）";

      if (pcunitTemp->IsEncapsulable()) {
        EXPECT_TRUE(pcunitTemp->IsEncapsulated()) << "封装部件完成";
      }
    }

    //测试部件的输入输出参数个数。
    poUnit = listTotalUnit.GetHeadPosition();
    iTotal = listTotalUnit.GetCount();
    INT64 lSrc = 0, lDest = 0;
    for (int i = 0; i < iTotal; i++) {
      pcunitTemp = listTotalUnit.GetNext(poUnit);
      if (pcunitTemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        lSrc = 0;
        lDest = 0;
        pCUCP = (CUnitComponent *)pcunitTemp;
        if (pCUCP->IsEncapsulable()) {
          for (int j = 0; j < 16; j++) {
            if (pCUCP->IsParaLinked(j)) {
              if ((pCUCP->GetParaType(j) & (tINPUT | tOUTPUT)) == tOUTPUT) {
                lSrc++;
              }
              else {
                lDest++;
              }
            }
          }
          EXPECT_EQ(lSrc, pCUCP->m_lOutputParaNumber) << "输出型参数 = 输出型动态链接 + 尚未链接的输出型参数";
          EXPECT_EQ(lDest, pCUCP->m_lInputParaNumber) << "输入型参数 = 输入型动态链接 + 尚未链接的输入型参数";
        }
      }
    }

    // clearup
    poUnit = unitlist.GetHeadPosition();
    iTotal = unitlist.GetCount();
    for (i = 0; i < iTotal; i++) {
      pcunitTemp = unitlist.GetNext(poUnit);
      delete pcunitTemp;
      pcunitTemp = nullptr;
    }
    // release list's memory
    unitlist.RemoveAll();
    listTotalUnit.RemoveAll();
    runtimeUnitList.RemoveAll();
  }

  ///////////////////////////////////////////////////////////////////////////////////
  //
  // 这个与上面的测试结果应该完全一样，所以用来做为对比。
  // TestEncapsulation1将EncapsulateUnitList拆分为其内部使用几个函数顺序执行。
  //
  ////////////////////////////////////////////////////////////////////////////////////
  TEST_P(TestCUnitComponentEncapsulation, TestEncapsulation20) {
    CString strFileName = GetParam();

    CFile cFile;
    char buffer[512];

    CString strFileHeader;
    CUnitList unitlist, listTotalUnit, runtimeUnitList;
    CUnitBase * pctemp, *pcunitTemp;
    INT64 i, iTotal;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;
    POSITION poUnit;

    if (!cFile.Open(strFileName, CFile::modeRead)) {
      EXPECT_FALSE(true) << "read file %s failed" << strFileName;
    }
    CArchive ar(&cFile, CArchive::load, 512, buffer);
    ar >> strFileHeader >> iCurrentUnit >> iTotal;
    for (i = 0; i < iTotal; i++) {
      ar >> pctemp;
      unitlist.AddTail(pctemp);
      pctemp->SetUpperUnitList(&unitlist);
    }

    ReSetCompileFlag(&unitlist);

    SetParaLockFlag(&unitlist);

    CreateUniUnitList(&unitlist, listTotalUnit);

    SetNoSrcUnitExectivePriority(&listTotalUnit);

    ExectiveCompilation(listTotalUnit, &runtimeUnitList);

    EncapsulateUnitlist(&unitlist, listTotalUnit);

    POSITION po = listTotalUnit.GetHeadPosition();
    INT64 jTotal = listTotalUnit.GetCount();
    CUnitBase * punit;
    for (int j = 0; j < jTotal; j++) {
      punit = listTotalUnit.GetNext(po);
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        if (punit->IsEncapsulable()) { // 可封装部件？
          EXPECT_TRUE(punit->IsEncapsulated()) << "可封装部件应该已经都被封装了";
        }
      }
    }

    EXPECT_EQ(runtimeUnitList.GetCount(), listTotalUnit.GetCount()) << "运行时单元序列与所有单元的数量不符";
    // 检查所有的部件内部单元的运行优先级
    poUnit = listTotalUnit.GetHeadPosition();
    for (i = 0; i < iTotal; i++) {
      pcunitTemp = listTotalUnit.GetNext(poUnit);
      if (pcunitTemp->GetExectivePriority() == 1) {
        EXPECT_TRUE(!pcunitTemp->IsHaveSourceUnit()) << "执行优先级为1的单元没有数据输入";
      }
      if (pcunitTemp->GetExectivePriority() > 1) {
        EXPECT_TRUE(pcunitTemp->IsHaveSourceUnit());
      }
      EXPECT_GT(pcunitTemp->GetExectivePriority(), 0) << "执行优先级大于1的单元有源单元（数据输入）";

      if (pcunitTemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        EXPECT_TRUE(pcunitTemp->IsEncapsulated()) << "封装部件完成";
      }
    }

    // clearup
    poUnit = unitlist.GetHeadPosition();
    iTotal = unitlist.GetCount();
    for (i = 0; i < iTotal; i++) {
      pcunitTemp = unitlist.GetNext(poUnit);
      delete pcunitTemp;
      pcunitTemp = nullptr;
    }
    // release list's memory
    unitlist.RemoveAll();
    listTotalUnit.RemoveAll();
    runtimeUnitList.RemoveAll();
  }

  class TestCUnitComponentCheckInnerDataLink : public::testing::TestWithParam<CString>
  {
  };

  INSTANTIATE_TEST_CASE_P(TestCheckInnerDataLink, TestCUnitComponentCheckInnerDataLink, testing::Values(
    "c:\\DACViewTestFile\\UnitComponent\\TestCheckInnerDataLink1.sqi"
    , "c:\\DACViewTestFile\\UnitComponent\\TestCheckInnerDataLink2.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套两层.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层参数有链接.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层部件之间链接参数有链接.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\封装后的部件.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层参数有链接有封装后的部件.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层参数有链接有封装后的多个部件.sqi"
  ));

  TEST_P(TestCUnitComponentCheckInnerDataLink, TestComponentCheckInnerDataLink) {
    CString strFileName = GetParam();
    CFile cFile;
    char buffer[512];
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);
    CUnitBase *cp2;
    CString strFileHeader;
    CUnitBase * pctemp, *pcunitTemp;
    INT64 i, iTotal;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;
    CUnitList unitlist, unitListRunTime;

    if (!cFile.Open(strFileName, CFile::modeRead)) {
      EXPECT_FALSE(true) << "read file " << strFileName << "  failed";
    }
    CArchive ar(&cFile, CArchive::load, 512, buffer);
    ar >> strFileHeader >> iCurrentUnit >> iTotal;

    for (int i = 0; i < iTotal; i++) {
      ar >> pctemp;
      unitlist.AddTail(pctemp);
      pctemp->SetUpperUnitList(&unitlist);
    }

    // 在编译单元序列前，必须重置编译标志及其他相关数据
    ReSetCompileFlag(&unitlist);

    SetParaLockFlag(&unitlist);

    CompileUnitList(&unitlist, &unitListRunTime);

    int l = 0;
    POSITION poUnit = unitlist.GetHeadPosition();
    for (i = 0; i < iTotal; i++) {
      pcunitTemp = unitlist.GetNext(poUnit);
      if (pcunitTemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)pcunitTemp;
        if (pCpt->IsEncapsulable()) {
          for (int j = 0; j < 16; j++) {
            if (pCpt->IsParaLinked(j)) {
              // 顺道测试此函数。
              EXPECT_FALSE((pCpt->GetDynLinkType(j) == pCpt->GetParaType(j)));
              if ((pCpt->GetParaType(j) & (tINPUT | tOUTPUT)) == tINPUT) {
                for (int k = 0; k < 16; k++) {
                  if (pCpt->IsInnerDataLinked(j, k)) {
                    EXPECT_EQ(pCpt->GetParaType(k) & (tINPUT | tOUTPUT), tOUTPUT) 
                      << "测试参数内部数据链接时目的参数类型应为输出型";
                    EXPECT_TRUE(pCpt->GetParaDestUnit(k) == nullptr) 
                      << "输出型参数的目的单元指针永远为nullptr" << "  k = " << k;
                    EXPECT_EQ(-1, pCpt->GetParaDestIndex(k)) << "目的单元参数索引重置为不可能";
                    l++;
                  }
                }
              }
            }
          }
          EXPECT_TRUE(pCpt->IsEncapsulated());
          EXPECT_TRUE(pCpt->HaveParameter());
        }
      }
    }
    EXPECT_GT(l, 1) << "测试CheckInnerDataLink函数时，其数据文件中的部件至少有两个输出型参数链接至一个内部单元";

    POSITION po = unitlist.GetHeadPosition();
    for (int i = 0; i < iTotal; i++) {
      cp2 = unitlist.GetNext(po);
      delete cp2;
    }
    // delete pCpt;
    unitlist.RemoveAll();
  }

}