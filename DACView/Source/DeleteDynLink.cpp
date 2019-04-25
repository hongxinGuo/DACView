#include"stdafx.h"

#include"CObjectBase.h"
#include"CUnitBase.h"
#include"CUnitComponent.h"

#include"DeleteDynLink.h"

///////////////////////////////////////////////////////////////////////////////
//
// DeleteDynLinkToMe()
//
// ����:
//   ������еĵ�Ԫ�Ͷ���,�������ָ��Ԫ���򲿼����ڲ���Ԫ���У����δ��װ�Ļ����Ķ�̬����. 
// called by CSQIUnitView's DeleteUnit().
//
////////////////////////////////////////////////////////////////////////////////
void DeleteDynLinkToMe(CUnitList * pUnitList, CObjectList * pObjectList, CUnitBase * pUnit) {
  POSITION po = pUnitList->GetHeadPosition(), poCpt;
  INT_PTR iTemp = pUnitList->GetCount();
  CUnitBase * pcUnit;
  CUnitList listUnit;
  CUnitComponent * pCpt = nullptr;
  CUnitList *plistCpt;
  INT64 iTotalCpt = 0;
  CUnitBase * pcunit = nullptr;

  if (pUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { // ���������ɾ������
    pCpt = (CUnitComponent *)pUnit;
    if (!pCpt->IsEncapsulable() || (pCpt->IsEncapsulable() && !pCpt->IsEncapsulated())) { // ����δ��װ�Ĳ������߲������װ�Ĳ���
      // �������ָ�򱾲����ڲ���Ԫ���еĶ�̬����
      plistCpt = pCpt->GetUnitList();
      poCpt = plistCpt->GetHeadPosition();
      iTotalCpt = plistCpt->GetCount();
      for (int j = 0; j < iTotalCpt; j++) {
        pcunit = plistCpt->GetNext(poCpt);
        DeleteDynLinkToMe(pUnitList, pObjectList, pcunit);
      }
    }
  }
  po = pUnitList->GetHeadPosition();
  INT iTotal = pUnitList->GetCount();
  for (int i = 0; i < iTotal; i++) {
    pcUnit = pUnitList->GetNext(po);
    pcUnit->AddToList(listUnit);
  }
  // �����Ԫ������ָ��ɾ����Ԫ�Ķ�̬����
  po = listUnit.GetHeadPosition();
  iTotal = listUnit.GetCount();
  for (int i = 0; i < iTemp; i++) {
    pcUnit = listUnit.GetNext(po);
    pcUnit->DeleteDynLink(pUnit);
  }

  po = pObjectList->GetHeadPosition();
  iTemp = pObjectList->GetCount();
  CObjectBase * pcObj;
  // �������������ָ��ɾ����Ԫ�Ķ�̬����
  for (int i = 0; i < iTemp; i++) {
    pcObj = pObjectList->GetNext(po);
    pcObj->DeleteDynLink(pUnit);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// DeleteDynLinkFromMe()
//
// ����:
//   ��鵥Ԫ,���pUnit���򲿼����ڲ���Ԫ���У����δ��װ�Ļ���ָ��Ķ�̬����. 
// called by CSQIUnitView's DeleteUnit().
//
////////////////////////////////////////////////////////////////////////////////
void DeleteDynLinkFromMe(CUnitBase * pUnit) {
  CUDLList * plistDL;
  CUnitList listUnit;
  CUnitComponent * pCpt = nullptr;
  CUnitList *plistCpt;
  INT64 iTotalCpt = 0;
  CUnitBase * pcunit = nullptr;
  POSITION poCpt;

  if (pUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { // ���������ɾ������
    pCpt = (CUnitComponent *)pUnit;
    if (!pCpt->IsEncapsulable() || (pCpt->IsEncapsulable() && !pCpt->IsEncapsulated())) { // ����δ��װ�Ĳ������߲������װ�Ĳ���
      // ����������ڲ���Ԫ����ָ������ж�̬����
      plistCpt = pCpt->GetUnitList();
      poCpt = plistCpt->GetHeadPosition();
      iTotalCpt = plistCpt->GetCount();
      for (int j = 0; j < iTotalCpt; j++) {
        pcunit = plistCpt->GetNext(poCpt);
        DeleteDynLinkFromMe(pcunit);
      }
    }
  }

  plistDL = pUnit->GetDynLinkList();
  for (auto pcDynLink : *plistDL) {
    // clear destination unit's selected flag
    pcDynLink->GetDestUnit()->SetParameterLock(pcDynLink->GetDestIndex(), FALSE);
  }
}
