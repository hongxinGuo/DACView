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
    INT64 iTotal;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitBase * pctemp, *pcunit;
    CUnitList UnitList;

    // ���Է�װ��Ĳ���������־������
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
          EXPECT_FALSE(pcunit2->IsCompiled()); // ֻ�����һ�㣬������Ĳ�û����֤��
        }
      }
    }

    ReSetCompileFlag(&m_unitlist);

    // ���Ա����־�Ƿ�����
    POSITION po;
    poUnit = m_unitlist.GetHeadPosition();
    for (int i = 0; i < iTotal; i++) {
      pcunit = m_unitlist.GetNext(poUnit);
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
    poUnit = UnitList.GetHeadPosition();
    iTotal = UnitList.GetCount();
    int iDynLinkToNumber, iDynLinkToNumber2;
    for (int i = 0; i < iTotal; i++) {
      pcunit = UnitList.GetNext(poUnit);
      EXPECT_EQ(pcunit->GetExectivePriority(), 0) << "����ǰִ�����ȼ�Ϊ0";
      iDynLinkToNumber = pcunit->GetInputParameterNumber(); 
      po = UnitList.GetHeadPosition();
      iDynLinkToNumber2 = 0;
      for (int j = 0; j < iTotal; j++) {
        pctemp = UnitList.GetNext(po);
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
        POSITION poDL = pDLList->GetHeadPosition();
        INT64 kTotal = pDLList->GetCount();
        for (int k = 0; k < kTotal; k++) {
          CUnitDynLink * pDL = pDLList->GetNext(poDL);
          if (pDL->GetDestUnit() == pcunit) iDynLinkToNumber2++;
        }
      }
      EXPECT_EQ(iDynLinkToNumber, iDynLinkToNumber2) << "���붯̬���������������� " << strFileName << "  " << pcunit->GetName();
    }
  }

  // ����������Դ��Ԫ��ִ�����ȼ�Ϊ1
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
      EXPECT_FALSE(pcunit->IsCompiled()) << "��ʱ��Ԫ������δ����" << pcunit->GetName();
    }

    ExectiveCompilation(listUnit, &runtimeUnitList);

    poUnit = listUnit.GetHeadPosition();
    iTotal = listUnit.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pcunitTemp = listUnit.GetNext(poUnit);
      if (pcunitTemp->GetExectivePriority() == 1) {
        EXPECT_TRUE(!pcunitTemp->IsHaveSourceUnit()) << "ִ�����ȼ�Ϊ1�ĵ�Ԫû����������";
      }
      if (pcunitTemp->GetExectivePriority() > 1) {
        EXPECT_TRUE(pcunitTemp->IsHaveSourceUnit());
      }
      EXPECT_GT(pcunitTemp->GetExectivePriority(), 0) << "ִ�����ȼ�����1�ĵ�Ԫ��Դ��Ԫ���������룩";
      CUDLList * pUDLList = pcunitTemp->GetDynLinkList();
      INT64 jTotal = pUDLList->GetCount();
      POSITION poDL = pUDLList->GetHeadPosition();
      CUnitDynLink * pDL;
      for (int j = 0; j < jTotal; j++) {
        pDL = pUDLList->GetNext(poDL);
        EXPECT_LT(pcunitTemp->GetExectivePriority(), pDL->GetDestUnit()->GetExectivePriority()) << "��̬�������ȼ�����"
          << strFileName << "  " << pcunitTemp->GetName() << "  " << pDL->GetDestUnit()->GetName();
      }
    }

    poUnit = runtimeUnitList.GetHeadPosition();
    iTotal = runtimeUnitList.GetCount();
    pcunitTemp = runtimeUnitList.GetNext(poUnit);
    for (int i = 0; i < iTotal - 1; i++) {
      pcunit = runtimeUnitList.GetNext(poUnit);
      EXPECT_TRUE(pcunit->IsCompiled()) << "��ʱ��Ԫ���ж�������" << pcunit->GetName();
      EXPECT_LE(pcunitTemp->GetExectivePriority(), pcunit->GetExectivePriority()) << "����ʱ��Ԫ����ִ�����ȼ����д���";
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

    EXPECT_EQ(runtimeUnitList.GetCount(), listUnit.GetCount()) << strFileName << "����ʱ��Ԫ���������е�Ԫ����������";

    POSITION poUnit = listUnit.GetHeadPosition();
    iTotal = listUnit.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pcunitTemp = listUnit.GetNext(poUnit);
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
          EXPECT_EQ(pCpt->GetRunTimeUnitList()->GetCount(), 0); // ���ɷ�װ������ִ�е�Ԫ�����ǿյ�
        }
      }
    }

    CUnitList rtUnitList;

    if (!UnitListLoopDetect(&m_unitlist)) {   // ���û�з��ֶ�̬����ѭ��
      CompileUnitList(&m_unitlist, &rtUnitList);
    }
    else ASSERT_TRUE(0); // ��ѭ�����ֵĻ������˳����ԡ�

    CUnitBase * pcunit;
    POSITION poUnit = rtUnitList.GetHeadPosition();
    iTotal = rtUnitList.GetCount();
    pcunitTemp = rtUnitList.GetNext(poUnit);
    for (int i = 0; i < iTotal - 1; i++) {
      pcunit = rtUnitList.GetNext(poUnit);
      EXPECT_LE(pcunitTemp->GetExectivePriority(), pcunit->GetExectivePriority()) << "����ʱ��Ԫ����ִ�����ȼ����д���";
    }

    // ����ִ�����ȼ������еĵ�Ԫ���У����������ڲ��ĵ�Ԫ���У��Բ��ԡ�
    CUnitDynLink * pDL;
    CUDLList * pUDLList;
    int jTotal, k, l;
    POSITION poDL;
    CUnitBase * pDestUnit;

    poUnit = rtUnitList.GetHeadPosition();
    iTotal = rtUnitList.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pctemp = rtUnitList.GetNext(poUnit);
      EXPECT_TRUE(pctemp->IsCompiled()) << strFileName << "  " << pctemp->GetName() << "  ��ʱ��Ԫ����Ӧ���Ѿ���������";
      EXPECT_LE(1, pctemp->GetExectivePriority());
      pUDLList = pctemp->GetDynLinkList();
      jTotal = pUDLList->GetCount();
      poDL = pUDLList->GetHeadPosition();
      for (int j = 0; j < jTotal; j++) {
        pDL = pUDLList->GetNext(poDL);
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
    po = m_unitlist.GetHeadPosition();
    iTotal = m_unitlist.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pctemp = m_unitlist.GetNext(po);
      if (pctemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCpt = (CUnitComponent *)pctemp;
        if (pCpt->IsEncapsulable()) {
          EXPECT_TRUE(pCpt->IsEncapsulated()) << "after compile component"; // ���Բ�����װ��־
        }
        if ( !pCpt->IsPermitEncapsulation() ) {
          EXPECT_FALSE(pCpt->IsEncapsulated());
          CUnitList * pUnitList = pCpt->GetRunTimeUnitList();
          EXPECT_EQ(pUnitList->GetCount(), 0); // ���ɷ�װ������ִ�е�Ԫ�����ǿյ�
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

    // ����������йصĲ��ԣ�����Ҫ���������ļ�������ϵͳ���ݹ�����ԭ���޷��������������ʶ���Ҫ���������ļ�������ٲ��Է�װ��
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
    CUnitBase  *pcunitTemp, *pcunit;
    INT64 iTotal, jTotal, lTotal;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;
    CUDLList * pDLList = nullptr, *pDLList2 = nullptr;
    CUnitDynLink * pDL = nullptr, *pDL2 = nullptr;
    CUnitComponent * pSrcComponent = nullptr, *pDestComponent = nullptr;
    CUnitComponent * pCUCP = nullptr;

    // ���ñ�������֮ǰ������
    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CreateUniUnitList(&m_unitlist, listTotalUnit);

    // ����Ӳ������������Ķ�̬�������ݵ��������Ա���װ����ʱ���Խӿڲ����������Ƿ���ȷ
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
    //�ټ��ϼ��㲿�����������������������������͵ó������ܹ�������������ݵ�����
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
    poUnit = m_unitlist.GetHeadPosition();
    iTotal = m_unitlist.GetCount();
    CUnitList * pUnitList;
    POSITION po;
    CUnitBase * punit;
    for (int i = 0; i < iTotal; i++) {
      pcunit = m_unitlist.GetNext(poUnit);
      if (pcunit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        pCUCP = (CUnitComponent *)pcunit;
        if (pCUCP->IsEncapsulable()) { // �ɷ�װ�����Ķ���
          if (pCUCP->IsEncapsulated()) continue;
          // ���ȷ�װ�²㲿��
          pCUCP->EncapsulateBelowComponent(listTotalUnit);

          // ��֤�²㲿������װ�ˣ�Ŀǰֻ����֤����һ�㣬�����µ���δ�ܲ��ԣ�ֻ�ܿ����Ĳ������еĲ�������װ���������֤��
          pUnitList = pCUCP->GetUnitList();
          po = pUnitList->GetHeadPosition();
          jTotal = pUnitList->GetCount();
          for (int j = 0; j < jTotal; j++) {
            punit = pUnitList->GetNext(po);
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
          EXPECT_EQ(pUnitList->GetCount(), pRTUnitList->GetCount()) << "�²㲿����װ�������ʱ��Ԫ���У��䵥Ԫ�������ڲ���Ԫ���е����";
          POSITION poRT = pRTUnitList->GetHeadPosition();
          INT64 kTotal = pRTUnitList->GetCount();
          CUnitBase * punitTemp3, *punitTemp2 = pRTUnitList->GetNext(poRT);
          if (kTotal > 1) {
            for (int k = 0; k < kTotal - 1; k++) {
              punitTemp3 = pRTUnitList->GetNext(poRT);
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
                kTotal = pUDLList->GetCount();
                POSITION poDL1 = pUDLList->GetHeadPosition();
                CUnitDynLink * pDL = pUDLList->GetNext(poDL1);
                while (pDL->GetDestUnit() != pCUCP) {
                  pDL = pUDLList->GetNext(poDL1);
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
                INT64 lTotal = pDLList->GetCount();
                POSITION poDL = pDLList->GetHeadPosition();
                CUnitDynLink * pDL11;
                for (int l = 0; l < lTotal; l++) {
                  pDL11 = pDLList->GetNext(poDL);
                  if (pDL11->GetDestUnit() == pCUCP) { // �ҵ�Դ��Ԫ�Ķ�̬������
                    CUnitBase * pSrcUnit = pDL11->GetSrcUnit();
                    if (pSrcUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
                      EXPECT_EQ(pDL11->GetDynLinkClass(), UNIT_TO_UNIT) << "Դ��ԪΪ�������䶯̬��������ΪCOMPONENT_TO_COMPONENT";
                    }
                    else if (m_unitlist.Find(pSrcUnit->GetComponentUpper())) { // ������Ԫ�Ĳ���λ�ڱ���Ԫ������
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

          EXPECT_EQ((pCUCP->GetDynLinkList())->GetCount(), 0); // ��������Ͷ�̬����ǰ����������Ķ�̬������������
          // Ѱ���Ƿ���������������Ķ�̬���ӡ�
          // �ڱ�����������һ���µĶ�̬���ӣ��������Ķ�̬���Ӹ�����µĶ�̬���ӣ���Դ��Ԫ��Ϊ��������
          // �����ڲ�Դ��Ԫ��Ŀ�ĵ�Ԫ����Ϊ�����������ò��������е�Ŀ�ĵ�Ԫָ���Ŀ�Ĳ���������
          pCUCP->HandleTheDynLinkedfromComponent();

          // ���Դ�������Ͷ�̬�����Ƿ���ȷ
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
          po = pUnitList->GetHeadPosition();
          jTotal = pUnitList->GetCount();
          for (int j = 0; j < jTotal; j++) {
            punit = pUnitList->GetNext(po);
            if (punit->IsEncapsulable()) {
              EXPECT_TRUE(punit->IsEncapsulated()) << "����һ��Ĳ�������װ��";
            }
          }

          // ��鲿��״̬�Ƿ���ȷ
          pCUCP->CheckComponentSelf();
          // ȷ�����ɷ�װ����������ʱ��Ԫ����Ϊ��
          EXPECT_TRUE(pCUCP->GetRunTimeUnitList()->IsEmpty()) << strFileName << "���ɷ�װ��������ʱ��Ԫ����Ӧ��Ϊ��";
        }
      }
    }

    // ���Կɷ�װ��������װ�ˣ������ɷ�װ�Ĳ����⣩
    po = listTotalUnit.GetHeadPosition();
    jTotal = listTotalUnit.GetCount();
    for (int j = 0; j < jTotal; j++) {
      punit = listTotalUnit.GetNext(po);
      if (punit->IsEncapsulable()) { // �ɷ�װ������
        EXPECT_TRUE(punit->IsEncapsulated()) << "�ɷ�װ����Ӧ���Ѿ�������װ��";
      }
      if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent)) ) {
        if (!((CUnitComponent *)punit)->IsPermitEncapsulation() ) {
          EXPECT_TRUE(((CUnitComponent *)punit)->GetRunTimeUnitList()->IsEmpty()) << strFileName << "���ɷ�װ��������ʱ��Ԫ����Ӧ��Ϊ��";
        }
      }
    }

    EXPECT_EQ(runtimeUnitList.GetCount(), listTotalUnit.GetCount()) << "����ʱ��Ԫ���������е�Ԫ����������";

    poUnit = listTotalUnit.GetHeadPosition();
    iTotal = listTotalUnit.GetCount();
    for (int i = 0; i < iTotal; i++) {
      pcunitTemp = listTotalUnit.GetNext(poUnit);
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
        if (punit->IsEncapsulable()) { // �ɷ�װ������
          EXPECT_TRUE(punit->IsEncapsulated()) << "�ɷ�װ����Ӧ���Ѿ�������װ��";
        }
      }
    }

    EXPECT_EQ(runtimeUnitList.GetCount(), listTotalUnit.GetCount()) << "����ʱ��Ԫ���������е�Ԫ����������";
    // ������еĲ����ڲ���Ԫ���������ȼ�
    poUnit = listTotalUnit.GetHeadPosition();
    iTotal = listTotalUnit.GetCount();
    for (i = 0; i < iTotal; i++) {
      pcunitTemp = listTotalUnit.GetNext(poUnit);
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
    CUnitBase * pctemp;
    INT64 iTotal;
    INT64 iCurrentUnit = sizeof(ULONG);
    CUnitComponent * pCpt = nullptr;

    ReSetCompileFlag(&m_unitlist);

    SetParaLockFlag(&m_unitlist, &m_objectlist);

    CUnitList rtUnitList;

    EXPECT_FALSE(UnitListLoopDetect(&m_unitlist));  // ���û�з��ֶ�̬����ѭ��
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

    // �ڱ��뵥Ԫ����ǰ���������ñ����־�������������
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
    EXPECT_GT(l, 1) << "����CheckInnerDataLink����ʱ���������ļ��еĲ�����������������Ͳ���������һ���ڲ���Ԫ";

    // clear up
    ReleaseSQIFile(&m_unitlist, &m_objectlist);
    EXPECT_TRUE(m_unitlist.IsEmpty());
    EXPECT_TRUE(m_objectlist.IsEmpty());

  }

}
