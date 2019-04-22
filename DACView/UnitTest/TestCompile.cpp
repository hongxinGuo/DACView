#include"stdafx.h"
#include"pch.h"

#include"CUnitBase.h"
#include"CUnitComponent.h"
#include"SQIFileDoc.h"

#include"compileUnitList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

namespace DACViewTest {

  class TestCompile : public::testing::TestWithParam<CString>
  {
	protected:
		void SetUp(void) override {
			CString strFileName = GetParam();

			CFile cFile;
			char buffer[512];

			CString strFileHeader;
			INT32 iObjectNumber;
			INT64 iUnitNumber;

			if (!cFile.Open(strFileName, CFile::modeRead)) {
				EXPECT_FALSE(true) << "read file %s failed" << strFileName;
			}
			CArchive ar(&cFile, CArchive::load, 512, buffer);
			LoadSQIFile(ar, &m_unitlist, &m_objectlist, &iUnitNumber, &iObjectNumber);
		}

		void TearDown(void) override {
			// clearup
			ReleaseSQIFile(&m_unitlist, &m_objectlist);
			EXPECT_TRUE(m_unitlist.IsEmpty());
			EXPECT_TRUE(m_objectlist.IsEmpty());
		}

	public:
		CUnitList m_unitlist;
		CObjectList m_objectlist;
		CString strFileName;
  };

  INSTANTIATE_TEST_CASE_P(TestCompile, TestCompile,
    testing::Values("c:\\DACViewTestFile\\CompileUnitList\\简单单元.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\可封装部件.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\可封装部件1.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\可封装部件有Object输入.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套两层.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层参数有链接.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层部件之间链接参数有链接.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\封装后的部件.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层参数有链接有封装后的部件.sqi"
			, "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层参数有链接有封装后的多个部件.sqi"
			, "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层参数有链接有封装后的多个部件有Object多项输入.sqi"

    ));

