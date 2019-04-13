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
	POSITION poUnit = pUnitList->GetHeadPosition();
	CUnitBase * pcunitTemp;
	INT_PTR i, iCount = pUnitList->GetCount();

	for (i = 0; i < iCount; i++) {
		pcunitTemp = pUnitList->GetNext(poUnit);
		pcunitTemp->ClearLoopDetectFlag();
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//
// 单元序列动态链接的循环链接测试。
//
// Return :
//		BOOL : TRUE if find a loop DynLink in m_CUnitList
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
BOOL UnitListLoopDetect(CUnitList * pCUnitList) {
	POSITION po, poUnit = pCUnitList->GetHeadPosition();
	CUnitBase * pcunit, *pCUnit;
	INT_PTR i, j, jTemp, iTemp = pCUnitList->GetCount();
	CUnitList unitlist;
	BOOL fFind = FALSE;

	// 寻找是否有循环.
	ClearLoopDetectFlag(pCUnitList);
  po = pCUnitList->GetHeadPosition();
  iTemp = pCUnitList->GetCount();
	for (i = 0; i < iTemp; i++) {
		pcunit = pCUnitList->GetNext(poUnit);
		fFind = pcunit->LoopDetect(&unitlist);
		if (fFind) {
			po = unitlist.GetTailPosition();
			pCUnit = unitlist.GetPrev(po);
			po = unitlist.GetHeadPosition();
			// delete units that not in loop list from unitlist
			while (1) {
				pcunit = unitlist.GetNext(po);
				if (pcunit == pCUnit) break;
				unitlist.RemoveHead();
			}
			// tell user that has find a loop 
			CString str;
			jTemp = unitlist.GetCount();
			po = unitlist.GetHeadPosition();

			// make loop string from loop units
			for (j = 0; j < jTemp; j++) {
				pcunit = unitlist.GetNext(po);
				str += pcunit->GetName();
				str += "->";
			}
			ShowMessageIndirect(ID_ERROR_STRATEGY_LOOP_DETECT, (LPCTSTR)str);

			// set loop detect flag
			poUnit = unitlist.GetHeadPosition();
			jTemp = unitlist.GetCount();
			CUnitBase * pcunitPrev = unitlist.GetNext(poUnit);
			for (j = 1; j < jTemp; j++) {
				pcunit = unitlist.GetNext(poUnit);
				pcunitPrev->SetLoopDetectFlag(pcunit);
				pcunitPrev = pcunit;
			}

			unitlist.RemoveAll();
			return(TRUE);
		}
		unitlist.RemoveAll();
	}

	return(FALSE); // not find Loop
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
//		BOOL : 当动态链接循环中已经存在存在了一个截断，返回真；否则返回假
//
// Description :
//		When you want to set cutoff on a unit, you can't set more than one cutoff on one loop
//	So this function is to check whether this loop have a cutoff already.
//
///////////////////////////////////////////////////////////////////////////////////////// 
BOOL AlreadyHaveCutOff(CUnitBase * pCUnit, CUnitList * pUnitList) {
	POSITION po, poUnit = pUnitList->GetHeadPosition();
	CUnitBase * pcCutOff, *pcunit, *pcunitTemp;
	INT_PTR i, iCount, iTemp = pUnitList->GetCount();
	CUnitList unitlist;
	CString strName, str;
	BOOL fFind = FALSE;

	for (i = 0; i < iTemp; i++) {
		pcunitTemp = pUnitList->GetNext(poUnit);
		fFind = pcunitTemp->CheckCutOff(&unitlist);
		if (fFind && unitlist.Find(pCUnit)) {
			po = unitlist.GetHeadPosition();
			while (1) {
				pcunit = unitlist.GetNext(po);
				if (pcunit == pCUnit) break;
				unitlist.RemoveHead();
			}
			iCount = unitlist.GetCount();
			po = unitlist.GetHeadPosition();
			for (ULONG j = 0; j < iCount; j++) {
				pcunit = unitlist.GetNext(po);
				if (pcunit->IsSetCutOff()) {
					pcCutOff = pcunit;
					po = unitlist.GetHeadPosition();
					for (j = 0; j < iCount; j++) {
						pcunit = unitlist.GetNext(po);
						str += pcunit->GetName();
						str += "->";
					}
					str += pcCutOff->GetName();
					ShowMessageIndirect(ID_ERROR_STRATEGY_TOO_MANY_START_POINT, (LPCTSTR)str);
					unitlist.RemoveAll();
					return(TRUE);
				}
			}
		}
		unitlist.RemoveAll();
	}
	unitlist.RemoveAll();

	return(FALSE); // not find Loop
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 将所有的单元组成单元序列，部件则将其内部单元序列和部件本身加入。
//
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CreateUniUnitList(CUnitList * pUnitList, CUnitList &listUniUnit) {
  POSITION poUnit = pUnitList->GetHeadPosition();
  CUnitBase * pcunit;
  INT64 iTotal = pUnitList->GetCount();

  // 在此之前，单元已经清除编译标志和执行优先级，设置了输入参数个数
  // 将所有的单元(包括部件本身）组成一个单独的单元序列. 此时部件尚未进行下层部件的封装，故而下层部件内的单元也添加进此单元序列中。这是为了测试是否有循环存在
  for (int i = 0; i < iTotal; i++) {
    pcunit = pUnitList->GetNext(poUnit);
    if ( !pcunit->IsEncapsulated() ) ASSERT(pcunit->GetExectivePriority() == 0);
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
	POSITION poUnit = plistUnit->GetHeadPosition();
	INT64 iTotal = plistUnit->GetCount();
  CUnitBase * punit;

	// 设置没有源单元的单元(输入参数连接的单元)的处理优先值为1(最先处理).
	for (int i = 0; i < iTotal; i++) {
		punit = plistUnit->GetNext(poUnit);
		if (!punit->IsHaveSourceUnit()) { // no one link to me ?
			punit->SetExectivePriority(1);
		}
	}
  return(true);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 封装单元序列中的部件.此时部件内的单元序列已经被编译了
//
// 测试封装部件时，只要替换掉这个函数，就能一步一步测试了。
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool EncapsulateUnitlist(CUnitList * pUnitList, CUnitList & listTotalUnit) {
  CUnitBase * pcunit;
  
  INT64 iTotal = pUnitList->GetCount();
  POSITION poUnit = pUnitList->GetHeadPosition();
  for (int i = 0; i < iTotal; i++) {
    pcunit = pUnitList->GetNext(poUnit);
    if ( pcunit->IsEncapsulated() ) ASSERT(pcunit->IsCompiled());
    pcunit->Encapsulation(listTotalUnit);
  }
  return(true);
}

bool CheckUnitListCompiledStatus(CUnitList * pUnitList) {
  // 简单检测本层的单元序列（不包括部件）都设置了执行优先级
  CUnitBase *pcunit;
  INT64 iTotal = pUnitList->GetCount();
  POSITION poUnit = pUnitList->GetHeadPosition();
  for (int i = 0; i < iTotal; i++) {
    pcunit = pUnitList->GetNext(poUnit);
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
  CUnitBase *pcunit;
  INT64 iTotal = pRunTimeUnitList->GetCount();
  POSITION poUnit = pRunTimeUnitList->GetHeadPosition();
  for (int i = 0; i < iTotal; i++) {
    pcunit = pRunTimeUnitList->GetNext(poUnit);
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
  POSITION poUnit;
  CUnitBase * pcunit;
  INT64 iRunTimeTemp = 0, iTotal;

  TRACE("Compile unit list\n");

  // clear runtime unitList
  pRunTimeUnitList->RemoveAll();
  // 开始编译
  ULONG iRunTimeTempOld = 0, iCurrentPriority = 0;
  BOOL done = FALSE, fFindLoop = FALSE;
  iTotal = unitlist.GetCount();
  while ((!done) && (!fFindLoop)) {
    iCurrentPriority++;
    poUnit = unitlist.GetHeadPosition();
    for (int i = 0; i < iTotal; i++) {
      pcunit = unitlist.GetNext(poUnit);
      if (pcunit->GetExectivePriority() == iCurrentPriority) {
        pcunit->SetDestUnitPriority();
        pcunit->SetCompiledFlag(true);
        pRunTimeUnitList->AddTail(pcunit); // create runtime list
        TRACE("%s(%u)\n", (LPCTSTR)(pcunit->GetName()), pcunit->GetExectivePriority());
      }
    }
    iRunTimeTemp = pRunTimeUnitList->GetCount();

    if (iRunTimeTemp == iTotal) {		// 全找到了
      done = TRUE;
    }
    else if (iRunTimeTemp == iRunTimeTempOld) {// 如无法找到更多的单元(全找到了或还剩下循环)
      fFindLoop = TRUE; // 发现了循环存在
    }
    else iRunTimeTempOld = iRunTimeTemp;

    if (iRunTimeTemp > iTotal) {    // something was wrong 
      pRunTimeUnitList->RemoveAll();
      return(FALSE);
    }
  }

  // 如有循环剩下,则将循环开始单元(cut_off unit)的优先值设为iCurrentPriority.
  if (fFindLoop) {
    poUnit = unitlist.GetHeadPosition();
    for (int i = 0; i < iTotal; i++) {
      pcunit = unitlist.GetNext(poUnit);
      if (pcunit->IsSetCutOff()) {
        pcunit->SetExectivePriorityDirect(iCurrentPriority);
        pcunit->SetCompiledFlag(true);
      }
    }

    // 处理与循环有关的单元.
    iRunTimeTempOld = 0;
    while (!done) {
      poUnit = unitlist.GetHeadPosition();
      for (int i = 0; i < iTotal; i++) {
        pcunit = unitlist.GetNext(poUnit);
        if (pcunit->GetExectivePriority() == iCurrentPriority) {
          pcunit->SetDestUnitPriority();
          pcunit->SetCompiledFlag(true);
          pRunTimeUnitList->AddTail(pcunit);     // create runtime list
          TRACE("%s(%u)\n", (LPCTSTR)(pcunit->GetName()),
            pcunit->GetExectivePriority());
        }
      }
      iRunTimeTemp = pRunTimeUnitList->GetCount();
      if (iRunTimeTemp == iRunTimeTempOld) done = TRUE;
      else {
        iRunTimeTempOld = iRunTimeTemp;
      }
      if (iRunTimeTemp == iTotal) done = TRUE;
      if (iRunTimeTemp > iTotal) {    // something was wrong 
        pRunTimeUnitList->RemoveAll();
        return(FALSE);
      }
      iCurrentPriority++;
    }
    ASSERT(iRunTimeTemp == iTotal);
  } // fFindLoop

  return(true);
}

void ReSetCompileFlag(CUnitList * pUnitList)
{
  CUnitBase * pcUnit;
  INT64 iTotal;

  // 重置编译标志。这步现在要做，以防止出现可能的误存储。
  POSITION poUnit = pUnitList->GetHeadPosition();
  iTotal = pUnitList->GetCount();
  for (int i = 0; i < iTotal; i++) {
    pcUnit = pUnitList->GetNext(poUnit);
    pcUnit->ResetCompileFlag();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// 清除单元序列中残存的旧标志，然后设置正确的内部状态以备编译。
//
///////////////////////////////////////////////////////////////////////////////
void SetParaLockFlag(CUnitList * pUnitList, CObjectList * pObjectList) {
  CUnitBase * pcUnit;
  INT64 iTotal;
  POSITION poUnit = pUnitList->GetHeadPosition();
  iTotal = pUnitList->GetCount();

  // set unit select parameter
  poUnit = pUnitList->GetHeadPosition();
  for (int i = 0; i < iTotal; i++) {
    pcUnit = pUnitList->GetNext(poUnit);
    pcUnit->SetParaLockFlag();
  }

  POSITION poObj = pObjectList->GetHeadPosition();
  iTotal = pObjectList->GetCount();
  CObjectBase * pcObj;
  for (int i = 0; i < iTotal; i++) {
    pcObj = pObjectList->GetNext(poObj);
    pcObj->SetParameterSelected();
  }
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
//   这个函数是生成运行时态单元序列.在此之前，设置了输入参数个数，清除了编译标志和执行优先级
//    
////////////////////////////////////////////////////////////////////////
bool CompileUnitList( CUnitList * pUnitList, CUnitList * pRunTimeUnitList) {
	CUnitList unitlist, rtUnitList;

	TRACE("Compile unit list\n");

  // 在此之前，单元已经清除编译标志和执行优先级，设置了输入参数个数
  // 将所有的单元(包括部件本身）组成一个单独的单元序列. 此时部件尚未进行下层部件的封装，故而下层部件内的单元也添加进此单元序列中。这是为了测试是否有循环存在
  CreateUniUnitList(pUnitList, unitlist);

  // 检测是否有循环形成
	if (UnitListLoopDetect(&unitlist)) return(FALSE); 

	// 设置没有源单元的单元(输入参数连接的单元)的处理优先值为1(最先处理).
  SetNoSrcUnitExectivePriority(&unitlist);

  // 开始编译
  ExectiveCompilation(unitlist, &rtUnitList);
	
  // 封装本单元序列中的部件.此时部件内的单元序列已经被编译了
  EncapsulateUnitlist(pUnitList, unitlist);
 
  // 以下两个只是检查而已。
  // 简单检测本层的单元序列（不包括部件）都设置了执行优先级
  CheckUnitListCompiledStatus(pUnitList);

  // 简单检测运行时单元序列（除部件外）都设置了执行优先级
  CheckRunTimeUnitListCompiledStatus(&rtUnitList);

  // 此时单元序列pUnitList已经编译了，故而可封装的部件已经封装，不再将内部单元序列加入运行时单元序列.
  // 不可封装的部件仍然将内部单元序列加入unitlist中。
  unitlist.RemoveAll();
  POSITION poUnit = pUnitList->GetHeadPosition();
  CUnitBase * pcunit;
  INT64 iTotal = pUnitList->GetCount();
  for (int i = 0; i < iTotal; i++) {
    pcunit = pUnitList->GetNext(poUnit);
    pcunit->AddToList(unitlist);
  }

  // 将rtUnitList中存在于unitlist单元序列的单元加入pRunTimeUnitList。
  ASSERT(pRunTimeUnitList->IsEmpty());
  poUnit = rtUnitList.GetHeadPosition();
  iTotal = rtUnitList.GetCount();
  for (int i = 0; i < iTotal; i++) {
    pcunit = rtUnitList.GetNext(poUnit);
    if (unitlist.Find(pcunit)) {
      pRunTimeUnitList->AddTail(pcunit);
    }
  }
  ASSERT(pRunTimeUnitList->GetCount() == unitlist.GetCount());

  unitlist.RemoveAll();
  rtUnitList.RemoveAll();
	return (TRUE);
}

