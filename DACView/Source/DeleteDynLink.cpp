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
  CUnitBase * pcUnit;
  CUnitList listUnit;
  CUnitComponent * pCpt = nullptr;
  CUnitList *plistCpt;

  if (pUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { // ���������ɾ������
    pCpt = (CUnitComponent *)pUnit;
    if (!pCpt->IsEncapsulable() || (pCpt->IsEncapsulable() && !pCpt->IsEncapsulated())) { // ����δ��װ�Ĳ������߲������װ�Ĳ���
      // �������ָ�򱾲����ڲ���Ԫ���еĶ�̬����
      plistCpt = pCpt->GetUnitList();
      for (const auto pcunit : *plistCpt) {
        DeleteDynLinkToMe(pUnitList, pObjectList, pcunit);
      }
    }
  }

  // ������ʱ��Ԫ����
  for (const auto pcUnit : *pUnitList) {
    pcUnit->AddToList(listUnit);
  }
  // �����Ԫ������ָ��ɾ����Ԫ�Ķ�̬����
  for (const auto pcUnit : listUnit) {
    pcUnit->DeleteDynLink(pUnit);
  }

  POSITION po = pObjectList->GetHeadPosition();
  INT64 iTemp = pObjectList->GetCount();
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

  if (pUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { // ���������ɾ������
    pCpt = (CUnitComponent *)pUnit;
    if (!pCpt->IsEncapsulable() || (pCpt->IsEncapsulable() && !pCpt->IsEncapsulated())) { // ����δ��װ�Ĳ������߲������װ�Ĳ���
      // ����������ڲ���Ԫ����ָ������ж�̬����
      plistCpt = pCpt->GetUnitList();
      for (const auto pcunit : *plistCpt) {
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