  // 测试设置单元参数标志
  TEST_P(TestCompile, TestSetParaSelectedFlag) {
    INT64 iTotal;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitBase * pctemp, *pcunit;
    CUnitList UnitList;

    // 测试封装后的部件其编译标志已重置
    POSITION poUnit = m_unitlist.GetHeadPosition();
    iTotal = m_unitlist.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pctemp = m_unitlist.GetNext(poUnit);
      if (pctemp->IsEncapsulable() && pctemp->IsEncapsulated()) {
        EXPECT_TRUE(pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent)));
        CUnitList *pUList2 = ((CUnitComponent *)pctemp)->GetUnitList();
        POSITION poUnit2 = pUList2->GetHeadPosition();
        int jTotal = pUList2->GetCount();
        for (int j = 0; j < jTotal; j++) {
          CUnitBase * pcunit2 = pUList2->GetNext(poUnit2);
          EXPECT_FALSE(pcunit2->IsCompiled()); // 只检测了一层，更下面的层没有验证。
        }
      }
    }

    ReSetCompileFlag(&m_unitlist);

    // 测试编译标志是否重置
    POSITION po;
    poUnit = m_unitlist.GetHeadPosition();
    for (int i = 0; i < iTotal; i++) {
      pcunit = m_unitlist.GetNext(poUnit);
      if (!pcunit->IsEncapsulated()) {
        EXPECT_FALSE(pcunit->IsCompiled()) << "重置后编译标志为假";
      }
      EXPECT_FALSE(pcunit->IsHaveSourceUnit()) << "重置后有源数量为0";
      EXPECT_EQ(pcunit->GetExectivePriority(), 0) << "重置后执行优先级为0";
    }

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, UnitList);
    // 没有什么好的测试函数，暂时什么也不做了。

    // 测试设置参数选择标志是否正确，使用生成的UnitList做为对照
    poUnit = UnitList.GetHeadPosition();
    iTotal = UnitList.GetCount();
    int iDynLinkToNumber, iDynLinkToNumber2;
    for (int i = 0; i < iTotal; i++) {
      pcunit = UnitList.GetNext(poUnit);
      EXPECT_EQ(pcunit->GetExectivePriority(), 0) << "编译前执行优先级为0";
      iDynLinkToNumber = pcunit->GetInputParameterNumber(); 
      po = UnitList.GetHeadPosition();
      iDynLinkToNumber2 = 0;
      for (int j = 0; j < iTotal; j++) {
        pctemp = UnitList.GetNext(po);
        if (pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
          CUnitComponent * pCpt = (CUnitComponent *)pctemp;
          if (!pCpt->IsEncapsulated()) { // 如果是未封装的部件，则检测是否存在部件参数输入至内部单元的情况
            for (int l = 0; l < 16; l++) {
              if (pCpt->IsParaLinked(l)) {
                if (pCpt->GetParaDestUnit(l) != nullptr) { // 从部件参数输入至内部单元
                  EXPECT_EQ(pCpt->GetParaSrcUnit(l), nullptr) << "部件参数数据输入至内部单元时，源单元即部件本身，设置为nullptr" << pCpt->GetName();
                  if (pCpt->GetParaDestUnit(l) == pcunit) iDynLinkToNumber2++;
                }
              }
            }
          }
        }
        CUDLList * pDLList = pctemp->GetDynLinkList();
        POSITION poDL = pDLList->GetHeadPosition();
        INT64 kTotal = pDLList->GetCount();
        for (int k = 0; k < kTotal; k++) {
          CUnitDynLink * pDL = pDLList->GetNext(poDL);
          if (pDL->GetDestUnit() == pcunit) iDynLinkToNumber2++;
        }
      }
      EXPECT_EQ(iDynLinkToNumber, iDynLinkToNumber2) << "联入动态链接数量设置有误 " << strFileName << "  " << pcunit->GetName();
    }
  }

  // 测试设置无源单元的执行优先级为1
  TEST_P(TestCompile, TestSetNoSrcUnitExectivePriority) {
    CUnitList rtUnitList;
    CUnitBase *pcunitTemp;
    CUnitComponent * pCpt = nullptr;
    INT64 iCurrentUnit = 0;
    INT64 iTotal;

    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, rtUnitList);

    SetNoSrcUnitExectivePriority(&rtUnitList);

    iTotal = rtUnitList.GetCount();
    POSITION poUnit = rtUnitList.GetHeadPosition();
    for (int i = 0; i < iTotal; i++) {
      pcunitTemp = rtUnitList.GetNext(poUnit);
      if (pcunitTemp->GetExectivePriority() == 1) {
        EXPECT_TRUE(!pcunitTemp->IsHaveSourceUnit()) << "执行优先级为1的单元没有数据输入";
      }
      if (pcunitTemp->GetExectivePriority() > 1) {
        EXPECT_TRUE(pcunitTemp->IsHaveSourceUnit()) << "执行优先级大于1的单元有源单元（数据输入）";
      }
      if (pcunitTemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        if (!((CUnitComponent *)pcunitTemp)->IsEncapsulated()) { // 此时部件尚未被编译，故而封装的部件是已经封装了的，不测试
          EXPECT_EQ(pcunitTemp->GetExectivePriority(), 1) << "在设置无源单元的执行优先级时，部件执行优先级永远设置为1";
        }
      }
    }
  }

  // 测试编译单元序列
  TEST_P(TestCompile, TestExectiveCompilation) {
    CUnitList listUnit, runtimeUnitList;
    CUnitComponent * pCpt = nullptr;
    INT64 iTotal;

    CUnitBase *pcunitTemp;

    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, listUnit);

    SetNoSrcUnitExectivePriority(&listUnit);

    CUnitBase * pcunit;
    POSITION poUnit = runtimeUnitList.GetHeadPosition();
    iTotal = runtimeUnitList.GetCount();
    for (int i = 0; i < iTotal - 1; i++) {
      pcunit = runtimeUnitList.GetNext(poUnit);
      EXPECT_FALSE(pcunit->IsCompiled()) << "此时单元序列尚未编译" << pcunit->GetName();
    }

    ExectiveCompilation(listUnit, &runtimeUnitList);

    poUnit = listUnit.GetHeadPosition();
    iTotal = listUnit.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pcunitTemp = listUnit.GetNext(poUnit);
      if (pcunitTemp->GetExectivePriority() == 1) {
        EXPECT_TRUE(!pcunitTemp->IsHaveSourceUnit()) << "执行优先级为1的单元没有数据输入";
      }
      if (pcunitTemp->GetExectivePriority() > 1) {
        EXPECT_TRUE(pcunitTemp->IsHaveSourceUnit());
      }
      EXPECT_GT(pcunitTemp->GetExectivePriority(), 0) << "执行优先级大于1的单元有源单元（数据输入）";
      CUDLList * pUDLList = pcunitTemp->GetDynLinkList();
      INT64 jTotal = pUDLList->GetCount();
      POSITION poDL = pUDLList->GetHeadPosition();
      CUnitDynLink * pDL;
      for (int j = 0; j < jTotal; j++) {
        pDL = pUDLList->GetNext(poDL);
        EXPECT_LT(pcunitTemp->GetExectivePriority(), pDL->GetDestUnit()->GetExectivePriority()) << "动态链接优先级出错："
          << strFileName << "  " << pcunitTemp->GetName() << "  " << pDL->GetDestUnit()->GetName();
      }
    }

    poUnit = runtimeUnitList.GetHeadPosition();
    iTotal = runtimeUnitList.GetCount();
    pcunitTemp = runtimeUnitList.GetNext(poUnit);
    for (int i = 0; i < iTotal - 1; i++) {
      pcunit = runtimeUnitList.GetNext(poUnit);
      EXPECT_TRUE(pcunit->IsCompiled()) << "此时单元序列都编译了" << pcunit->GetName();
      EXPECT_LE(pcunitTemp->GetExectivePriority(), pcunit->GetExectivePriority()) << "运行时单元序列执行优先级排列错误";
      pcunitTemp = pcunit;
    }
  }

  TEST_P(TestCompile, TestEncapsulateUnitList) {
    CUnitList listUnit, runtimeUnitList;
    CUnitBase *pcunitTemp;
    CUnitComponent * pCpt = nullptr;
    INT64 iTotal;
    INT64 iCurrentUnit = sizeof(ULONG);

    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, listUnit);

    SetNoSrcUnitExectivePriority(&listUnit);

    ExectiveCompilation(listUnit, &runtimeUnitList);

    EncapsulateUnitlist(&listUnit, listUnit);

    EXPECT_EQ(runtimeUnitList.GetCount(), listUnit.GetCount()) << strFileName << "运行时单元序列与所有单元的数量不符";

    POSITION poUnit = listUnit.GetHeadPosition();
    iTotal = listUnit.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pcunitTemp = listUnit.GetNext(poUnit);
      if (pcunitTemp->GetExectivePriority() == 1) {
        EXPECT_TRUE(!pcunitTemp->IsHaveSourceUnit()) << "执行优先级为1的单元没有数据输入";
      }
      if (pcunitTemp->GetExectivePriority() > 1) {
        EXPECT_TRUE(pcunitTemp->IsHaveSourceUnit());
      }
      EXPECT_GT(pcunitTemp->GetExectivePriority(), 0) << "执行优先级大于1的单元有源单元（数据输入）";

      if (pcunitTemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        if (((CUnitComponent *)pcunitTemp)->IsEncapsulable()) {
          EXPECT_TRUE(pcunitTemp->IsEncapsulated()) << "封装部件完成";
        }
      }
    }
  }

  TEST_P(TestCompile, TestCompile) {
    INT64 iTotal;
    CUnitBase * pctemp, *pcunitTemp;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;

    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    POSITION po = m_unitlist.GetHeadPosition();
    iTotal = m_unitlist.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pctemp = m_unitlist.GetNext(po);
      if (pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)pctemp;
        if (!pCpt->IsPermitEncapsulation()) {
          EXPECT_EQ(pCpt->GetRunTimeUnitList()->GetCount(), 0); // 不可封装部件的执行单元序列是空的
        }
      }
    }

    CUnitList rtUnitList;

    if (!UnitListLoopDetect(&m_unitlist)) {   // 如果没有发现动态链接循环
      CompileUnitList(&m_unitlist, &rtUnitList);
    }
    else ASSERT_TRUE(0); // 有循环出现的话，就退出测试。

    CUnitBase * pcunit;
    POSITION poUnit = rtUnitList.GetHeadPosition();
    iTotal = rtUnitList.GetCount();
    pcunitTemp = rtUnitList.GetNext(poUnit);
    for (int i = 0; i < iTotal - 1; i++) {
      pcunit = rtUnitList.GetNext(poUnit);
      EXPECT_LE(pcunitTemp->GetExectivePriority(), pcunit->GetExectivePriority()) << "运行时单元序列执行优先级排列错误";
    }

    // 测试执行优先级。所有的单元序列（包括部件内部的单元序列）皆测试。
    CUnitDynLink * pDL;
    CUDLList * pUDLList;
    int jTotal, k, l;
    POSITION poDL;
    CUnitBase * pDestUnit;

    poUnit = rtUnitList.GetHeadPosition();
    iTotal = rtUnitList.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pctemp = rtUnitList.GetNext(poUnit);
      EXPECT_TRUE(pctemp->IsCompiled()) << strFileName << "  " << pctemp->GetName() << "  此时单元序列应该已经被编译了";
      EXPECT_LE(1, pctemp->GetExectivePriority());
      pUDLList = pctemp->GetDynLinkList();
      jTotal = pUDLList->GetCount();
      poDL = pUDLList->GetHeadPosition();
      for (int j = 0; j < jTotal; j++) {
        pDL = pUDLList->GetNext(poDL);
        k = pctemp->GetExectivePriority();
        pDestUnit = pDL->GetDestUnit();
        l = pDestUnit->GetExectivePriority();
        if ((k >= l) && (!pctemp->IsSetCutOff()) && (!pDestUnit->IsSetCutOff())) { //优先级出现问题？
          if (pctemp->GetComponentUpper() != pDestUnit) { //封装后，内部单元有数据输出到部件参数处，此时不考虑执行优先级（内部单元的执行总在部件执行之后）
            EXPECT_FALSE(0) << "动态链接执行优先级出现错误" << pctemp->GetName() << "  " << pDestUnit->GetName();
          }
        }
      }
    }

    // 
    po = m_unitlist.GetHeadPosition();
    iTotal = m_unitlist.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pctemp = m_unitlist.GetNext(po);
      if (pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)pctemp;
        if (pCpt->IsEncapsulable()) {
          EXPECT_TRUE(pCpt->IsEncapsulated()) << "after compile component"; // 测试部件封装标志
        }
        if ( !pCpt->IsPermitEncapsulation() ) {
          EXPECT_FALSE(pCpt->IsEncapsulated());
          CUnitList * pUnitList = pCpt->GetRunTimeUnitList();
          EXPECT_EQ(pUnitList->GetCount(), 0); // 不可封装部件的执行单元序列是空的
        }
      }
    }
  }


  TEST_P(TestCompile, TestComponentEncapsulation) {
    CUnitBase  *pcunitTemp;
    INT64 iTotal;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;
    CUnitList unitListRunTime;
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);


    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    // 所有与编译有关的测试，都需要编译整体文件。由于系统数据关联的原因，无法单独部件本身，故而需要编译整体文件，最后再测试封装。
    CompileUnitList(&m_unitlist, &unitListRunTime);

    POSITION poUnit = m_unitlist.GetHeadPosition();
    iTotal = m_unitlist.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pcunitTemp = m_unitlist.GetNext(poUnit);
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
  }

  ///////////////////////////////////////////////////////////////////////////////////////
  //
  // 所有与封装有关的测试都在这里，按部就班地一步步测试验证。
  //
  ////////////////////////////////////////////////////////////////////////////////////////
  TEST_P(TestCompile, TestEncapsulationStepByStep) {
    CUnitList listTotalUnit, runtimeUnitList, *pUnitList3;
    CUnitBase  *pcunitTemp, *pcunit;
    INT64 iTotal, jTotal, lTotal;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;
    CUDLList * pDLList = nullptr, *pDLList2 = nullptr;
    CUnitDynLink * pDL = nullptr, *pDL2 = nullptr;
    CUnitComponent * pSrcComponent = nullptr, *pDestComponent = nullptr;
    CUnitComponent * pCUCP = nullptr;

    // 设置编译所需之前置数据
    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, listTotalUnit);

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
          if (pDestComponent->IsEncapsulable()) pDestComponent->m_lInputParaNumber++;
          break;
        case COMPONENT_TO_COMPONENT:
          pSrcComponent = pDL->GetSrcUnit()->GetComponentUpper();
          ASSERT(pSrcComponent != nullptr);
          if (pSrcComponent->IsEncapsulable())pSrcComponent->m_lOutputParaNumber++;
          pDestComponent = pDL->GetDestUnit()->GetComponentUpper();
          ASSERT(pDestComponent != nullptr);
          if (pDestComponent->IsEncapsulable()) pDestComponent->m_lInputParaNumber++;
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
                EXPECT_EQ((pCUCP->GetParaType(j) & (tINPUT | tOUTPUT)), tINPUT); // 不是输出型，就是输入型。
              }
            }
          }
        }
      }
    }

    SetNoSrcUnitExectivePriority(&listTotalUnit);

    ExectiveCompilation(listTotalUnit, &runtimeUnitList);

    // 将Encapsulation展开，分段执行和测试
    poUnit = m_unitlist.GetHeadPosition();
    iTotal = m_unitlist.GetCount();
    CUnitList * pUnitList;
    POSITION po;
    CUnitBase * punit;
    for (int i = 0; i < iTotal; i++) {
      pcunit = m_unitlist.GetNext(poUnit);
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
                    else if (m_unitlist.Find(pSrcUnit->GetComponentUpper())) { // 包含单元的部件位于本单元序列中
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
                      EXPECT_EQ(pDL2->GetDynLinkClass(), UNIT_TO_COMPONENT) << strFileName << "处理后由COMPONENT_TO_COMPONENT变为UNIT_TO_COMPONENT";
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
          // 确保不可封装部件的运行时单元序列为空
          EXPECT_TRUE(pCUCP->GetRunTimeUnitList()->IsEmpty()) << strFileName << "不可封装部件运行时单元序列应该为空";
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
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent)) ) {
        if (!((CUnitComponent *)punit)->IsPermitEncapsulation() ) {
          EXPECT_TRUE(((CUnitComponent *)punit)->GetRunTimeUnitList()->IsEmpty()) << strFileName << "不可封装部件运行时单元序列应该为空";
        }
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
  }

  ///////////////////////////////////////////////////////////////////////////////////
  //
  // 这个与上面的测试结果应该完全一样，所以用来做为对比。
  // TestEncapsulation1将EncapsulateUnitList拆分为其内部使用几个函数顺序执行。
  //
  ////////////////////////////////////////////////////////////////////////////////////
  TEST_P(TestCompile, TestEncapsulation20) {
		/*
    CString strFileName = GetParam();

    CFile cFile;
    char buffer[512];

    CString strFileHeader;

    if (!cFile.Open(strFileName, CFile::modeRead)) {
      EXPECT_FALSE(true) << "read file %s failed" << strFileName;
    }
    CArchive ar(&cFile, CArchive::load, 512, buffer);
    INT32 iObjectNumber = 0;
    INT64 iUnitNumber = 0;
    LoadSQIFile(ar, &m_unitlist, &m_objectlist, &iUnitNumber, &iObjectNumber);
		*/
    CUnitList listTotalUnit, runtimeUnitList;
    CUnitBase *pcunitTemp;
    INT64 i, iTotal;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;
    POSITION poUnit;


    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, listTotalUnit);

    SetNoSrcUnitExectivePriority(&listTotalUnit);

    ExectiveCompilation(listTotalUnit, &runtimeUnitList);

    EncapsulateUnitlist(&m_unitlist, listTotalUnit);

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
    iTotal = listTotalUnit.GetCount();
    for (i = 0; i < iTotal; i++) {
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
  }

  TEST_P(TestCompile, TestSetExectivePriority) {
    CUnitBase * pctemp;
    INT64 iTotal;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;

    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CUnitList rtUnitList;

    EXPECT_FALSE(UnitListLoopDetect(&m_unitlist));  // 如果没有发现动态链接循环
    CompileUnitList(&m_unitlist, &rtUnitList);

    POSITION po = m_unitlist.GetHeadPosition();
    iTotal = m_unitlist.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pctemp = m_unitlist.GetNext(po);
      if (pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)pctemp;
        if (pCpt->IsEncapsulable()) {
          EXPECT_TRUE(pCpt->IsEncapsulated()) << "after component compiled";
        }
      }
    }

    po = m_unitlist.GetHeadPosition();
    POSITION poDL;
    CUnitBase * pcunittemp, * pcunit2;
    CString strName;
    CUDLList * pDLList;
    CUnitDynLink * pUnitDynLink;
    iTotal = m_unitlist.GetCount();
    INT32 a, b;
    for (int i = 0; i < iTotal; i++) {
      pcunittemp = m_unitlist.GetNext(po);
      pDLList = pcunittemp->GetDynLinkList();
      poDL = pDLList->GetHeadPosition();
      int jTotal = pDLList->GetCount();
      for (int j = 0; j < jTotal; j++) {
        pUnitDynLink = pDLList->GetNext(poDL);
        a = pcunittemp->GetExectivePriority();
        pcunit2 = pUnitDynLink->GetDestUnit();
        b = pcunit2->GetExectivePriority();
        EXPECT_LT(a, b) << "Found Error in File " << strFileName << "'s SetExectivePriority's Unit "
          << pcunittemp->GetName() << " -> " << pUnitDynLink->GetDestUnit()->GetName();
      }
    }
  }

}

