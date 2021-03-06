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
  for (const auto punit : *pCUnitList) {
    fFind = punit->LoopDetect(&unitlist);
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
      for (const auto punit2 : unitlist) {
        str += punit2->GetName();
        str += "->";
      }
      ShowMessageIndirect(ID_ERROR_STRATEGY_LOOP_DETECT, (LPCTSTR)str);

      // 将此循环涉及的动态链接都设置标志（用于将动态链接线显示为红色）
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
        if (pcunit->IsCutoff()) {
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
// 将所有的单元组成单元序列。未编译的部件则将其内部单元序列和部件本身加入，编译过的部件只将其自身加入单元序列（内部不可见了）。
//
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CreateUniUnitList(CUnitList * pUnitList, CUnitList &listUniUnit) {
  // 在此之前，单元已经清除编译标志和执行优先级，设置了输入参数个数
  // 将所有的单元(包括未编译部件本身及其内部单元序列）组成一个单独的单元序列. 
  for (const auto pcunit : *pUnitList) {
    if (!pcunit->IsEncapsulated()) {
      // 编译前其值为0， 编译后其值为1
      ASSERT((pcunit->GetExectivePriority() == 0) || (pcunit->GetExectivePriority() == 1));
    }
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
bool ExectiveCompilation(CUnitList * pUnitList, CUnitList * pRunTimeUnitList) {
  INT64 iRunTimeTemp = 0, iTotal;

  TRACE("Start unit list's compilation\n");

  // clear runtime unitList
  pRunTimeUnitList->clear();
  // 开始编译
  INT64 iRunTimeTempOld = 0, iCurrentPriority = 0;
  bool done = false, fFindLoop = false;
  iTotal = pUnitList->size();
  while ((!done) && (!fFindLoop)) {
    iCurrentPriority++;
    for (const auto punit : *pUnitList) {
      if (punit->GetExectivePriority() == iCurrentPriority) { // 找到了执行优先级相等的单元
        TRACE("%s(%u)\n", (LPCTSTR)(punit->GetName()), punit->GetExectivePriority());
        punit->SetDestUnitPriority();    // 设置此单元的目的单元执行优先级
        ASSERT(!punit->IsCompiled());    // 设置编译标志之前，确保此单元尚未被编译
        punit->SetCompiledFlag(true);
        pRunTimeUnitList->push_back(punit); // create runtime list
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
      TRACE("编译过程出现错误\n");
      return(false);
    }
  }

  // 如有循环剩下,则将循环开始单元(cut_off unit)的优先值设为iCurrentPriority.
  if (fFindLoop) {
    for (const auto punit : *pUnitList) {
      if (punit->IsCutoff() && !punit->IsCompiled()) { // 只设置没有编译过的截断单元（没有动态链接的单元已经被编译过了，哪怕也设置了截断）
        punit->SetExectivePriorityDirect(iCurrentPriority); // 必须使用此直接设置函数，SetExectivePriority会根据不同的情况分别处理
        punit->SetCompiledFlag(true);
      }
    }

    // 处理与循环有关的单元.
    iRunTimeTempOld = 0;
    while (!done) {
      for (const auto punit : *pUnitList) {
        if (punit->GetExectivePriority() == iCurrentPriority) {
          punit->SetDestUnitPriority();
          punit->SetCompiledFlag(true);
          pRunTimeUnitList->push_back(punit);     // create runtime list
          TRACE("%s(%u)\n", (LPCTSTR)(punit->GetName()),
            punit->GetExectivePriority());
        }
      }
      iRunTimeTemp = pRunTimeUnitList->size();
      if (iRunTimeTemp == iRunTimeTempOld) done = TRUE;
      else {
        iRunTimeTempOld = iRunTimeTemp;
      }
      if (iRunTimeTemp == iTotal) done = TRUE;
      if (iRunTimeTemp > iTotal) {    // something was wrong 
        TRACE("生成的运行时单元序列有误，其数量多于了被编译单元序列的数量\n");
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
  for (const auto punit : *pUnitList) {
    punit->ResetCompileFlag();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// 清除单元序列中残存的旧标志，然后设置正确的内部状态以备编译。
//
///////////////////////////////////////////////////////////////////////////////
void SetParaLockFlag(CUnitList * pUnitList, CObjectList * pObjectList) {
  // set unit select parameter
  for (const auto punit : *pUnitList) {
    punit->SetParaLockFlag();
  }

  for (const auto pobj : *pObjectList) {
    pobj->SetParameterSelected();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// 清除单元序列中残存的旧标志，然后设置正确的内部状态以备编译。
//
// 此函数只设置单元序列中的动态链接，Object序列中的输入型参数置之不理。
//
//
///////////////////////////////////////////////////////////////////////////////
void SetParaLockFlag(CUnitList * pUnitList) {
  // set unit select parameter
  for (const auto pcUnit : *pUnitList) {
    pcUnit->SetParaLockFlag();
  }
}


bool EncapsulateUnitList(CUnitList & unitlist) {

  // 封装需封装的部件
  // 此处的pUnitList要保证是生成的UniUnitlist,否则就可能有部件没有封装
  for (auto punit : unitlist) {
    if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
      ASSERT(!punit->IsCompiled());
      if (punit->IsEncapsulable() && (!punit->IsEncapsulated())) {
        ASSERT(punit->IsEncapsulating());
        punit->Encapsulation(unitlist);
      }
      ASSERT(!punit->IsCompiled());
    }
  }

  return true;
}

bool SetEncapsulatingFlag(CUnitList & unitlist) {

  // 设置单元序列编译中的标志
  for (auto punit : unitlist) {
    ASSERT(!punit->IsEncapsulating());
    if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
      if (punit->IsEncapsulable() && !punit->IsEncapsulated()) { // 如果部件可封装且尚未被封装
        punit->SetEncapsulatingFlag(true);  // 则设置此部件处于封装中的标志
        punit->mtest_fEncapsulating = true;
      }
    }
  }
  return true;
}

bool CompileInnerComponent(CUnitList * pUnitList) {
  for (const auto punit : *pUnitList) {
    if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
      if (punit->IsEncapsulating()) { //如果部件正处于封装状态或者部件不可被封装
        ASSERT(punit->IsCompiled()); //编译部件内部的单元序列时，部件本身已经编译过了。因为编译是从顶层开始的，最后编译的是最内部的部件
        punit->Compilation();
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

  TRACE("Start Compile unit list\n");


  // 在此之前，单元序列已经清除编译标志和执行优先级，设置了输入参数个数,单元序列中的可封装部件都已经被封装了。
  // 此单元序列已经通过CreateUnitList函数加入了不可编译部件中的单元序列
  // 此时部件已完成封装，故而其内部单元序列不再加此此单元序列中。不可封装部件的内部单元序列要加入。

  // 检测是否有循环形成
  if (UnitListLoopDetect(pUnitList)) return(false);

  // 设置没有源单元的单元(输入参数连接的单元)的处理优先值为1(最先处理).
  SetNoSrcUnitExectivePriority(pUnitList);

  // 开始编译
  ExectiveCompilation(pUnitList, pRunTimeUnitList);
  ASSERT(pUnitList->size() == pRunTimeUnitList->size());
#ifdef _DEBUG
  for (auto punit : *pUnitList) {
    ASSERT(find(pRunTimeUnitList->begin(), pRunTimeUnitList->end(), punit) != pRunTimeUnitList->end());
  }
#endif

  // 然后编译本单元序列中的部件
  CompileInnerComponent(pUnitList); // 此时使用汇总后的单元序列

  // 以下两个只是检查而已。
  // 简单检测本层的单元序列（不包括部件）都设置了执行优先级
  CheckUnitListCompiledStatus(pUnitList);

  // 简单检测运行时单元序列（除部件外）都设置了执行优先级
  CheckRunTimeUnitListCompiledStatus(pRunTimeUnitList);

  return (true);
}

bool Compilation(CUnitList * pUnitList, CObjectList & objectlist, CUnitList * pRunTimeUnitList) {
  CUnitList unitlist;

  // 重置编译标志
  ReSetCompileFlag(pUnitList);
  // 设置各单元的输入动态链接数
  SetParaLockFlag(pUnitList, &objectlist);


  // 生成单一单元序列
  CreateUniUnitList(pUnitList, unitlist);

  // 设置封装中标志
  SetEncapsulatingFlag(unitlist);

  // 封装单元序列中的可封装部件
  EncapsulateUnitList(unitlist);

  unitlist.clear();

  //此时生成的单一单元序列，由于部件已经封装过了，故而可封装部件内的单元序列不再加入，只有最上层的单元和不可封装部件内的单元序列加入此序列
  CreateUniUnitList(pUnitList, unitlist);

  // 编译封装后新生成的单元序列
  CompileUnitList(&unitlist, pRunTimeUnitList);

  unitlist.clear();
  return true;
}


