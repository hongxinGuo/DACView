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

  // ����punit�Ƿ�λ��unitlist�У�����ֵΪpunit�еĵ�Ԫ����������AddToList��������
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
    testing::Values("c:\\DACViewTestFile\\CompileUnitList\\�򵥵�Ԫ.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ����.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�����нض�.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ����1.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ������Object����.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ������.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ�.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ����������.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ㲿��֮�����Ӳ���������.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\��װ��Ĳ���.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ�����������з�װ��Ĳ���.sqi"
			, "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ�����������з�װ��Ķ������.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ�����������з�װ��Ķ��������Object��������.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ�����������з�װ��Ķ�������нض���Object��������.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�������Ͷ���100+.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�������Ͷ���200+.sqi"
    ));

  // �������õ�Ԫ������־
  TEST_P(TestCompile, TestSetParaSelectedFlag) {
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitList UnitList;

    // ���Է�װ��Ĳ���������־�����ã��ڶ�ȡ����ʱ������Ƿ�װ�˵ľ�����������־
    for (const auto punit : m_unitlist) {
      if (punit->IsEncapsulable() && punit->IsEncapsulated()) {
        EXPECT_TRUE(punit->IsKindOf(RUNTIME_CLASS(CUnitComponent)));
        EXPECT_FALSE(punit->IsCompiled()) << "�˲�������ı����־��δ����";
        CUnitList *pUList2 = ((CUnitComponent *)punit)->GetUnitList();
        for (const auto pcunit2 : *pUList2) {
          EXPECT_TRUE(pcunit2->IsCompiled()); // ֻ�����һ�㣬������Ĳ�û����֤��
        }
      }
    }

    ReSetCompileFlag(&m_unitlist);

    CreateUniUnitList(&m_unitlist, UnitList);

    // ���Ա����־�Ƿ�����
    for (const auto punit : UnitList) {
      EXPECT_FALSE(punit->IsCompiled()) << "���ú�����־Ϊ��";
      EXPECT_FALSE(punit->IsHaveSourceUnit()) << "���ú���Դ����Ϊ0";
      EXPECT_EQ(punit->GetExectivePriority(), 0) << "���ú�ִ�����ȼ�Ϊ0";
    }

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    // ����m_unitlsit�еĿɱ��뵥Ԫ�Ƿ�λ��UnitList�У���������ͬ
    int iTotal = 0;
    for (const auto punit : m_unitlist) {
      iTotal += TestIsInUnitList(punit, UnitList);
    }
    EXPECT_EQ(iTotal, UnitList.size()) << "���ɵ�һ��Ԫ���е���������";

    // �������ò���ѡ���־�Ƿ���ȷ��ʹ�����ɵ�UnitList��Ϊ����
    int iDynLinkToNumber, iDynLinkToNumber2;
    for (const auto punit : UnitList) {
      EXPECT_EQ(punit->GetExectivePriority(), 0) << "����ǰִ�����ȼ�Ϊ0";
      iDynLinkToNumber = punit->GetInputParameterNumber(); 
      iDynLinkToNumber2 = 0;
      for (const auto pctemp : UnitList) {
        if (pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
          CUnitComponent * pCpt = (CUnitComponent *)pctemp;
          if (!pCpt->IsEncapsulated()) { // �����δ��װ�Ĳ����������Ƿ���ڲ��������������ڲ���Ԫ�����
            for (int l = 0; l < 16; l++) {
              if (pCpt->IsParaLinked(l)) {
                if (pCpt->GetParaDestUnit(l) != nullptr) { // �Ӳ��������������ڲ���Ԫ
                  EXPECT_EQ(pCpt->GetParaSrcUnit(l), nullptr) << "�������������������ڲ���Ԫʱ��Դ��Ԫ��������������Ϊnullptr" << pCpt->GetName();
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
      EXPECT_EQ(iDynLinkToNumber, iDynLinkToNumber2) << "���붯̬���������������� " << strFileName << "  " << punit->GetName();
    }
  }

  // ����������Դ��Ԫ��ִ�����ȼ�Ϊ1
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
        EXPECT_TRUE(!punit->IsHaveSourceUnit()) << "ִ�����ȼ�Ϊ1�ĵ�Ԫû����������";
      }
      if (punit->GetExectivePriority() > 1) {
        EXPECT_TRUE(punit->IsHaveSourceUnit()) << "ִ�����ȼ�����1�ĵ�Ԫ��Դ��Ԫ���������룩";
      }
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        if (!((CUnitComponent *)punit)->IsEncapsulated() && ((CUnitComponent *)punit)->IsEncapsulable()) { // ��ʱ������δ�����룬�ʶ���װ�Ĳ������Ѿ���װ�˵ģ�������
          EXPECT_EQ(punit->GetExectivePriority(), 1) << "��������Դ��Ԫ��ִ�����ȼ�ʱ������ִ�����ȼ���Զ����Ϊ1";
        }
      }
    }
  }

  TEST_P(TestCompile, TestCompile) {
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;

    CUnitList rtUnitList;

    if (!UnitListLoopDetect(&m_unitlist)) {   // ���û�з��ֶ�̬����ѭ��
      Compilation(&m_unitlist, m_objectlist, &rtUnitList);
    }
    else ASSERT_TRUE(0); // ��ѭ�����ֵĻ������˳����ԡ�

    CUnitList unitlist;
    CreateUniUnitList(&m_unitlist, unitlist);
    EXPECT_EQ(unitlist.size(), rtUnitList.size());
    // ������ĵ�Ԫ����������ĵ�Ԫ������ȫ��ͬ
    for (auto punit : unitlist) {
      EXPECT_TRUE(find(rtUnitList.begin(), rtUnitList.end(), punit) != rtUnitList.end());
    }

    CUnitBase * pcunitTemp = rtUnitList.front();
    auto it = rtUnitList.begin();
    it++;
    for (; it != rtUnitList.end(); it++) {
      auto punit = *it;
      EXPECT_LE(pcunitTemp->GetExectivePriority(), punit->GetExectivePriority()) << "����ʱ��Ԫ����ִ�����ȼ����д���"
        << pcunitTemp->GetName() << "  " << punit->GetName();
      pcunitTemp = punit;
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        CUnitComponent * pCpt = (CUnitComponent *)punit;
        if (pCpt->IsEncapsulated() && pCpt->mtest_fEncapsulating && pCpt->IsEncapsulating()) { //�ɷ�װ�������ڴ˴α����з�װ��
          CUnitList * prtUnitList = pCpt->GetRunTimeUnitList(); // ���˲������ڲ�����ʱ��Ԫ����
          auto it2 = prtUnitList->begin();
          auto punitTemp2 = *it2++;
          for (; it2 != prtUnitList->end(); it2++) {
            auto punit2 = *it2;
            EXPECT_LE(punitTemp2->GetExectivePriority(), punit2->GetExectivePriority()) << "����ʱ��Ԫ����ִ�����ȼ����д���"
              << punitTemp2->GetName() << "  " << punit2->GetName();
            punitTemp2 = punit2;
          }
        }
      }
    }

    // ����ִ�����ȼ������еĵ�Ԫ���У����������ڲ��ĵ�Ԫ���У��Բ��ԡ�
    CUDLList * pUDLList;
    int k, l;
    CUnitBase * pDestUnit;

    for (const auto pctemp : rtUnitList) {
      EXPECT_TRUE(pctemp->IsCompiled()) << strFileName << "  " << pctemp->GetName() << "  ��ʱ��Ԫ����Ӧ���Ѿ���������";
      EXPECT_LE(1, pctemp->GetExectivePriority());
      pUDLList = pctemp->GetDynLinkList();
      for (const auto pDL : *pUDLList) {
        k = pctemp->GetExectivePriority();
        pDestUnit = pDL->GetDestUnit();
        l = pDestUnit->GetExectivePriority();
        if ((k >= l) && (!pctemp->IsCutoff()) && (!pDestUnit->IsCutoff())) { //���ȼ��������⣿
          if (pctemp->GetComponentUpper() != pDestUnit) { //��װ���ڲ���Ԫ�������������������������ʱ������ִ�����ȼ����ڲ���Ԫ��ִ�����ڲ���ִ��֮��
            EXPECT_FALSE(0) << "��̬����ִ�����ȼ����ִ���" << pctemp->GetName() << "  " << pDestUnit->GetName();
          }
        }
      }
    }

    // 
    for (const auto punit : m_unitlist) {
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)punit;
        if (pCpt->IsEncapsulable()) {
          EXPECT_TRUE(pCpt->IsEncapsulated()) << "after compile component"; // ���Բ�����װ��־
        }
        if ( !pCpt->IsPermitEncapsulation() ) {
          EXPECT_FALSE(pCpt->IsEncapsulated());
          CUnitList * pUnitList = pCpt->GetRunTimeUnitList();
          EXPECT_EQ(pUnitList->size(), 0); // ���ɷ�װ������ִ�е�Ԫ�����ǿյ�
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

    CreateUniUnitList(&m_unitlist, unitlist);  // ׼�����б����뵥Ԫ����

    // ����������йصĲ��ԣ�����Ҫ���������ļ�������ϵͳ���ݹ�����ԭ���޷��������������ʶ���Ҫ���������ļ�������ٲ��Է�װ��
    Compilation(&m_unitlist, m_objectlist, &unitListRunTime);

    for (const auto punit : unitlist) {
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)punit;
        if (pCpt->IsEncapsulable()) {
          for (int i = 0; i < 16; i++) {
            if (pCpt->IsParaLinked(i)) {
              // ˳�����Դ˺�����
              EXPECT_TRUE((pCpt->GetDynLinkType(i) & pCpt->GetParaType(i)));
            }
          }
          EXPECT_TRUE(pCpt->IsEncapsulated());
          //EXPECT_TRUE(pCpt->HaveParameter());
        }
      }
    }

    // �������еĵ�Ԫ��������
    for (const auto punit : unitlist) {
      EXPECT_GT(punit->GetExectivePriority(), 0);
      EXPECT_TRUE(punit->IsCompiled());
      if (!punit->IsHaveSourceUnit()) EXPECT_EQ(punit->GetExectivePriority(), 1);
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { 
        if (punit->mtest_fEncapsulating) {// ��װ�еĲ�����Ӧ�÷�װ��
          EXPECT_TRUE(punit->IsEncapsulated());
          EXPECT_TRUE(punit->IsEncapsulable());  
        }
      }
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////
  //
  // �������װ�йصĲ��Զ�����������Ͱ��һ����������֤��
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

    // ���ñ�������֮ǰ������
    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, listTotalUnit);

    SetEncapsulatingFlag(listTotalUnit);

    // ���Բ����Ƿ���ȷ�����˷�װ�б�־
    for (const auto punit : listTotalUnit) {
      EXPECT_FALSE(punit->IsCompiled()) << "���ú�����־Ϊ��";
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        CUnitComponent * pCpt;
        pCpt = (CUnitComponent *)punit;
        if (pCpt->IsEncapsulated() || !(pCpt->IsEncapsulable())) {
          EXPECT_FALSE(pCpt->IsEncapsulating()) << "��װ���򲻿ɷ�װ�Ĳ����������ٴη�װ";
          EXPECT_FALSE(pCpt->mtest_fEncapsulating);
        }
        else {
          EXPECT_TRUE(pCpt->IsEncapsulating()) << "δ��װ�Ĳ�����ʱ���ڷ�װ��";
          EXPECT_TRUE(pCpt->mtest_fEncapsulating);
        }
      }
    }

    // ����Ӳ������������Ķ�̬�������ݵ��������Ա���װ����ʱ���Խӿڲ����������Ƿ���ȷ
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
    //�ټ��ϼ��㲿�����������������������������͵ó������ܹ�������������ݵ�����
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
                EXPECT_EQ((pCUCP->GetParaType(j) & (tINPUT | tOUTPUT)), tINPUT); // ��������ͣ����������͡�
              }
            }
          }
        }
      }
    }

    // ��Encapsulationչ�����ֶ�ִ�кͲ���
    CUnitList * pUnitList;
    for (const auto pcunit1 : listTotalUnit) {
      if (pcunit1->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCUCP = (CUnitComponent *)pcunit1;
        if (pCUCP->IsEncapsulable()) { // �ɷ�װ�����Ķ���
          if (pCUCP->IsEncapsulated()) continue;
          
          // ���ȷ�װ�²㲿��
          pCUCP->EncapsulateBelowComponent(listTotalUnit);

          // ��֤�²㲿������װ�ˣ�Ŀǰֻ����֤����һ�㣬�����µ���δ�ܲ��ԣ�ֻ�ܿ����Ĳ������еĲ�������װ���������֤��
          pUnitList = pCUCP->GetUnitList();
          for (const auto punit : *pUnitList) {
            if (punit->IsEncapsulable()) {
              EXPECT_TRUE(punit->IsEncapsulated()) << "����һ��Ĳ�������װ��";
            }
          }

          // ��鲿��״̬�Ƿ���ȷ�� �˺����������������֤�ģ�����Ҫ�ٴμ���
          pCUCP->CheckComponentSelf();
          
          // ����������������͵����ڲ�Դ��Ԫ������һ���µĶ�̬���ӣ�Ŀ�ĵ�ԪΪ��������
          // �˲���Ҫ���ڴ������������Ĺ���ִ�С�
          pCUCP->CreateNewDynLinkFromInterfaceOutputTypePara();

          // ��������Ͳ��������¶�̬���ӵ����
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
                EXPECT_EQ(pDL->GetDestUnit(), pCUCP) << "����Ͳ��������¶�̬����ʱ��Ŀ�ĵ�Ԫ���Ǳ�����";
                EXPECT_EQ(pDL->GetSrcUnit(), punit5);
                EXPECT_EQ(pDL->GetDestIndex(), l);
                EXPECT_EQ(pDL->GetSrcIndex(), pCUCP->GetParaSrcIndex(l));
                EXPECT_EQ(pDL->GetDynLinkType(), pCUCP->GetDynLinkType(l));
                EXPECT_EQ(pDL->GetDynLinkClass(), UNIT_TO_UNIT) << "���ɵ��¶�̬����������ΪUNIT_TO_UNIT";
                EXPECT_EQ(pCUCP->GetParaDestIndex(l), -1) << "����Ͳ�����Ŀ��������������";
                EXPECT_EQ(pCUCP->GetParaDestUnit(l), nullptr) << "����Ͳ�����Ŀ�ĵ�ԪΪ��";
              }
            }
          }

          ////////////////////////////////////////////////////////////////////////////////////////////////////
          // ����ӿڲ��������ڲ������ĺ��ⲿ����Ķ�̬�������ӵ�����������

          // Ѱ���Ƿ�������뱾�����Ķ�̬����
          pCUCP->HandleTheDynLinkedInComponent(listTotalUnit);

          // ���������Ͳ����ķ�װ���
          for (int k = 0; k < 16; k++) {
            if (pCUCP->IsParaLinked(k)) {
              if (((pCUCP->GetParaType(k) & (tINPUT | tOUTPUT)) == tINPUT) && (pCUCP->GetParaSrcUnit(k) != nullptr)) {
                CUDLList *pDLList = pCUCP->GetParaSrcUnit(k)->GetDynLinkList();
                for (const auto pDL11 : *pDLList) {
                  if (pDL11->GetDestUnit() == pCUCP) { // �ҵ�Դ��Ԫ�Ķ�̬������
                    CUnitBase * pSrcUnit = pDL11->GetSrcUnit();
                    if (pSrcUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
                      EXPECT_EQ(pDL11->GetDynLinkClass(), UNIT_TO_UNIT) << "Դ��ԪΪ�������䶯̬��������ΪCOMPONENT_TO_COMPONENT";
                    }
                    else if (pSrcUnit->GetComponentUpper() != nullptr) {
                      CUnitList * pUnitList = pSrcUnit->GetComponentUpper()->GetUnitList();
                      if (find(pUnitList->begin(), pUnitList->end(), pSrcUnit->GetComponentUpper()) != pUnitList->end()) { // ������Ԫ�Ĳ���λ�ڱ���Ԫ������
                        EXPECT_EQ(pDL11->GetDynLinkClass(), COMPONENT_TO_UNIT) << "����Դ��Ԫ�Ĳ���λ�����ϲ㵥Ԫ������";
                        if (pSrcUnit->GetComponentUpper()->IsEncapsulable()) {
                          EXPECT_FALSE(pSrcUnit->GetComponentUpper()->IsEncapsulated()) << "������δ��װ";
                        }
                      }
                      EXPECT_EQ(pDL11->GetDynLinkClass(), UNIT_TO_UNIT) << "Դ��ԪΪ�򵥵�Ԫ";
                    }
                    else {
                      EXPECT_EQ(pDL11->GetDynLinkClass(), UNIT_TO_UNIT) << "Դ��ԪΪ�򵥵�Ԫ";
                      EXPECT_EQ(pSrcUnit->GetComponentUpper(), nullptr) << "Դ��Ԫλ�����ϲ㣬û�в���������";
                    }
                  }
                }
              }
            }
          }

          EXPECT_EQ((pCUCP->GetDynLinkList())->size(), 0); // ��������Ͷ�̬����ǰ����������Ķ�̬������������
          // Ѱ���Ƿ���������������Ķ�̬���ӡ�
          // �ڱ�����������һ���µĶ�̬���ӣ��������Ķ�̬���Ӹ�����µĶ�̬���ӣ���Դ��Ԫ��Ϊ��������
          // �����ڲ�Դ��Ԫ��Ŀ�ĵ�Ԫ����Ϊ�����������ò��������е�Ŀ�ĵ�Ԫָ���Ŀ�Ĳ���������
          pCUCP->HandleTheDynLinkedfromComponent();

          // ���Դ�������Ͷ�̬�����Ƿ���ȷ
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
                    EXPECT_FALSE(1); // û�ҵ���ֺ��Ŀ�ĵ�Ԫ���д���
                  }
                  else {
                    EXPECT_EQ(pDL->GetDynLinkType(), pDL2->GetDynLinkType());
                    if (pDL->GetDynLinkClass() == COMPONENT_TO_UNIT) {
                      EXPECT_EQ(pDL2->GetDynLinkClass(), UNIT_TO_UNIT);
                    }
                    else {
                      EXPECT_EQ(pDL2->GetDynLinkClass(), UNIT_TO_COMPONENT) << strFileName << "�������COMPONENT_TO_COMPONENT��ΪUNIT_TO_COMPONENT";
                    }
                  }
                }
              }
            }
          }

          // Ѱ���Ƿ��ڲ�������֮�����������ӣ�����Ӧ���ò�������Ľضϱ�־.��ʱ��δ���÷�װ��־
          // ����������ñ��������ڶ����������ڲ���̬������δ���úã�������ԡ�
          pCUCP->SetInnerDataLinkFlag();

          // ����SetInnerDataLinkFlag()���Ͳ��ⲽ�ˣ�����β����أ�

          // ���ôӱ������������������ݵ��ڲ���Ԫ��ִ�����ȼ�
          pCUCP->SetMyUnitListExectivePriority();

          pCUCP->SetEncapsulateFlag(true);
        }
        else { // ���ɷ�װ�����Ķ���
          // ���ȷ�װ�²㲿��
          pCUCP->EncapsulateBelowComponent(listTotalUnit);

          // �����Ƿ��²㲿������װ��
          pUnitList = pCUCP->GetUnitList();
          for (const auto punit : *pUnitList) {
            if (punit->IsEncapsulable()) {
              EXPECT_TRUE(punit->IsEncapsulated()) << "����һ��Ĳ�������װ��";
            }
          }

          // ��鲿��״̬�Ƿ���ȷ
          //pCUCP->CheckComponentSelf();
          // ȷ�����ɷ�װ����������ʱ��Ԫ����Ϊ��
          EXPECT_TRUE(pCUCP->GetRunTimeUnitList()->empty()) << strFileName << "���ɷ�װ��������ʱ��Ԫ����Ӧ��Ϊ��";
        }
      }
    }

    // ���Կɷ�װ��������װ�ˣ������ɷ�װ�Ĳ����⣩
    for (const auto punit : listTotalUnit) {
      if (punit->IsEncapsulable()) { // �ɷ�װ������
        EXPECT_TRUE(punit->IsEncapsulated()) << "�ɷ�װ����Ӧ���Ѿ�������װ��";
      }
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent)) ) {
        if (!((CUnitComponent *)punit)->IsPermitEncapsulation() ) {
          EXPECT_TRUE(((CUnitComponent *)punit)->GetRunTimeUnitList()->empty()) << strFileName << "���ɷ�װ��������ʱ��Ԫ����Ӧ��Ϊ��";
        }
      }
    }

    CUnitList unitlist;
    CreateUniUnitList(&m_unitlist, unitlist);
    // ��������ɵĵ�Ԫ����
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
            EXPECT_TRUE(fFound) << "ִ�����ȼ�Ϊ1�ĵ�Ԫ���������ݴӲ������������";
          }
          else {
            EXPECT_TRUE(false);
          }
        }
      }
      if (punit->GetExectivePriority() > 1) {
        EXPECT_TRUE(punit->IsHaveSourceUnit() || punit->IsCutoff());
      }
      EXPECT_GT(punit->GetExectivePriority(), 0) << "ִ�����ȼ�����1�ĵ�Ԫ��Դ��Ԫ���������룩";

      if (punit->IsEncapsulable()) {
        EXPECT_TRUE(punit->IsEncapsulated()) << "��װ�������";
      }
    }

    //���Բ����������������������
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
          EXPECT_EQ(lSrc, pCUCP->m_lOutputParaNumber) << "����Ͳ��� = ����Ͷ�̬���� + ��δ���ӵ�����Ͳ���";
          EXPECT_EQ(lDest, pCUCP->m_lInputParaNumber) << "�����Ͳ��� = �����Ͷ�̬���� + ��δ���ӵ������Ͳ���";
        }
      }
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////
  //
  // ���������Ĳ��Խ��Ӧ����ȫһ��������������Ϊ�Աȡ�
  // TestEncapsulation1��EncapsulateUnitList���Ϊ���ڲ�ʹ�ü�������˳��ִ�С�
  //
  ////////////////////////////////////////////////////////////////////////////////////
  TEST_P(TestCompile, TestEncapsulation20) {

    CUnitList listTotalUnit, runtimeUnitList;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;

    // ���ñ�������֮ǰ������
    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, listTotalUnit);

    SetEncapsulatingFlag(listTotalUnit);
    
    // ��װ��Ԫ�����еĿɷ�װ����
    EncapsulateUnitList(listTotalUnit);

    CUnitList unitlist;
    CreateUniUnitList(&m_unitlist, unitlist);
    // ����˵�Ԫ����
    CompileUnitList(&unitlist, &runtimeUnitList);

    for (const auto punit : listTotalUnit) {
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        if (punit->IsEncapsulable()) { // �ɷ�װ������
          EXPECT_TRUE(punit->IsEncapsulated()) << "�ɷ�װ����Ӧ���Ѿ�������װ��";
        }
      }
    }

    EXPECT_EQ(runtimeUnitList.size(), unitlist.size()) << "����ʱ��Ԫ�����뱾��Ԫ���е���������";
    unitlist.clear();

    // ������еĲ����ڲ���Ԫ���������ȼ�
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
            EXPECT_TRUE(fFound) << "ִ�����ȼ�Ϊ1�ĵ�Ԫ���������ݴӲ������������";
          }
          else {
            EXPECT_TRUE(false);
          }
        }
      }
      if (punit->GetExectivePriority() > 1) {
        EXPECT_TRUE(punit->IsHaveSourceUnit() || punit->IsCutoff());
      }
      EXPECT_GT(punit->GetExectivePriority(), 0) << "ִ�����ȼ�����1�ĵ�Ԫ��Դ��Ԫ���������룩";

      if (punit->IsEncapsulable()) {
        EXPECT_TRUE(punit->IsEncapsulated()) << "��װ�������";
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

    // ��װ��Ԫ�����еĿɷ�װ����
    EncapsulateUnitList(listTotalUnit);
    listTotalUnit.clear();

    CreateUniUnitList(&m_unitlist, listTotalUnit); // ��ʱ�����Ѿ���װ���ʶ����ɵ�listTotalUnitҪ����֮ǰ���ɵĵ�Ԫ����

    // ����˵�Ԫ����
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
        if (!pcunit2->IsCutoff()) { // û�����ýضϵ�Ŀ�ĵ�Ԫ��ִ�����ȼ�����С��Դ��Ԫ
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
    , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ������.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ�.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ����������.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ㲿��֮�����Ӳ���������.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\��װ��Ĳ���.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ�����������з�װ��Ĳ���.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ�����������з�װ��Ķ������.sqi"
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
              // ˳�����Դ˺�����
              EXPECT_FALSE((pCpt->GetDynLinkType(j) == pCpt->GetParaType(j)));
              if ((pCpt->GetParaType(j) & (tINPUT | tOUTPUT)) == tINPUT) {
                for (int k = 0; k < 16; k++) {
                  if (pCpt->IsInnerDataLinked(j, k)) {
                    EXPECT_EQ(pCpt->GetParaType(k) & (tINPUT | tOUTPUT), tOUTPUT)
                      << "���Բ����ڲ���������ʱĿ�Ĳ�������ӦΪ�����";
                    EXPECT_TRUE(pCpt->GetParaDestUnit(k) == nullptr)
                      << "����Ͳ�����Ŀ�ĵ�Ԫָ����ԶΪnullptr" << "  k = " << k;
                    EXPECT_EQ(-1, pCpt->GetParaDestIndex(k)) << "Ŀ�ĵ�Ԫ������������Ϊ������";
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
    EXPECT_GT(l, 1) << "����CheckInnerDataLink����ʱ���������ļ��еĲ�����������������Ͳ���������һ���ڲ���Ԫ";

    // clear up
    ReleaseSQIFile(&m_unitlist, &m_objectlist);
    EXPECT_TRUE(m_unitlist.empty());
    EXPECT_TRUE(m_objectlist.empty()); 

  }

}