namespace DACViewTest {

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
    CString strFileHeader;
    CUnitBase *pcunitTemp;
    INT64 iTotal;
    INT64 iUnitNumber, iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;
    CUnitList m_unitlist, unitListRunTime;

    if (!cFile.Open(strFileName, CFile::modeRead)) {
      EXPECT_FALSE(true) << "read file " << strFileName << "  failed";
    }
    CArchive ar(&cFile, CArchive::load, 512, buffer);
    INT32 iObjectNumber = 0;
    CObjectList m_objectlist;
    LoadSQIFile(ar, &m_unitlist, &m_objectlist, &iUnitNumber, &iObjectNumber);

    // 在编译单元序列前，必须重置编译标志及其他相关数据
    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CompileUnitList(&m_unitlist, &unitListRunTime);

    int l = 0;
    POSITION poUnit = m_unitlist.GetHeadPosition();
    iTotal = m_unitlist.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pcunitTemp = m_unitlist.GetNext(poUnit);
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
										CUnitBase * pcunit3 = pCpt->GetParaSrcUnit(k);
										CUDLList * pUDLList = pcunit3->GetDynLinkList();
										POSITION poDL = pUDLList->GetHeadPosition();
										INT64 jTotal = pUDLList->GetCount();
										bool fFind = false;
										CUnitDynLink * pDL;
										while (!fFind) {
											pDL = pUDLList->GetNext(poDL);
											if (pDL->GetDestUnit() == pCpt) fFind = true;
										}
										EXPECT_TRUE(fFind);
										EXPECT_EQ(pDL->GetDynLinkType(), pCpt->GetDynLinkType(k));
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

    // clear up
    ReleaseSQIFile(&m_unitlist, &m_objectlist);
    EXPECT_TRUE(m_unitlist.IsEmpty());
    EXPECT_TRUE(m_objectlist.IsEmpty());

  }

}
