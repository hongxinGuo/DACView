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

  // 检验punit是否位于unitlist中，返回值为punit中的单元数量（等于AddToList的数量）
  int TestIsInUnitList(CUnitBase * punit, CUnitList & unitlist) {
    if (!punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
      if (find(unitlist.begin(), unitlist.end(), punit) != unitlist.end()) return 1;
      else {
        ASSERT(0);
        return 0;
      }
    }
    else {
      CUnitComponent * pCpt = (CUnitComponent *)punit;
      if (pCpt->IsEncapsulated()) {
        if (find(unitlist.begin(), unitlist.end(), punit) != unitlist.end()) return 1;
        else {
          ASSERT(0);
          return 0;
        }
      }
      else {
        CUnitList * pUnitList = pCpt->GetUnitList();
        int iTotal = 0;
        if (find(unitlist.begin(), unitlist.end(), punit) != unitlist.end()) iTotal = 1;
        else {
          ASSERT(0);
          return 0;
        }
        for (const auto punit2 : *pUnitList) {
          iTotal += TestIsInUnitList(punit2, unitlist);
        }
        return iTotal;
      }
    }
  }

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
			EXPECT_TRUE(m_unitlist.empty());
			EXPECT_TRUE(m_objectlist.empty());
		}

	public:
		CUnitList m_unitlist;
		CObjectList m_objectlist;
		CString strFileName;
  };

  INSTANTIATE_TEST_CASE_P(TestCompile, TestCompile,
    testing::Values("c:\\DACViewTestFile\\CompileUnitList\\简单单元.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\可封装部件.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\可封装部件有截断.sqi"
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
      , "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层参数有链接有封装后的多个部件有截断有Object多项输入.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\各种类型都有100+.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\各种类型都有200+.sqi"
    ));

  // 测试设置单元参数标志
  TEST_P(TestCompile, TestSetParaSelectedFlag) {
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitList UnitList;

    // 测试封装后的部件其编译标志已设置（在读取部件时，如果是封装了的就设置其编译标志
    for (const auto punit : m_unitlist) {
      if (punit->IsEncapsulable() && punit->IsEncapsulated()) {
        EXPECT_TRUE(punit->IsKindOf(RUNTIME_CLASS(CUnitComponent)));
        EXPECT_FALSE(punit->IsCompiled()) << "此部件本身的编译标志尚未设置";
        CUnitList *pUList2 = ((CUnitComponent *)punit)->GetUnitList();
        for (const auto pcunit2 : *pUList2) {
          EXPECT_TRUE(pcunit2->IsCompiled()); // 只检测了一层，更下面的层没有验证。
        }
      }
    }

    ReSetCompileFlag(&m_unitlist);

    CreateUniUnitList(&m_unitlist, UnitList);

    // 测试编译标志是否重置
    for (const auto punit : UnitList) {
      EXPECT_FALSE(punit->IsCompiled()) << "重置后编译标志为假";
      EXPECT_FALSE(punit->IsHaveSourceUnit()) << "重置后有源数量为0";
      EXPECT_EQ(punit->GetExectivePriority(), 0) << "重置后执行优先级为0";
    }

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    // 检验m_unitlsit中的可编译单元是否位于UnitList中，且数量相同
    int iTotal = 0;
    for (const auto punit : m_unitlist) {
      iTotal += TestIsInUnitList(punit, UnitList);
    }
    EXPECT_EQ(iTotal, UnitList.size()) << "生成单一单元序列的数量不符";

    // 测试设置参数选择标志是否正确，使用生成的UnitList做为对照
    int iDynLinkToNumber, iDynLinkToNumber2;
    for (const auto punit : UnitList) {
      EXPECT_EQ(punit->GetExectivePriority(), 0) << "编译前执行优先级为0";
      iDynLinkToNumber = punit->GetInputParameterNumber(); 
      iDynLinkToNumber2 = 0;
      for (const auto pctemp : UnitList) {
        if (pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
          CUnitComponent * pCpt = (CUnitComponent *)pctemp;
          if (!pCpt->IsEncapsulated()) { // 如果是未封装的部件，则检测是否存在部件参数输入至内部单元的情况
            for (int l = 0; l < 16; l++) {
              if (pCpt->IsParaLinked(l)) {
                if (pCpt->GetParaDestUnit(l) != nullptr) { // 从部件参数输入至内部单元
                  EXPECT_EQ(pCpt->GetParaSrcUnit(l), nullptr) << "部件参数数据输入至内部单元时，源单元即部件本身，设置为nullptr" << pCpt->GetName();
                  if (pCpt->GetParaDestUnit(l) == punit) iDynLinkToNumber2++;
                }
              }
            }
          }
        }
        CUDLList * pDLList = pctemp->GetDynLinkList();
        for (const auto pDL : *pDLList) {
          if (pDL->GetDestUnit() == punit) iDynLinkToNumber2++;
        }
      }
      EXPECT_EQ(iDynLinkToNumber, iDynLinkToNumber2) << "联入动态链接数量设置有误 " << strFileName << "  " << punit->GetName();
    }
  }

  // 测试设置无源单元的执行优先级为1
  TEST_P(TestCompile, TestSetNoSrcUnitExectivePriority) {
    CUnitList rtUnitList;
    CUnitComponent * pCpt = nullptr;
    INT64 iCurrentUnit = 0;

    ReSetCompileFlag(&m_unitlist);
 
    CreateUniUnitList(&m_unitlist, rtUnitList);

    SetParaLockFlag(&m_unitlist, &m_objectlist);
 
    SetNoSrcUnitExectivePriority(&rtUnitList);

    for (const auto punit : rtUnitList) {
      if (punit->GetExectivePriority() == 1) {
        EXPECT_TRUE(!punit->IsHaveSourceUnit()) << "执行优先级为1的单元没有数据输入";
      }
      if (punit->GetExectivePriority() > 1) {
        EXPECT_TRUE(punit->IsHaveSourceUnit()) << "执行优先级大于1的单元有源单元（数据输入）";
      }
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        if (!((CUnitComponent *)punit)->IsEncapsulated() && ((CUnitComponent *)punit)->IsEncapsulable()) { // 此时部件尚未被编译，故而封装的部件是已经封装了的，不测试
          EXPECT_EQ(punit->GetExectivePriority(), 1) << "在设置无源单元的执行优先级时，部件执行优先级永远设置为1";
        }
      }
    }
  }

  TEST_P(TestCompile, TestCompile) {
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;

    CUnitList rtUnitList;

    if (!UnitListLoopDetect(&m_unitlist)) {   // 如果没有发现动态链接循环
      Compilation(&m_unitlist, m_objectlist, &rtUnitList);
    }
    else ASSERT_TRUE(0); // 有循环出现的话，就退出测试。

    CUnitList unitlist;
    CreateUniUnitList(&m_unitlist, unitlist);
    EXPECT_EQ(unitlist.size(), rtUnitList.size());
    // 待编译的单元序列与编译后的单元序列完全相同
    for (auto punit : unitlist) {
      EXPECT_TRUE(find(rtUnitList.begin(), rtUnitList.end(), punit) != rtUnitList.end());
    }

    CUnitBase * pcunitTemp = rtUnitList.front();
    auto it = rtUnitList.begin();
    it++;
    for (; it != rtUnitList.end(); it++) {
      auto punit = *it;
      EXPECT_LE(pcunitTemp->GetExectivePriority(), punit->GetExectivePriority()) << "运行时单元序列执行优先级排列错误"
        << pcunitTemp->GetName() << "  " << punit->GetName();
      pcunitTemp = punit;
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        CUnitComponent * pCpt = (CUnitComponent *)punit;
        if (pCpt->IsEncapsulated() && pCpt->mtest_fEncapsulating && pCpt->IsEncapsulating()) { //可封装部件且在此次编译中封装中
          CUnitList * prtUnitList = pCpt->GetRunTimeUnitList(); // 检测此部件的内部运行时单元序列
          auto it2 = prtUnitList->begin();
          auto punitTemp2 = *it2++;
          for (; it2 != prtUnitList->end(); it2++) {
            auto punit2 = *it2;
            EXPECT_LE(punitTemp2->GetExectivePriority(), punit2->GetExectivePriority()) << "运行时单元序列执行优先级排列错误"
              << punitTemp2->GetName() << "  " << punit2->GetName();
            punitTemp2 = punit2;
          }
        }
      }
    }

    // 测试执行优先级。所有的单元序列（包括部件内部的单元序列）皆测试。
    CUDLList * pUDLList;
    int k, l;
    CUnitBase * pDestUnit;

    for (const auto pctemp : rtUnitList) {
      EXPECT_TRUE(pctemp->IsCompiled()) << strFileName << "  " << pctemp->GetName() << "  此时单元序列应该已经被编译了";
      EXPECT_LE(1, pctemp->GetExectivePriority());
      pUDLList = pctemp->GetDynLinkList();
      for (const auto pDL : *pUDLList) {
        k = pctemp->GetExectivePriority();
        pDestUnit = pDL->GetDestUnit();
        l = pDestUnit->GetExectivePriority();
        if ((k >= l) && (!pctemp->IsCutoff()) && (!pDestUnit->IsCutoff())) { //优先级出现问题？
          if (pctemp->GetComponentUpper() != pDestUnit) { //封装后，内部单元有数据输出到部件参数处，此时不考虑执行优先级（内部单元的执行总在部件执行之后）
            EXPECT_FALSE(0) << "动态链接执行优先级出现错误" << pctemp->GetName() << "  " << pDestUnit->GetName();
          }
        }
      }
    }

    // 
    for (const auto punit : m_unitlist) {
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)punit;
        if (pCpt->IsEncapsulable()) {
          EXPECT_TRUE(pCpt->IsEncapsulated()) << "after compile component"; // 测试部件封装标志
        }
        if ( !pCpt->IsPermitEncapsulation() ) {
          EXPECT_FALSE(pCpt->IsEncapsulated());
          CUnitList * pUnitList = pCpt->GetRunTimeUnitList();
          EXPECT_EQ(pUnitList->size(), 0); // 不可封装部件的执行单元序列是空的
        }
      }
    }
  }

  TEST_P(TestCompile, TestCompilation) {
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;
    CUnitList unitListRunTime, unitlistTotal;
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);
    CUnitList unitlist;

    CreateUniUnitList(&m_unitlist, unitlist);  // 准备所有被编译单元序列

    // 所有与编译有关的测试，都需要编译整体文件。由于系统数据关联的原因，无法单独部件本身，故而需要编译整体文件，最后再测试封装。
    Compilation(&m_unitlist, m_objectlist, &unitListRunTime);

    for (const auto punit : unitlist) {
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)punit;
        if (pCpt->IsEncapsulable()) {
          for (int i = 0; i < 16; i++) {
            if (pCpt->IsParaLinked(i)) {
              // 顺道测试此函数。
              EXPECT_TRUE((pCpt->GetDynLinkType(i) & pCpt->GetParaType(i)));
            }
          }
          EXPECT_TRUE(pCpt->IsEncapsulated());
          //EXPECT_TRUE(pCpt->HaveParameter());
        }
      }
    }

    // 测试所有的单元都编译了
    for (const auto punit : unitlist) {
      EXPECT_GT(punit->GetExectivePriority(), 0);
      EXPECT_TRUE(punit->IsCompiled());
      if (!punit->IsHaveSourceUnit()) EXPECT_EQ(punit->GetExectivePriority(), 1);
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { 
        if (punit->mtest_fEncapsulating) {// 封装中的部件都应该封装了
          EXPECT_TRUE(punit->IsEncapsulated());
          EXPECT_TRUE(punit->IsEncapsulable());  
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
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;
    CUDLList * pDLList = nullptr, *pDLList2 = nullptr;
    shared_ptr<CUnitDynLink> pDL, pDL2;
    CUnitComponent * pSrcComponent = nullptr, *pDestComponent = nullptr;
    CUnitComponent * pCUCP = nullptr;

    // 设置编译所需之前置数据
    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, listTotalUnit);

    SetEncapsulatingFlag(listTotalUnit);

    // 测试部件是否正确设置了封装中标志
    for (const auto punit : listTotalUnit) {
      EXPECT_FALSE(punit->IsCompiled()) << "重置后编译标志为假";
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        CUnitComponent * pCpt;
        pCpt = (CUnitComponent *)punit;
        if (pCpt->IsEncapsulated() || !(pCpt->IsEncapsulable())) {
          EXPECT_FALSE(pCpt->IsEncapsulating()) << "封装过或不可封装的部件不允许再次封装";
          EXPECT_FALSE(pCpt->mtest_fEncapsulating);
        }
        else {
          EXPECT_TRUE(pCpt->IsEncapsulating()) << "未封装的部件此时处于封装中";
          EXPECT_TRUE(pCpt->mtest_fEncapsulating);
        }
      }
    }

    // 计算从部件输入和输出的动态链接数据的数量，以备封装部件时测试接口参数的设置是否正确
    for (const auto pcunitTemp : listTotalUnit) {
      pDLList = pcunitTemp->GetDynLinkList();
      for (const auto pDL : *pDLList) {
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
    for (const auto pcunitTemp : listTotalUnit) {
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

    // 将Encapsulation展开，分段执行和测试
    CUnitList * pUnitList;
    for (const auto pcunit1 : listTotalUnit) {
      if (pcunit1->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCUCP = (CUnitComponent *)pcunit1;
        if (pCUCP->IsEncapsulable()) { // 可封装部件的动作
          if (pCUCP->IsEncapsulated()) continue;
          
          // 首先封装下层部件
          pCUCP->EncapsulateBelowComponent(listTotalUnit);

          // 验证下层部件都封装了（目前只是验证下面一层，再往下的尚未能测试，只能靠最后的测试所有的部件都封装了来间接验证）
          pUnitList = pCUCP->GetUnitList();
          for (const auto punit : *pUnitList) {
            if (punit->IsEncapsulable()) {
              EXPECT_TRUE(punit->IsEncapsulated()) << "下面一层的部件都封装了";
            }
          }

          // 检查部件状态是否正确， 此函数本身就是用来验证的，不需要再次检验
          pCUCP->CheckComponentSelf();
          
          // 将部件参数中输出型的在内部源单元处建立一个新的动态链接，目的单元为本部件。
          // 此步骤要先于处理联入联出的过程执行。
          pCUCP->CreateNewDynLinkFromInterfaceOutputTypePara();

          // 测试输出型参数生成新动态链接的情况
          for (int l = 0; l < 16; l++) {
            if (pCUCP->IsParaLinked(l)) {
              if ((pCUCP->GetParaType(l) & (tINPUT | tOUTPUT)) == tOUTPUT) {
                CUnitBase * punit5 = pCUCP->GetParaSrcUnit(l);
                CUDLList * pUDLList = punit5->GetDynLinkList();
                INT64 kTotal = pUDLList->size();
                auto itDL1 = pUDLList->begin();
                shared_ptr<CUnitDynLink> pDL = *itDL1;
                while (pDL->GetDestUnit() != pCUCP) {
                  pDL = *++itDL1;
                }
                EXPECT_EQ(pDL->GetDestUnit(), pCUCP) << "输出型参数生成新动态链接时其目的单元就是本部件";
                EXPECT_EQ(pDL->GetSrcUnit(), punit5);
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
                for (const auto pDL11 : *pDLList) {
                  if (pDL11->GetDestUnit() == pCUCP) { // 找到源单元的动态链接了
                    CUnitBase * pSrcUnit = pDL11->GetSrcUnit();
                    if (pSrcUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
                      EXPECT_EQ(pDL11->GetDynLinkClass(), UNIT_TO_UNIT) << "源单元为部件的其动态链接类型为COMPONENT_TO_COMPONENT";
                    }
                    else if (pSrcUnit->GetComponentUpper() != nullptr) {
                      CUnitList * pUnitList = pSrcUnit->GetComponentUpper()->GetUnitList();
                      if (find(pUnitList->begin(), pUnitList->end(), pSrcUnit->GetComponentUpper()) != pUnitList->end()) { // 包含单元的部件位于本单元序列中
                        EXPECT_EQ(pDL11->GetDynLinkClass(), COMPONENT_TO_UNIT) << "包含源单元的部件位于最上层单元序列中";
                        if (pSrcUnit->GetComponentUpper()->IsEncapsulable()) {
                          EXPECT_FALSE(pSrcUnit->GetComponentUpper()->IsEncapsulated()) << "部件尚未封装";
                        }
                      }
                      EXPECT_EQ(pDL11->GetDynLinkClass(), UNIT_TO_UNIT) << "源单元为简单单元";
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

          EXPECT_EQ((pCUCP->GetDynLinkList())->size(), 0); // 处理输出型动态链接前，部件本身的动态链接序列是零
          // 寻找是否存在联出本部件的动态链接。
          // 在本部件中生成一个新的动态链接，将联出的动态链接赋予此新的动态链接，其源单元改为本部件，
          // 并将内部源单元的目的单元设置为本部件，重置部件参数中的目的单元指针和目的参数索引。
          pCUCP->HandleTheDynLinkedfromComponent();

          // 测试处理输出型动态链接是否正确
          bool fFound = false;
          pUnitList3 = pCUCP->GetUnitList();
          for (const auto pcunitTemp : *pUnitList3) {
            pDLList = pcunitTemp->GetDynLinkList();
            for (const auto pDL : *pDLList) {
              if (pDL->GetDestUnit() == pCUCP) {
                if (pDL->mTest_pDestUnitSaved != nullptr) {
                  pDLList2 = pCUCP->GetDynLinkList();
                  for (auto itDL2 = pDLList2->begin(); itDL2 != pDLList2->end(); itDL2++) {
                    pDL2 = *itDL2;
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
          // 这个函数调用必须是最后第二步，否则内部动态链接尚未设置好，结果不对。
          pCUCP->SetInnerDataLinkFlag();

          // 测试SetInnerDataLinkFlag()。就差这步了，但如何测试呢？

          // 设置从本部件参数处联入数据的内部单元的执行优先级
          pCUCP->SetMyUnitListExectivePriority();

          pCUCP->SetEncapsulateFlag(true);
        }
        else { // 不可封装部件的动作
          // 首先封装下层部件
          pCUCP->EncapsulateBelowComponent(listTotalUnit);

          // 测试是否下层部件都封装了
          pUnitList = pCUCP->GetUnitList();
          for (const auto punit : *pUnitList) {
            if (punit->IsEncapsulable()) {
              EXPECT_TRUE(punit->IsEncapsulated()) << "下面一层的部件都封装了";
            }
          }

          // 检查部件状态是否正确
          //pCUCP->CheckComponentSelf();
          // 确保不可封装部件的运行时单元序列为空
          EXPECT_TRUE(pCUCP->GetRunTimeUnitList()->empty()) << strFileName << "不可封装部件运行时单元序列应该为空";
        }
      }
    }

    // 测试可封装部件都封装了（除不可封装的部件外）
    for (const auto punit : listTotalUnit) {
      if (punit->IsEncapsulable()) { // 可封装部件？
        EXPECT_TRUE(punit->IsEncapsulated()) << "可封装部件应该已经都被封装了";
      }
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent)) ) {
        if (!((CUnitComponent *)punit)->IsPermitEncapsulation() ) {
          EXPECT_TRUE(((CUnitComponent *)punit)->GetRunTimeUnitList()->empty()) << strFileName << "不可封装部件运行时单元序列应该为空";
        }
      }
    }

    CUnitList unitlist;
    CreateUniUnitList(&m_unitlist, unitlist);
    // 编译此生成的单元序列
    CompileUnitList(&unitlist, &runtimeUnitList);

    for (const auto punit : listTotalUnit) {
      if (punit->GetExectivePriority() == 1) {
        if ((punit->IsHaveSourceUnit()) && !punit->IsCutoff()) {
          CUnitComponent * pCpt = punit->GetComponentUpper();
          bool fFound = false;
          if (pCpt != nullptr) {
            for (int i = 0; i < 16; i++) {
              if (pCpt->GetParaDestUnit(i) == punit) {
                fFound = true;
              }
            }
            EXPECT_TRUE(fFound) << "执行优先级为1的单元必须有数据从部件参数处获得";
          }
          else {
            EXPECT_TRUE(false);
          }
        }
      }
      if (punit->GetExectivePriority() > 1) {
        EXPECT_TRUE(punit->IsHaveSourceUnit() || punit->IsCutoff());
      }
      EXPECT_GT(punit->GetExectivePriority(), 0) << "执行优先级大于1的单元有源单元（数据输入）";

      if (punit->IsEncapsulable()) {
        EXPECT_TRUE(punit->IsEncapsulated()) << "封装部件完成";
      }
    }

    //测试部件的输入输出参数个数。
    INT64 lSrc = 0, lDest = 0;
    for (const auto pcunitTemp : listTotalUnit) {
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

    CUnitList listTotalUnit, runtimeUnitList;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;

    // 设置编译所需之前置数据
    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, listTotalUnit);

    SetEncapsulatingFlag(listTotalUnit);
    
    // 封装单元序列中的可封装部件
    EncapsulateUnitList(listTotalUnit);

    CUnitList unitlist;
    CreateUniUnitList(&m_unitlist, unitlist);
    // 编译此单元序列
    CompileUnitList(&unitlist, &runtimeUnitList);

    for (const auto punit : listTotalUnit) {
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        if (punit->IsEncapsulable()) { // 可封装部件？
          EXPECT_TRUE(punit->IsEncapsulated()) << "可封装部件应该已经都被封装了";
        }
      }
    }

    EXPECT_EQ(runtimeUnitList.size(), unitlist.size()) << "运行时单元序列与本单元序列的数量不符";
    unitlist.clear();

    // 检查所有的部件内部单元的运行优先级
    for (const auto punit : listTotalUnit) {
      if (punit->GetExectivePriority() == 1) {
        if ((punit->IsHaveSourceUnit()) && !punit->IsCutoff()) {
          CUnitComponent * pCpt = punit->GetComponentUpper();
          bool fFound = false;
          if (pCpt != nullptr) {
            for (int i = 0; i < 16; i++) {
              if (pCpt->GetParaDestUnit(i) == punit) {
                fFound = true;
              }
            }
            EXPECT_TRUE(fFound) << "执行优先级为1的单元必须有数据从部件参数处获得";
          }
          else {
            EXPECT_TRUE(false);
          }
        }
      }
      if (punit->GetExectivePriority() > 1) {
        EXPECT_TRUE(punit->IsHaveSourceUnit() || punit->IsCutoff());
      }
      EXPECT_GT(punit->GetExectivePriority(), 0) << "执行优先级大于1的单元有源单元（数据输入）";

      if (punit->IsEncapsulable()) {
        EXPECT_TRUE(punit->IsEncapsulated()) << "封装部件完成";
      }
    }
  }

  TEST_P(TestCompile, TestSetExectivePriority) {
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;
    CUnitList listTotalUnit, runtimeUnitList;

    ReSetCompileFlag(&m_unitlist);
    CreateUniUnitList(&m_unitlist, listTotalUnit);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    SetEncapsulatingFlag(listTotalUnit);

    // 封装单元序列中的可封装部件
    EncapsulateUnitList(listTotalUnit);
    listTotalUnit.clear();

    CreateUniUnitList(&m_unitlist, listTotalUnit); // 此时部件已经封装，故而生成的listTotalUnit要少于之前生成的单元序列

    // 编译此单元序列
    CompileUnitList(&listTotalUnit, &runtimeUnitList);


    for (const auto pctemp : m_unitlist) {
      if (pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)pctemp;
        if (pCpt->IsEncapsulable()) {
          EXPECT_TRUE(pCpt->IsEncapsulated()) << "after component compiled";
        }
      }
    }

    CUnitBase * pcunit2;
    CString strName;
    CUDLList * pDLList;
    INT32 a, b;
    for (const auto pcunittemp : m_unitlist) {
      pDLList = pcunittemp->GetDynLinkList();
      for (const auto pDL : *pDLList) {
        a = pcunittemp->GetExectivePriority();
        pcunit2 = pDL->GetDestUnit();
        b = pcunit2->GetExectivePriority();
        if (!pcunit2->IsCutoff()) { // 没有设置截断的目的单元其执行优先级必须小于源单元
          EXPECT_LT(a, b) << "Found Error in File " << strFileName << "'s SetExectivePriority's Unit "
            << pcunittemp->GetName() << " -> " << pDL->GetDestUnit()->GetName();
        }
      }
    }
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
    CString strFileHeader;
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


    Compilation(&m_unitlist, m_objectlist, &unitListRunTime);

    int l = 0;
    for (const auto pcunitTemp : m_unitlist) {
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
										auto it = pUDLList->begin();
										INT64 jTotal = pUDLList->size();
										bool fFind = false;
										shared_ptr<CUnitDynLink> pDL;
										while (!fFind) {
											pDL = *it++;
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
    EXPECT_TRUE(m_unitlist.empty());
    EXPECT_TRUE(m_objectlist.empty()); 

  }

}
