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
      "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ����.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ������.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ�.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ����������.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ㲿��֮�����Ӳ���������.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\��װ��Ĳ���.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ�����������з�װ��Ĳ���.sqi"
    , "c:\\DACViewTestFile\\CompileUnitList\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ�����������з�װ��Ķ������.sqi"
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

    // ����������йصĲ��ԣ�����Ҫ���������ļ�������ϵͳ���ݹ�����ԭ���޷��������������ʶ���Ҫ���������ļ�������ٲ��Է�װ��
    CompileUnitList(&unitlist, &unitListRunTime);

    POSITION poUnit = unitlist.GetHeadPosition();
    for (int i = 0; i < iTotal; i++) {
      pcunitTemp = unitlist.GetNext(poUnit);
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
  // �������װ�йصĲ��Զ�����������Ͱ��һ����������֤��
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

    // ���ñ�������֮ǰ������
    ReSetCompileFlag(&unitlist);

    SetParaLockFlag(&unitlist);

    CreateUniUnitList(&unitlist, listTotalUnit);

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
              }
            }
          }
        }
      }
    }

    SetNoSrcUnitExectivePriority(&listTotalUnit);

    ExectiveCompilation(listTotalUnit, &runtimeUnitList);

    // ��Encapsulationչ�����ֶ�ִ�кͲ���
    poUnit = unitlist.GetHeadPosition();
    iTotal = unitlist.GetCount();
    CUnitList * pUnitList;
    POSITION po;
    CUnitBase * punit;
    for (int i = 0; i < iTotal; i++) {
      pcunit = unitlist.GetNext(poUnit);
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
                    else if (unitlist.Find(pSrcUnit->GetComponentUpper())) { // ������Ԫ�Ĳ���λ�ڱ���Ԫ������
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
                      EXPECT_EQ(pDL2->GetDynLinkClass(), UNIT_TO_COMPONENT) << "�������COMPONENT_TO_COMPONENT��ΪUNIT_TO_COMPONENT";
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

          // �����ڲ���Ԫ���е�����ʱ��Ԫ����
          pCUCP->CreateRunTimeUnitList();

          // �����Ƿ����ɵ�����ʱ��Ԫ���и�����������Ԫ���еĸ�����ͬ���Ұ���������
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
  // ���������Ĳ��Խ��Ӧ����ȫһ��������������Ϊ�Աȡ�
  // TestEncapsulation1��EncapsulateUnitList���Ϊ���ڲ�ʹ�ü�������˳��ִ�С�
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
        if (punit->IsEncapsulable()) { // �ɷ�װ������
          EXPECT_TRUE(punit->IsEncapsulated()) << "�ɷ�װ����Ӧ���Ѿ�������װ��";
        }
      }
    }

    EXPECT_EQ(runtimeUnitList.GetCount(), listTotalUnit.GetCount()) << "����ʱ��Ԫ���������е�Ԫ����������";
    // ������еĲ����ڲ���Ԫ���������ȼ�
    poUnit = listTotalUnit.GetHeadPosition();
    for (i = 0; i < iTotal; i++) {
      pcunitTemp = listTotalUnit.GetNext(poUnit);
      if (pcunitTemp->GetExectivePriority() == 1) {
        EXPECT_TRUE(!pcunitTemp->IsHaveSourceUnit()) << "ִ�����ȼ�Ϊ1�ĵ�Ԫû����������";
      }
      if (pcunitTemp->GetExectivePriority() > 1) {
        EXPECT_TRUE(pcunitTemp->IsHaveSourceUnit());
      }
      EXPECT_GT(pcunitTemp->GetExectivePriority(), 0) << "ִ�����ȼ�����1�ĵ�Ԫ��Դ��Ԫ���������룩";

      if (pcunitTemp->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
        EXPECT_TRUE(pcunitTemp->IsEncapsulated()) << "��װ�������";
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

    // �ڱ��뵥Ԫ����ǰ���������ñ����־�������������
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

    POSITION po = unitlist.GetHeadPosition();
    for (int i = 0; i < iTotal; i++) {
      cp2 = unitlist.GetNext(po);
      delete cp2;
    }
    // delete pCpt;
    unitlist.RemoveAll();
  }

}