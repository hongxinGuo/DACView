#include"stdafx.h"

#include"typedef.h"
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
void DeleteDynLinkToMe(CUnitList * pUnitList, CObjectList * pObjectList, CUnitBasePtr pUnit) {
  CUnitList listUnit;
  CUnitComponentPtr pCpt = nullptr;
  CUnitList *plistCpt;

  if (pUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { // ���������ɾ������
    pCpt = static_pointer_cast<CUnitComponent>(pUnit);
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

  // �������������ָ��ɾ����Ԫ�Ķ�̬����
  for (const auto pcObj : *pObjectList) {
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
void DeleteDynLinkFromMe(CUnitBasePtr pUnit) {
  CUDLList * plistDL;
  CUnitList listUnit;
  CUnitComponentPtr pCpt = nullptr;
  CUnitList *plistCpt;

  if (pUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { // ���������ɾ������
    pCpt = dynamic_pointer_cast<CUnitComponent>(pUnit);
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
