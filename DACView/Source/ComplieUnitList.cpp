#include "stdafx.h"

#include"CompileUnitList.h"

#include "cUnitComponent.h"

#include "CUnitDynLink.h"

#include "..\\resource.h"
#include "globeDef.h"

#include"SQIFileDoc.h"

////////////////////////////////////////////////////////////////////////////////////////
//
// ClearLoopDetectFlag( void )
//
//
/////////////////////////////////////////////////////////////////////////////////////////
void ClearLoopDetectFlag(CUnitList * pUnitList) {
  for (const auto pcunit : *pUnitList) {
    pcunit->ClearLoopDetectFlag();
  }
}

////////////////////////////////////////////////////////////////////////////////////////
//
// 单元序列动态链接的循环链接测试。
//
// Return :
//		bool : TRUE if find a loop DynLink in m_CUnitList
//
//		参数：
//			CUnitList * pUnitList; // 被查找的单元序列。
//
//
// Description :
//	检查单元序列中是否存在动态链接的循环,并且检查是否一个循环中有一个以上的截断点。
//	
//  部件是将自己的内部单元序列添加进待查单元序列中；
//  部件在封装后，其表现与简单单元一样，故而如果部件参数之间形成链路，则需要在部件本身做出标记，此时如果外界链接此两个参数的动态链接也形成环路，
//  那么就在上层形成了环路，需要做截断处理。
//  目前尚未编写处理封装后部件的具体过程。2019.02.18 
//
//   为了简化系统维护，循环测试要越早越好，故而应该将此函数用于生成新单元之时，即创建一个新单元后，或者从剪辑版paste过来一组新单元，立即执行循环测试
//
////////////////////////////////////////////////////////////////////////////////////////
bool UnitListLoopDetect(CUnitList * pCUnitList) {
  CUnitBase * pcunit1, *pCUnit;
  CUnitList unitlist;
  BOOL fFind = false;

  // 寻找是否有循环.
  ClearLoopDetectFlag(pCUnitList);
  for (const auto pcunit : *pCUnitList) {
    fFind = pcunit->LoopDetect(&unitlist);
    if (fFind) {
      pCUnit = unitlist.back();
      auto it = unitlist.begin();
      // delete units that not in loop list from unitlist
      while (1) {
        pcunit1 = *it++;
        if (pcunit1 == pCUnit) break;
        unitlist.pop_front();
      }
      // tell user that has find a loop 
      CString str;

      // make loop string from loop units
      for (const auto pcunit2 : unitlist) {
        str += pcunit2->GetName();
        str += "->";
      }
      ShowMessageIndirect(ID_ERROR_STRATEGY_LOOP_DETECT, (LPCTSTR)str);

      // set loop detect flag
      CUnitBase * pcunitPrev = unitlist.front();
      auto it1 = unitlist.begin();
      for (++it1; it1 != unitlist.end(); it1++) {
        pcunit1 = *it1;
        pcunitPrev->SetLoopDetectFlag(pcunit1);
        pcunitPrev = pcunit1;
      }

      unitlist.clear();
      return(true);
    }
    unitlist.clear();
  }

  return(false); // not find Loop
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// AlreadyHaveCutOff()
//
// Parameter :
//		CUnitBase * pCUnit : 需检查的单元
//		CUnitList * pUnitList 被检查的单元序列
//
// Return : 
//		bool : 当动态链接循环中已经存在存在了一个截断，返回真；否则返回假
//
// Description :
//		When you want to set cutoff on a unit, you can't set more than one cutoff on one loop
//	So this function is to check whether this loop have a cutoff already.
//
///////////////////////////////////////////////////////////////////////////////////////// 
bool AlreadyHaveCutOff(CUnitBase * pCUnit, CUnitList * pUnitList) {
  CUnitBase * pcCutOff;
  CUnitList unitlist;
  CString strName, str;
  BOOL fFind = false;

  for (const auto pcunitTemp : *pUnitList) {
    fFind = pcunitTemp->CheckCutOff(&unitlist);
    if (fFind && (find(unitlist.begin(), unitlist.end(), pCUnit) != unitlist.end())) {
      auto it = unitlist.begin();
      while (1) {
        auto pcunit = *it++;
        if (pcunit == pCUnit) break;
        unitlist.pop_front();
      }
      for (const auto pcunit : unitlist) {
        if (pcunit->IsSetCutOff()) {
          pcCutOff = pcunit;
          for (const auto pcunit2 : unitlist) {
            str += pcunit2->GetName();
            str += "->";
          }
          str += pcCutOff->GetName();
          ShowMessageIndirect(ID_ERROR_STRATEGY_TOO_MANY_START_POINT, (LPCTSTR)str);
          unitlist.clear();
          return(true);
        }
      }
    }
    unitlist.clear();
  }
  unitlist.clear();

  return(false); // not find Loop
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 将所有的单元组成单元序列，编译的部件则将其内部单元序列和部件本身加入。
//
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CreateUniUnitList(CUnitList * pUnitList, CUnitList &listUniUnit) {
  // 在此之前，单元已经清除编译标志和执行优先级，设置了输入参数个数
  // 将所有的单元(包括未编译部件本身及其内部单元序列）组成一个单独的单元序列. 
  for (const auto pcunit : *pUnitList) {
    if (!pcunit->IsEncapsulated()) ASSERT((pcunit->GetExectivePriority() == 0) || (pcunit->GetExectivePriority() == 1));
    pcunit->CheckSelf();
    pcunit->AddToList(listUniUnit);
  }
  return(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 设置没有源单元的单元(输入参数连接的单元)的处理优先值为最高(1，最先处理).
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
bool SetNoSrcUnitExectivePriority(CUnitList * plistUnit) {
  // 设置没有源单元的单元(输入参数连接的单元)的处理优先值为1(最先处理).
  for (const auto punit : *plistUnit) {
    if (!punit->IsHaveSourceUnit()) { // no one link to me ?
      punit->SetExectivePriority(1);
    }
  }
  return(true);
}

bool CheckUnitListCompiledStatus(CUnitList * pUnitList) {
  // 简单检测本层的单元序列（不包括部件）都设置了执行优先级
  for (const auto pcunit : *pUnitList) {
    if (!pcunit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
      ASSERT(pcunit->GetExectivePriority() > 0);
    }
    else if (!pcunit->IsEncapsulable()) {
      ASSERT(pcunit->GetExectivePriority() > 0);
    }
  }
  return(true);
}

bool CheckRunTimeUnitListCompiledStatus(CUnitList * pRunTimeUnitList) {
  // 简单检测运行时单元序列（除部件外）都设置了执行优先级
  for (const auto pcunit : *pRunTimeUnitList) {
    if (!pcunit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
      ASSERT(pcunit->GetExectivePriority() > 0);
      pcunit->SetComment("aaabbb");
    }
    else if (pcunit->IsEncapsulable()) {
      ASSERT(pcunit->GetExectivePriority() > 0);
    }
  }
  return(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 编译组织好的单元序列，设置所有单元的执行优先级。
// 封装部件的工作留给其他函数去完成。
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool ExectiveCompilation(CUnitList &unitlist, CUnitList * pRunTimeUnitList) {
  INT64 iRunTimeTemp = 0, iTotal;

  TRACE("Start unit list's compilation\n");

  // clear runtime unitList
  pRunTimeUnitList->clear();
  // 开始编译
  INT64 iRunTimeTempOld = 0, iCurrentPriority = 0;
  bool done = false, fFindLoop = false;
  iTotal = unitlist.size();
  while ((!done) && (!fFindLoop)) {
    iCurrentPriority++;
    for (const auto pcunit : unitlist) {
      if (pcunit->GetExectivePriority() == iCurrentPriority) { // 找到了执行优先级相等的单元
        pcunit->SetDestUnitPriority();    // 设置此单元的目的单元执行优先级
        pcunit->SetCompiledFlag(true);
        pRunTimeUnitList->push_back(pcunit); // create runtime list
        TRACE("%s(%u)\n", (LPCTSTR)(pcunit->GetName()), pcunit->GetExectivePriority());
      }
    }
    iRunTimeTemp = pRunTimeUnitList->size();

    if (iRunTimeTemp == iTotal) {		// 全找到了
      done = true;
    }
    else if (iRunTimeTemp == iRunTimeTempOld) {// 如无法找到更多的单元(全找到了或还剩下循环)
      fFindLoop = TRUE; // 发现了循环存在
    }
    else iRunTimeTempOld = iRunTimeTemp;

    if (iRunTimeTemp > iTotal) {    // something was wrong 
      pRunTimeUnitList->clear();
      return(false);
    }
  }

  // 如有循环剩下,则将循环开始单元(cut_off unit)的优先值设为iCurrentPriority.
  if (fFindLoop) {
    for (const auto pcunit : unitlist) {
      if (pcunit->IsSetCutOff()) {
        pcunit->SetExectivePriorityDirect(iCurrentPriority); // 必须使用此直接设置函数，SetExectivePriority会根据不同的情况分别处理
        pcunit->SetCompiledFlag(true);
      }
    }

    // 处理与循环有关的单元.
    iRunTimeTempOld = 0;
    while (!done) {
      for (const auto pcunit : unitlist) {
        if (pcunit->GetExectivePriority() == iCurrentPriority) {
          pcunit->SetDestUnitPriority();
          pcunit->SetCompiledFlag(true);
          pRunTimeUnitList->push_back(pcunit);     // create runtime list
          TRACE("%s(%u)\n", (LPCTSTR)(pcunit->GetName()),
            pcunit->GetExectivePriority());
        }
      }
      iRunTimeTemp = pRunTimeUnitList->size();
      if (iRunTimeTemp == iRunTimeTempOld) done = TRUE;
      else {
        iRunTimeTempOld = iRunTimeTemp;
      }
      if (iRunTimeTemp == iTotal) done = TRUE;
      if (iRunTimeTemp > iTotal) {    // something was wrong 
        pRunTimeUnitList->clear();
        return(false);
      }
      iCurrentPriority++;
    }
    ASSERT(iRunTimeTemp == iTotal);
  } // fFindLoop

  return(true);
}

void ReSetCompileFlag(CUnitList * pUnitList)
{
  // 重置编译标志。这步现在要做，以防止出现可能的误存储。
  for (const auto pcUnit : *pUnitList) {
    pcUnit->ResetCompileFlag();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// 清除单元序列中残存的旧标志，然后设置正确的内部状态以备编译。
//
///////////////////////////////////////////////////////////////////////////////
void SetParaLockFlag(CUnitList * pUnitList, CObjectList * pObjectList) {
  // set unit select parameter
  for (const auto pcUnit : *pUnitList) {
    pcUnit->SetParaLockFlag();
  }

  for (const auto pcObj : *pObjectList) {
    pcObj->SetParameterSelected();
  }
}

bool EncapsulateUnitList(CUnitList * pUnitList) {
  CUnitList unitlist;

  CreateUniUnitList(pUnitList, unitlist); // 生成需要编译的单一单元序列

  // 封装需封装的部件
  for (auto punit : unitlist) {
    if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
      if (punit->IsEncapsulable() && (!punit->IsEncapsulated())) {
        punit->Encapsulation(unitlist);
      }
    }
  }

  return true;
}

bool SetEncapsulatingFlag(CUnitList * pUnitList) {
  CUnitList unitlist;

  CreateUniUnitList(pUnitList, unitlist); // 生成需要编译的单一单元序列
  // 设置单元序列编译中的标志
  for (auto punit : unitlist) {
    ASSERT(!punit->IsEncapsulating());
    if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent)) && !punit->IsEncapsulated()) { // 如果部件尚未被封装
      if (punit->IsEncapsulable()) { // 如果部件可以被封装
        punit->SetEncapsulatingFlag(true);
      }
    }
  }

  return true;
}

bool CompileInnerComponent(CUnitList * pUnitList) {
  for (const auto punit : *pUnitList) {
    if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
      if (punit->IsEncapsulating()) { //如果部件正处于封装状态或者部件不可被封装
        ASSERT(punit->IsCompiled());
        punit->Compilation();
        ASSERT(punit->IsCompiled());
        ASSERT(!punit->IsEncapsulating());
      }
    }
    else punit->Compilation();
  }

  return true;
}


////////////////////////////////////////////////////////////////////////
//
// CompileUnitList()
//
// Parameter : 
//				CUnitList * pUnitList;  传入的单元序列
//				CUnitList * pRunTimeUnitList; // 传出的运行时单元序列
//
// Return :
//    BOOL      : TRUE if secuess created.
//              : FALSE if not created runtime unit list
//
// Description :
//   这个函数是生成运行时态单元序列.在此之前，设置了输入参数个数，清除了编译标志和执行优先级,部件已经被封装
// 
// 采用先编译后封装的办法，发现有时会导致优先级出现错误，具体情况就是当编译可被封装的部件时，
// 部件的优先级是在编译单元序列的最后才确认的，结果被测试函数发现错误。
// 办法：
// 将编译过程修改为先封装后编译。封装动作完成数据隔离的功能，各层之间只通过部件的参数发生数据联系；这样每个部件的编译过程都是一样的，
// 因为结构相同。不可封装的部件在编译时与上层单元序列一起编译。
// 
//    
////////////////////////////////////////////////////////////////////////
bool CompileUnitList(CUnitList * pUnitList, CUnitList * pRunTimeUnitList) {
  CUnitList unitlist;

  TRACE("Start Compile unit list\n");


  // 在此之前，单元序列已经清除编译标志和执行优先级，设置了输入参数个数,单元序列中的可封装部件都已经被封装了。
  // 将所有的单元(包括部件本身）组成一个单独的单元序列. 此时部件已完成封装，故而其内部单元序列不再加此此单元序列中。不可封装部件的内部单元序列要加入。
  // 这是为了测试是否有循环存在
  CreateUniUnitList(pUnitList, unitlist);

  // 检测是否有循环形成
  if (UnitListLoopDetect(&unitlist)) return(false);

  // 设置没有源单元的单元(输入参数连接的单元)的处理优先值为1(最先处理).
  SetNoSrcUnitExectivePriority(&unitlist);

  // 开始编译
  ExectiveCompilation(unitlist, pRunTimeUnitList);

  // 然后编译本单元序列中的部件
  CompileInnerComponent(&unitlist); // 此时使用汇总后的单元序列

  // 以下两个只是检查而已。
  // 简单检测本层的单元序列（不包括部件）都设置了执行优先级
  CheckUnitListCompiledStatus(pUnitList);

  // 简单检测运行时单元序列（除部件外）都设置了执行优先级
  CheckRunTimeUnitListCompiledStatus(pRunTimeUnitList);

  unitlist.clear();
  return (true);
}

bool Compilation(CUnitList * pUnitList, CUnitList * pRunTimeUnitList) {
  // 设置封装中标志
  SetEncapsulatingFlag(pUnitList);

  // 封装单元序列中的可封装部件
  EncapsulateUnitList(pUnitList);

  // 编译此单元序列
  CompileUnitList(pUnitList, pRunTimeUnitList);

  return true;
}


