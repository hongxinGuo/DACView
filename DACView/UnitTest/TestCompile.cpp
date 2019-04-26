#include"stdafx.h"

#include"CUnitBase.h"
#include"CUnitComponent.h"
#include"SQIFileDoc.h"

#include"compileUnitList.h"

#include"pch.h"

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
			EXPECT_TRUE(m_unitlist.empty());
			EXPECT_TRUE(m_objectlist.IsEmpty());
		}

	public:
		CUnitList m_unitlist;
		CObjectList m_objectlist;
		CString strFileName;
  };

  INSTANTIATE_TEST_CASE_P(TestCompile, TestCompile,
    testing::Values("c:\\DACViewTestFile\\CompileUnitList\\�򵥵�Ԫ.sqi"
      , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ����.sqi"
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

    ));

  // �������õ�Ԫ������־
  TEST_P(TestCompile, TestSetParaSelectedFlag) {
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitList UnitList;

    // ���Է�װ��Ĳ���������־������
    for (const auto pctemp : m_unitlist) {
      if (pctemp->IsEncapsulable() && pctemp->IsEncapsulated()) {
        EXPECT_TRUE(pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent)));
        CUnitList *pUList2 = ((CUnitComponent *)pctemp)->GetUnitList();
        for (const auto pcunit2 : *pUList2) {
          EXPECT_FALSE(pcunit2->IsCompiled()); // ֻ�����һ�㣬������Ĳ�û����֤��
        }
      }
    }

    ReSetCompileFlag(&m_unitlist);

    // ���Ա����־�Ƿ�����
    for (const auto pcunit : m_unitlist) {
      if (!pcunit->IsEncapsulated()) {
        EXPECT_FALSE(pcunit->IsCompiled()) << "���ú�����־Ϊ��";
      }
      EXPECT_FALSE(pcunit->IsHaveSourceUnit()) << "���ú���Դ����Ϊ0";
      EXPECT_EQ(pcunit->GetExectivePriority(), 0) << "���ú�ִ�����ȼ�Ϊ0";
    }

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, UnitList);
    // û��ʲô�õĲ��Ժ�������ʱʲôҲ�����ˡ�

    // �������ò���ѡ���־�Ƿ���ȷ��ʹ�����ɵ�UnitList��Ϊ����
    int iDynLinkToNumber, iDynLinkToNumber2;
    for (const auto pcunit : UnitList) {
      EXPECT_EQ(pcunit->GetExectivePriority(), 0) << "����ǰִ�����ȼ�Ϊ0";
      iDynLinkToNumber = pcunit->GetInputParameterNumber(); 
      iDynLinkToNumber2 = 0;
      for (const auto pctemp : UnitList) {
        if (pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
          CUnitComponent * pCpt = (CUnitComponent *)pctemp;
          if (!pCpt->IsEncapsulated()) { // �����δ��װ�Ĳ����������Ƿ���ڲ��������������ڲ���Ԫ�����
            for (int l = 0; l < 16; l++) {
              if (pCpt->IsParaLinked(l)) {
                if (pCpt->GetParaDestUnit(l) != nullptr) { // �Ӳ��������������ڲ���Ԫ
                  EXPECT_EQ(pCpt->GetParaSrcUnit(l), nullptr) << "�������������������ڲ���Ԫʱ��Դ��Ԫ��������������Ϊnullptr" << pCpt->GetName();
                  if (pCpt->GetParaDestUnit(l) == pcunit) iDynLinkToNumber2++;
                }
              }
            }
          }
        }
        CUDLList * pDLList = pctemp->GetDynLinkList();
        for (const auto pDL : *pDLList) {
          if (pDL->GetDestUnit() == pcunit) iDynLinkToNumber2++;
        }
      }
      EXPECT_EQ(iDynLinkToNumber, iDynLinkToNumber2) << "���붯̬���������������� " << strFileName << "  " << pcunit->GetName();
    }
  }

  // ����������Դ��Ԫ��ִ�����ȼ�Ϊ1
  TEST_P(TestCompile, TestSetNoSrcUnitExectivePriority) {
    CUnitList rtUnitList;
    CUnitComponent * pCpt = nullptr;
    INT64 iCurrentUnit = 0;

    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, rtUnitList);

    SetNoSrcUnitExectivePriority(&rtUnitList);

    for (const auto pcunitTemp : rtUnitList) {
      if (pcunitTemp->GetExectivePriority() == 1) {
        EXPECT_TRUE(!pcunitTemp->IsHaveSourceUnit()) << "ִ�����ȼ�Ϊ1�ĵ�Ԫû����������";
      }
      if (pcunitTemp->GetExectivePriority() > 1) {
        EXPECT_TRUE(pcunitTemp->IsHaveSourceUnit()) << "ִ�����ȼ�����1�ĵ�Ԫ��Դ��Ԫ���������룩";
      }
      if (pcunitTemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        if (!((CUnitComponent *)pcunitTemp)->IsEncapsulated()) { // ��ʱ������δ�����룬�ʶ���װ�Ĳ������Ѿ���װ�˵ģ�������
          EXPECT_EQ(pcunitTemp->GetExectivePriority(), 1) << "��������Դ��Ԫ��ִ�����ȼ�ʱ������ִ�����ȼ���Զ����Ϊ1";
        }
      }
    }
  }

  // ���Ա��뵥Ԫ����
  TEST_P(TestCompile, TestExectiveCompilation) {
    CUnitList listUnit, runtimeUnitList;
    CUnitComponent * pCpt = nullptr;

    CUnitBase *pcunitTemp;

    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, listUnit);

    SetNoSrcUnitExectivePriority(&listUnit);

    for (const auto pcunit : runtimeUnitList) {
      EXPECT_FALSE(pcunit->IsCompiled()) << "��ʱ��Ԫ������δ����" << pcunit->GetName();
    }

    ExectiveCompilation(listUnit, &runtimeUnitList);

    for (const auto pcunitTemp : listUnit) {
      if (pcunitTemp->GetExectivePriority() == 1) {
        EXPECT_TRUE(!pcunitTemp->IsHaveSourceUnit()) << "ִ�����ȼ�Ϊ1�ĵ�Ԫû����������";
      }
      if (pcunitTemp->GetExectivePriority() > 1) {
        EXPECT_TRUE(pcunitTemp->IsHaveSourceUnit());
      }
      EXPECT_GT(pcunitTemp->GetExectivePriority(), 0) << "ִ�����ȼ�����1�ĵ�Ԫ��Դ��Ԫ���������룩";
      CUDLList * pUDLList = pcunitTemp->GetDynLinkList();
      for (const auto pDL : *pUDLList) {
        EXPECT_LT(pcunitTemp->GetExectivePriority(), pDL->GetDestUnit()->GetExectivePriority()) << "��̬�������ȼ�����"
          << strFileName << "  " << pcunitTemp->GetName() << "  " << pDL->GetDestUnit()->GetName();
      }
    }

    pcunitTemp = runtimeUnitList.front();
    for (const auto pcunit : runtimeUnitList) {
      EXPECT_TRUE(pcunit->IsCompiled()) << "��ʱ��Ԫ���ж�������" << pcunit->GetName();
      EXPECT_LE(pcunitTemp->GetExectivePriority(), pcunit->GetExectivePriority()) << "����ʱ��Ԫ����ִ�����ȼ����д���";
      pcunitTemp = pcunit;
    }
  }

  TEST_P(TestCompile, TestEncapsulateUnitList) {
    CUnitList listUnit, runtimeUnitList;
    CUnitComponent * pCpt = nullptr;
    INT64 iCurrentUnit = sizeof(ULONG);

    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, listUnit);

    SetNoSrcUnitExectivePriority(&listUnit);

    ExectiveCompilation(listUnit, &runtimeUnitList);

    EncapsulateUnitlist(&listUnit, listUnit);

    EXPECT_EQ(runtimeUnitList.size(), listUnit.size()) << strFileName << "����ʱ��Ԫ���������е�Ԫ����������";

    for (const auto pcunitTemp : listUnit) {
      if (pcunitTemp->GetExectivePriority() == 1) {
        EXPECT_TRUE(!pcunitTemp->IsHaveSourceUnit()) << "ִ�����ȼ�Ϊ1�ĵ�Ԫû����������";
      }
      if (pcunitTemp->GetExectivePriority() > 1) {
        EXPECT_TRUE(pcunitTemp->IsHaveSourceUnit());
      }
      EXPECT_GT(pcunitTemp->GetExectivePriority(), 0) << "ִ�����ȼ�����1�ĵ�Ԫ��Դ��Ԫ���������룩";

      if (pcunitTemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        if (((CUnitComponent *)pcunitTemp)->IsEncapsulable()) {
          EXPECT_TRUE(pcunitTemp->IsEncapsulated()) << "��װ�������";
        }
      }
    }
  }

  TEST_P(TestCompile, TestCompile) {
    CUnitBase *pcunitTemp;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;

    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    for (const auto pctemp : m_unitlist) {
      if (pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)pctemp;
        if (!pCpt->IsPermitEncapsulation()) {
          EXPECT_EQ(pCpt->GetRunTimeUnitList()->size(), 0); // ���ɷ�װ������ִ�е�Ԫ�����ǿյ�
        }
      }
    }

    CUnitList rtUnitList;

    if (!UnitListLoopDetect(&m_unitlist)) {   // ���û�з��ֶ�̬����ѭ��
      CompileUnitList(&m_unitlist, &rtUnitList);
    }
    else ASSERT_TRUE(0); // ��ѭ�����ֵĻ������˳����ԡ�

    pcunitTemp = rtUnitList.front();
    for (const auto pcunit : rtUnitList) {
      EXPECT_LE(pcunitTemp->GetExectivePriority(), pcunit->GetExectivePriority()) << "����ʱ��Ԫ����ִ�����ȼ����д���";
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
        if ((k >= l) && (!pctemp->IsSetCutOff()) && (!pDestUnit->IsSetCutOff())) { //���ȼ��������⣿
          if (pctemp->GetComponentUpper() != pDestUnit) { //��װ���ڲ���Ԫ�������������������������ʱ������ִ�����ȼ����ڲ���Ԫ��ִ�����ڲ���ִ��֮��
            EXPECT_FALSE(0) << "��̬����ִ�����ȼ����ִ���" << pctemp->GetName() << "  " << pDestUnit->GetName();
          }
        }
      }
    }

    // 
    for (const auto pctemp : m_unitlist) {
      if (pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)pctemp;
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


  TEST_P(TestCompile, TestComponentEncapsulation) {
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;
    CUnitList unitListRunTime;
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);


    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    // ����������йصĲ��ԣ�����Ҫ���������ļ�������ϵͳ���ݹ�����ԭ���޷��������������ʶ���Ҫ���������ļ�������ٲ��Է�װ��
    CompileUnitList(&m_unitlist, &unitListRunTime);
    int i = 0;
    for (const auto pcunitTemp : m_unitlist) {
      i++;
      if (pcunitTemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)pcunitTemp;
        if (pCpt->IsEncapsulable()) {
          for (int j = 0; j < 16; j++) {
            if (pCpt->IsParaLinked(i)) {
              // ˳�����Դ˺�����
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

    SetNoSrcUnitExectivePriority(&listTotalUnit);

    ExectiveCompilation(listTotalUnit, &runtimeUnitList);

    // ��Encapsulationչ�����ֶ�ִ�кͲ���
    CUnitList * pUnitList;
    for (const auto pcunit : m_unitlist) {
      if (pcunit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCUCP = (CUnitComponent *)pcunit;
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

          // �����ڲ���Ԫ���е�����ʱ��Ԫ���С�����֮ǰ�²㲿�����Ѿ���װ�ˣ���ʱ������ʱ��Ԫ���еĸ��������ڲ���Ԫ�ĸ�����
          pCUCP->CreateRunTimeUnitList();

          // �������ɵ�����ʱ��Ԫ�������ڲ���Ԫ���е���������ͬ�ģ���װ����ǣ�������ִ�����ȼ����������У�������ͬ����
          CUnitList * pRTUnitList = pCUCP->GetRunTimeUnitList();
          EXPECT_EQ(pUnitList->size(), pRTUnitList->size()) << "�²㲿����װ�������ʱ��Ԫ���У��䵥Ԫ�������ڲ���Ԫ���е����";
          INT64 kTotal = pRTUnitList->size();
          CUnitBase *punitTemp2 = pRTUnitList->front();
          if (kTotal > 1) {
            for (const auto punitTemp3 : *pRTUnitList) {
              EXPECT_LE(punitTemp2->GetExectivePriority(), punitTemp3->GetExectivePriority()) << "��Ԫ���з�װ����ִ�����ȼ���������";
              punitTemp2 = punitTemp3;
            }
          }

          // ����������������͵����ڲ�Դ��Ԫ������һ���µĶ�̬���ӣ�Ŀ�ĵ�ԪΪ��������
          // �˲���Ҫ���ڴ������������Ĺ���ִ�С�
          pCUCP->CreateNewDynLinkFromInterfaceOutputTypePara();

          // ��������Ͳ��������¶�̬���ӵ����
          for (int l = 0; l < 16; l++) {
            if (pCUCP->IsParaLinked(l)) {
              if ((pCUCP->GetParaType(l) & (tINPUT | tOUTPUT)) == tOUTPUT) {
                CUnitBase * punit11 = pCUCP->GetParaSrcUnit(l);
                CUDLList * pUDLList = punit11->GetDynLinkList();
                kTotal = pUDLList->size();
                auto itDL1 = pUDLList->begin();
                shared_ptr<CUnitDynLink> pDL = *itDL1;
                while (pDL->GetDestUnit() != pCUCP) {
                  pDL = *++itDL1;
                }
                EXPECT_EQ(pDL->GetDestUnit(), pCUCP) << "����Ͳ��������¶�̬����ʱ��Ŀ�ĵ�Ԫ���Ǳ�����";
                EXPECT_EQ(pDL->GetSrcUnit(), punit11);
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
                    else if (find(m_unitlist.begin(), m_unitlist.end(), pSrcUnit->GetComponentUpper()) != m_unitlist.end()) { // ������Ԫ�Ĳ���λ�ڱ���Ԫ������
                      EXPECT_EQ(pDL11->GetDynLinkClass(), COMPONENT_TO_UNIT) << "����Դ��Ԫ�Ĳ���λ�����ϲ㵥Ԫ������";
                      if (pSrcUnit->GetComponentUpper()->IsEncapsulable()) {
                        EXPECT_FALSE(pSrcUnit->GetComponentUpper()->IsEncapsulated()) << "������δ��װ";
                      }
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

          // ���ñ�������ִ�����ȼ�
          pCUCP->SetMyselfExectivePriority();

          // �����Ƿ����ú��˱�������ִ�����ȼ�
          INT32 iComponentExectivePriority = 0;
          for (int i = 0; i < 16; i++) {
            if (pCUCP->IsParaLinked(i)) {
              if (((pCUCP->GetParaType(i)) & tINPUT) && (pCUCP->IsParameterLocked(i))) { // �����Ͳ����Ҵ���Դ��Ԫ��ֻ���ϲ㵥Ԫ��������
                if ((pCUCP->GetParaSrcUnit(i))->GetExectivePriority() > iComponentExectivePriority) {
                  iComponentExectivePriority = (pCUCP->GetParaSrcUnit(i))->GetExectivePriority();
                }
              }
            }
          }
          EXPECT_EQ(iComponentExectivePriority + 1, pCUCP->GetExectivePriority());

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
          // ����������ñ��������һ���������ڲ���̬������δ���úã�������ԡ�
          pCUCP->SetInnerDataLinkFlag();

          // ���ԡ��Ͳ��ⲽ�ˣ�����β����أ�

          pCUCP->SetCompiledFlag(true);
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
          pCUCP->CheckComponentSelf();
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

    EXPECT_EQ(runtimeUnitList.size(), listTotalUnit.size()) << "����ʱ��Ԫ���������е�Ԫ����������";

    for (const auto pcunitTemp : listTotalUnit) {
      if (pcunitTemp->GetExectivePriority() == 1) {
        EXPECT_TRUE(!pcunitTemp->IsHaveSourceUnit()) << "ִ�����ȼ�Ϊ1�ĵ�Ԫû����������";
      }
      if (pcunitTemp->GetExectivePriority() > 1) {
        EXPECT_TRUE(pcunitTemp->IsHaveSourceUnit());
      }
      EXPECT_GT(pcunitTemp->GetExectivePriority(), 0) << "ִ�����ȼ�����1�ĵ�Ԫ��Դ��Ԫ���������룩";

      if (pcunitTemp->IsEncapsulable()) {
        EXPECT_TRUE(pcunitTemp->IsEncapsulated()) << "��װ�������";
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
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;


    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, listTotalUnit);

    SetNoSrcUnitExectivePriority(&listTotalUnit);

    ExectiveCompilation(listTotalUnit, &runtimeUnitList);

    EncapsulateUnitlist(&m_unitlist, listTotalUnit);

    for (const auto punit : listTotalUnit) {
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        if (punit->IsEncapsulable()) { // �ɷ�װ������
          EXPECT_TRUE(punit->IsEncapsulated()) << "�ɷ�װ����Ӧ���Ѿ�������װ��";
        }
      }
    }

    EXPECT_EQ(runtimeUnitList.size(), listTotalUnit.size()) << "����ʱ��Ԫ���������е�Ԫ����������";
    // ������еĲ����ڲ���Ԫ���������ȼ�
    for (const auto pcunitTemp : listTotalUnit) {
      if (pcunitTemp->GetExectivePriority() == 1) {
        EXPECT_TRUE(!pcunitTemp->IsHaveSourceUnit()) << "ִ�����ȼ�Ϊ1�ĵ�Ԫû����������";
      }
      if (pcunitTemp->GetExectivePriority() > 1) {
        EXPECT_TRUE(pcunitTemp->IsHaveSourceUnit());
      }
      EXPECT_GT(pcunitTemp->GetExectivePriority(), 0) << "ִ�����ȼ�����1�ĵ�Ԫ��Դ��Ԫ���������룩";

      if (pcunitTemp->IsEncapsulable()) {
        EXPECT_TRUE(pcunitTemp->IsEncapsulated()) << "��װ�������";
      }
    }
  }

  TEST_P(TestCompile, TestSetExectivePriority) {
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;

    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CUnitList rtUnitList;

    EXPECT_FALSE(UnitListLoopDetect(&m_unitlist));  // ���û�з��ֶ�̬����ѭ��
    CompileUnitList(&m_unitlist, &rtUnitList);

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
      for (const auto pUnitDynLink : *pDLList) {
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

    // �ڱ��뵥Ԫ����ǰ���������ñ����־�������������
    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CompileUnitList(&m_unitlist, &unitListRunTime);

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
    EXPECT_TRUE(m_objectlist.IsEmpty());

  }

}
