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
// ��Ԫ���ж�̬���ӵ�ѭ�����Ӳ��ԡ�
//
// Return :
//		BOOL : TRUE if find a loop DynLink in m_CUnitList
//
//		������
//			CUnitList * pUnitList; // �����ҵĵ�Ԫ���С�
//
//
// Description :
//	��鵥Ԫ�������Ƿ���ڶ�̬���ӵ�ѭ��,���Ҽ���Ƿ�һ��ѭ������һ�����ϵĽضϵ㡣
//	
//  �����ǽ��Լ����ڲ���Ԫ������ӽ����鵥Ԫ�����У�
//  �����ڷ�װ���������򵥵�Ԫһ�����ʶ������������֮���γ���·������Ҫ�ڲ�������������ǣ���ʱ���������Ӵ����������Ķ�̬����Ҳ�γɻ�·��
//  ��ô�����ϲ��γ��˻�·����Ҫ���ضϴ���
//  Ŀǰ��δ��д�����װ�󲿼��ľ�����̡�2019.02.18 
//
//   Ϊ�˼�ϵͳά����ѭ������ҪԽ��Խ�ã��ʶ�Ӧ�ý��˺������������µ�Ԫ֮ʱ��������һ���µ�Ԫ�󣬻��ߴӼ�����paste����һ���µ�Ԫ������ִ��ѭ������
//
////////////////////////////////////////////////////////////////////////////////////////
BOOL UnitListLoopDetect(CUnitList * pCUnitList) {
	POSITION po, poUnit = pCUnitList->GetHeadPosition();
	CUnitBase * pcunit, *pCUnit;
	INT_PTR i, j, jTemp, iTemp = pCUnitList->GetCount();
	CUnitList unitlist;
	BOOL fFind = FALSE;

	// Ѱ���Ƿ���ѭ��.
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
//		CUnitBase * pCUnit : ����ĵ�Ԫ
//		CUnitList * pUnitList �����ĵ�Ԫ����
//
// Return : 
//		BOOL : ����̬����ѭ�����Ѿ����ڴ�����һ���ضϣ������棻���򷵻ؼ�
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
// �����еĵ�Ԫ��ɵ�Ԫ���У����������ڲ���Ԫ���кͲ���������롣
//
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CreateUniUnitList(CUnitList * pUnitList, CUnitList &listUniUnit) {
  POSITION poUnit = pUnitList->GetHeadPosition();
  CUnitBase * pcunit;
  INT64 iTotal = pUnitList->GetCount();

  // �ڴ�֮ǰ����Ԫ�Ѿ���������־��ִ�����ȼ��������������������
  // �����еĵ�Ԫ(���������������һ�������ĵ�Ԫ����. ��ʱ������δ�����²㲿���ķ�װ���ʶ��²㲿���ڵĵ�ԪҲ��ӽ��˵�Ԫ�����С�����Ϊ�˲����Ƿ���ѭ������
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
// ����û��Դ��Ԫ�ĵ�Ԫ(����������ӵĵ�Ԫ)�Ĵ�������ֵΪ���(1�����ȴ���).
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
bool SetNoSrcUnitExectivePriority(CUnitList * plistUnit) { 
	POSITION poUnit = plistUnit->GetHeadPosition();
	INT64 iTotal = plistUnit->GetCount();
  CUnitBase * punit;

	// ����û��Դ��Ԫ�ĵ�Ԫ(����������ӵĵ�Ԫ)�Ĵ�������ֵΪ1(���ȴ���).
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
// ��װ��Ԫ�����еĲ���.��ʱ�����ڵĵ�Ԫ�����Ѿ���������
//
// ���Է�װ����ʱ��ֻҪ�滻���������������һ��һ�������ˡ�
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
  // �򵥼�Ȿ��ĵ�Ԫ���У���������������������ִ�����ȼ�
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
  // �򵥼������ʱ��Ԫ���У��������⣩��������ִ�����ȼ�
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
// ������֯�õĵ�Ԫ���У��������е�Ԫ��ִ�����ȼ���
// ��װ�����Ĺ���������������ȥ��ɡ�
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
  // ��ʼ����
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

    if (iRunTimeTemp == iTotal) {		// ȫ�ҵ���
      done = TRUE;
    }
    else if (iRunTimeTemp == iRunTimeTempOld) {// ���޷��ҵ�����ĵ�Ԫ(ȫ�ҵ��˻�ʣ��ѭ��)
      fFindLoop = TRUE; // ������ѭ������
    }
    else iRunTimeTempOld = iRunTimeTemp;

    if (iRunTimeTemp > iTotal) {    // something was wrong 
      pRunTimeUnitList->RemoveAll();
      return(FALSE);
    }
  }

  // ����ѭ��ʣ��,��ѭ����ʼ��Ԫ(cut_off unit)������ֵ��ΪiCurrentPriority.
  if (fFindLoop) {
    poUnit = unitlist.GetHeadPosition();
    for (int i = 0; i < iTotal; i++) {
      pcunit = unitlist.GetNext(poUnit);
      if (pcunit->IsSetCutOff()) {
        pcunit->SetExectivePriorityDirect(iCurrentPriority);
        pcunit->SetCompiledFlag(true);
      }
    }

    // ������ѭ���йصĵ�Ԫ.
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

  // ���ñ����־���ⲽ����Ҫ�����Է�ֹ���ֿ��ܵ���洢��
  POSITION poUnit = pUnitList->GetHeadPosition();
  iTotal = pUnitList->GetCount();
  for (int i = 0; i < iTotal; i++) {
    pcUnit = pUnitList->GetNext(poUnit);
    pcUnit->ResetCompileFlag();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// �����Ԫ�����вд�ľɱ�־��Ȼ��������ȷ���ڲ�״̬�Ա����롣
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
//				CUnitList * pUnitList;  ����ĵ�Ԫ����
//				CUnitList * pRunTimeUnitList; // ����������ʱ��Ԫ����
//
// Return :
//    BOOL      : TRUE if secuess created.
//              : FALSE if not created runtime unit list
//
// Description :
//   �����������������ʱ̬��Ԫ����.�ڴ�֮ǰ�������������������������˱����־��ִ�����ȼ�
//    
////////////////////////////////////////////////////////////////////////
bool CompileUnitList( CUnitList * pUnitList, CUnitList * pRunTimeUnitList) {
	CUnitList unitlist, rtUnitList;

	TRACE("Compile unit list\n");

  // �ڴ�֮ǰ����Ԫ�Ѿ���������־��ִ�����ȼ��������������������
  // �����еĵ�Ԫ(���������������һ�������ĵ�Ԫ����. ��ʱ������δ�����²㲿���ķ�װ���ʶ��²㲿���ڵĵ�ԪҲ��ӽ��˵�Ԫ�����С�����Ϊ�˲����Ƿ���ѭ������
  CreateUniUnitList(pUnitList, unitlist);

  // ����Ƿ���ѭ���γ�
	if (UnitListLoopDetect(&unitlist)) return(FALSE); 

	// ����û��Դ��Ԫ�ĵ�Ԫ(����������ӵĵ�Ԫ)�Ĵ�������ֵΪ1(���ȴ���).
  SetNoSrcUnitExectivePriority(&unitlist);

  // ��ʼ����
  ExectiveCompilation(unitlist, &rtUnitList);
	
  // ��װ����Ԫ�����еĲ���.��ʱ�����ڵĵ�Ԫ�����Ѿ���������
  EncapsulateUnitlist(pUnitList, unitlist);
 
  // ��������ֻ�Ǽ����ѡ�
  // �򵥼�Ȿ��ĵ�Ԫ���У���������������������ִ�����ȼ�
  CheckUnitListCompiledStatus(pUnitList);

  // �򵥼������ʱ��Ԫ���У��������⣩��������ִ�����ȼ�
  CheckRunTimeUnitListCompiledStatus(&rtUnitList);

  // ��ʱ��Ԫ����pUnitList�Ѿ������ˣ��ʶ��ɷ�װ�Ĳ����Ѿ���װ�����ٽ��ڲ���Ԫ���м�������ʱ��Ԫ����.
  // ���ɷ�װ�Ĳ�����Ȼ���ڲ���Ԫ���м���unitlist�С�
  unitlist.RemoveAll();
  POSITION poUnit = pUnitList->GetHeadPosition();
  CUnitBase * pcunit;
  INT64 iTotal = pUnitList->GetCount();
  for (int i = 0; i < iTotal; i++) {
    pcunit = pUnitList->GetNext(poUnit);
    pcunit->AddToList(unitlist);
  }

  // ��rtUnitList�д�����unitlist��Ԫ���еĵ�Ԫ����pRunTimeUnitList��
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

