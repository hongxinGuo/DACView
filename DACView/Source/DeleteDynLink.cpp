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
// 描述:
//   检查所有的单元和对象,清除所有指向单元（或部件的内部单元序列，如果未封装的话）的动态连接. 
// called by CSQIUnitView's DeleteUnit().
//
////////////////////////////////////////////////////////////////////////////////
void DeleteDynLinkToMe(CUnitList * pUnitList, CObjectList * pObjectList, CUnitBasePtr pUnit) {
  CUnitList listUnit;
  CUnitComponentPtr pCpt = nullptr;
  CUnitList *plistCpt;

  if (pUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { // 处理部件类的删除工作
    pCpt = static_pointer_cast<CUnitComponent>(pUnit);
    if (!pCpt->IsEncapsulable() || (pCpt->IsEncapsulable() && !pCpt->IsEncapsulated())) { // 处理未封装的部件或者不允许封装的部件
      // 清除所有指向本部件内部单元序列的动态链接
      plistCpt = pCpt->GetUnitList();
      for (const auto pcunit : *plistCpt) {
        DeleteDynLinkToMe(pUnitList, pObjectList, pcunit);
      }
    }
  }

  // 生成临时单元序列
  for (const auto pcUnit : *pUnitList) {
    pcUnit->AddToList(listUnit);
  }
  // 清除单元序列中指向被删除单元的动态链接
  for (const auto pcUnit : listUnit) {
    pcUnit->DeleteDynLink(pUnit);
  }

  // 清除物体序列中指向被删除单元的动态链接
  for (const auto pcObj : *pObjectList) {
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
void DeleteDynLinkFromMe(CUnitBasePtr pUnit) {
  CUDLList * plistDL;
  CUnitList listUnit;
  CUnitComponentPtr pCpt = nullptr;
  CUnitList *plistCpt;

  if (pUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { // 处理部件类的删除工作
    pCpt = dynamic_pointer_cast<CUnitComponent>(pUnit);
    if (!pCpt->IsEncapsulable() || (pCpt->IsEncapsulable() && !pCpt->IsEncapsulated())) { // 处理未封装的部件或者不允许封装的部件
      // 清除本部件内部单元序列指向的所有动态链接
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
