#include"stdafx.h"

#include"CObjectBase.h"
#include"CUnitBase.h"
#include"CUnitComponent.h"

#include"DeleteDynLink.h"

///////////////////////////////////////////////////////////////////////////////
//
// DeleteDynLinkToMe()
//
// 描述:
//   检查所有的单元和对象,清除所有指向单元（或部件的内部单元序列，如果未封装的话）的动态连接. 
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

  if (pUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { // 处理部件类的删除工作
    pCpt = (CUnitComponent *)pUnit;
    if (!pCpt->IsEncapsulable() || (pCpt->IsEncapsulable() && !pCpt->IsEncapsulated())) { // 处理未封装的部件或者不允许封装的部件
      // 清除所有指向本部件内部单元序列的动态链接
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
  // 清除单元序列中指向被删除单元的动态链接
  po = listUnit.GetHeadPosition();
  iTotal = listUnit.GetCount();
  for (int i = 0; i < iTemp; i++) {
    pcUnit = listUnit.GetNext(po);
    pcUnit->DeleteDynLink(pUnit);
  }

  po = pObjectList->GetHeadPosition();
  iTemp = pObjectList->GetCount();
  CObjectBase * pcObj;
  // 清除物体序列中指向被删除单元的动态链接
  for (int i = 0; i < iTemp; i++) {
    pcObj = pObjectList->GetNext(po);
    pcObj->DeleteDynLink(pUnit);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// DeleteDynLinkFromMe()
//
// 描述:
//   检查单元,清除pUnit（或部件的内部单元序列，如果未封装的话）指向的动态连接. 
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

  if (pUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { // 处理部件类的删除工作
    pCpt = (CUnitComponent *)pUnit;
    if (!pCpt->IsEncapsulable() || (pCpt->IsEncapsulable() && !pCpt->IsEncapsulated())) { // 处理未封装的部件或者不允许封装的部件
      // 清除本部件内部单元序列指向的所有动态链接
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
