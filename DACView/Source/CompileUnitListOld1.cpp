#include "stdafx.h"

#include "globeDef.h"

#include"CompileUnitListOld1.h"

#include "cUnitComponent.h"

#include "CUnitDynLink.h"

#include "..\\resource.h"

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
// ��Ԫ���ж�̬���ӵ�ѭ�����Ӳ��ԡ�
//
// Return :
//		bool : TRUE if find a loop DynLink in m_CUnitList
//
//		������
//			CUnitList * pUnitList; // �����ҵĵ�Ԫ���С�
//
//
// Description :
//	��鵥Ԫ�������Ƿ���ڶ�̬���ӵ�ѭ��,���Ҽ���Ƿ�һ��ѭ������һ�����ϵĽضϵ㡣
//	
//  �����ǽ��Լ����ڲ���Ԫ�������ӽ����鵥Ԫ�����У�
//  �����ڷ�װ���������򵥵�Ԫһ�����ʶ������������֮���γ���·������Ҫ�ڲ�������������ǣ���ʱ���������Ӵ����������Ķ�̬����Ҳ�γɻ�·��
//  ��ô�����ϲ��γ��˻�·����Ҫ���ضϴ�����
//  Ŀǰ��δ��д������װ�󲿼��ľ�����̡�2019.02.18 
//
//   Ϊ�˼�ϵͳά����ѭ������ҪԽ��Խ�ã��ʶ�Ӧ�ý��˺������������µ�Ԫ֮ʱ��������һ���µ�Ԫ�󣬻��ߴӼ�����paste����һ���µ�Ԫ������ִ��ѭ������
//
////////////////////////////////////////////////////////////////////////////////////////
bool UnitListLoopDetect(CUnitList * pCUnitList) {
	CUnitBase * pcunit1, *pCUnit;
	CUnitList unitlist;
	BOOL fFind = false;

	// Ѱ���Ƿ���ѭ��.
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
//		CUnitBase * pCUnit : ����ĵ�Ԫ
//		CUnitList * pUnitList �����ĵ�Ԫ����
//
// Return : 
//		bool : ����̬����ѭ�����Ѿ����ڴ�����һ���ضϣ������棻���򷵻ؼ�
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
// �����еĵ�Ԫ��ɵ�Ԫ���У����������ڲ���Ԫ���кͲ����������롣
//
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CreateUniUnitList(CUnitList * pUnitList, CUnitList &listUniUnit) {
  // �ڴ�֮ǰ����Ԫ�Ѿ���������־��ִ�����ȼ��������������������
  // �����еĵ�Ԫ(�����������������һ�������ĵ�Ԫ����. ��ʱ������δ�����²㲿���ķ�װ���ʶ��²㲿���ڵĵ�ԪҲ���ӽ��˵�Ԫ�����С�����Ϊ�˲����Ƿ���ѭ������
  for (const auto pcunit : *pUnitList) {
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
	// ����û��Դ��Ԫ�ĵ�Ԫ(����������ӵĵ�Ԫ)�Ĵ�������ֵΪ1(���ȴ���).
	for (const auto punit : *plistUnit) {
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
  for (const auto pcunit : *pUnitList) {
    if ( pcunit->IsEncapsulated() ) ASSERT(pcunit->IsCompiled());
    pcunit->Encapsulation(listTotalUnit);
  }
  return(true);
}

bool CheckUnitListCompiledStatus(CUnitList * pUnitList) {
  // �򵥼�Ȿ��ĵ�Ԫ���У���������������������ִ�����ȼ�
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
  // �򵥼������ʱ��Ԫ���У��������⣩��������ִ�����ȼ�
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
// ������֯�õĵ�Ԫ���У��������е�Ԫ��ִ�����ȼ���
// ��װ�����Ĺ���������������ȥ��ɡ�
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool ExectiveCompilation(CUnitList &unitlist, CUnitList * pRunTimeUnitList) {
  INT64 iRunTimeTemp = 0, iTotal;

  TRACE("Start unit list's compilation\n");

  // clear runtime unitList
  pRunTimeUnitList->clear();
  // ��ʼ����
  INT64 iRunTimeTempOld = 0, iCurrentPriority = 0;
  bool done = false, fFindLoop = false;
  iTotal = unitlist.size();
  while ((!done) && (!fFindLoop)) {
    iCurrentPriority++;
    for (const auto pcunit : unitlist) {
      if (pcunit->GetExectivePriority() == iCurrentPriority) { // �ҵ���ִ�����ȼ���ȵĵ�Ԫ
        pcunit->SetDestUnitPriority();    // ���ô˵�Ԫ��Ŀ�ĵ�Ԫִ�����ȼ�
        pcunit->SetCompiledFlag(true);
        pRunTimeUnitList->push_back(pcunit); // create runtime list
        TRACE("%s(%u)\n", (LPCTSTR)(pcunit->GetName()), pcunit->GetExectivePriority());
      }
    }
    iRunTimeTemp = pRunTimeUnitList->size();

    if (iRunTimeTemp == iTotal) {		// ȫ�ҵ���
      done = true;
    }
    else if (iRunTimeTemp == iRunTimeTempOld) {// ���޷��ҵ�����ĵ�Ԫ(ȫ�ҵ��˻�ʣ��ѭ��)
      fFindLoop = TRUE; // ������ѭ������
    }
    else iRunTimeTempOld = iRunTimeTemp;

    if (iRunTimeTemp > iTotal) {    // something was wrong 
      pRunTimeUnitList->clear();
      return(false);
    }
  }

  // ����ѭ��ʣ��,��ѭ����ʼ��Ԫ(cut_off unit)������ֵ��ΪiCurrentPriority.
  if (fFindLoop) {
    for (const auto pcunit : unitlist) {
      if (pcunit->IsSetCutOff()) {
        pcunit->SetExectivePriorityDirect(iCurrentPriority); // ����ʹ�ô�ֱ�����ú�����SetExectivePriority����ݲ�ͬ������ֱ���
        pcunit->SetCompiledFlag(true);
      }
    }

    // ������ѭ���йصĵ�Ԫ.
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

void ReSetCompileFlagOld1(CUnitList * pUnitList)
{
  // ���ñ����־���ⲽ����Ҫ�����Է�ֹ���ֿ��ܵ���洢��
  for (const auto pcUnit : *pUnitList) {
    pcUnit->ResetCompileFlag();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
// �����Ԫ�����вд�ľɱ�־��Ȼ��������ȷ���ڲ�״̬�Ա����롣
//
///////////////////////////////////////////////////////////////////////////////
void SetParaLockFlagOld1(CUnitList * pUnitList, CObjectList * pObjectList) {
  // set unit select parameter
  for (const auto pcUnit : *pUnitList) {
    pcUnit->SetParaLockFlag();
  }

  for (const auto pcObj : *pObjectList) {
    pcObj->SetParameterSelected();
  }
}

////////////////////////////////////////////////////////////////////////
//
// CompileUnitListOld1(). �˺�������ʹ�ã�3������Ϊ�Աȡ�
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
// �����ȱ�����װ�İ취��������ʱ�ᵼ�����ȼ����ִ��󣬾���������ǵ�����ɱ���װ�Ĳ���ʱ��
// ���������ȼ����ڱ��뵥Ԫ���е�����ȷ�ϵģ���������Ժ������ִ���
// �취��
// ����������޸�Ϊ�ȷ�װ����롣��װ����������ݸ���Ĺ��ܣ�����֮��ֻͨ�������Ĳ�������������ϵ������ÿ�������ı�����̶���һ���ģ�
// ��Ϊ�ṹ��ͬ�����ɷ�װ�Ĳ����ڱ���ʱ���ϲ㵥Ԫ����һ����롣
// 
//    
////////////////////////////////////////////////////////////////////////
bool CompileUnitListOld1(CUnitList * pUnitList, CUnitList * pRunTimeUnitList) {
  CUnitList unitlist, rtUnitList;

  TRACE("Compile unit list\n");

  // �ڴ�֮ǰ����Ԫ�Ѿ���������־��ִ�����ȼ��������������������
  // �����еĵ�Ԫ(�����������������һ�������ĵ�Ԫ����. ��ʱ������δ�����²㲿���ķ�װ���ʶ��²㲿���ڵĵ�ԪҲ���ӽ��˵�Ԫ�����С�����Ϊ�˲����Ƿ���ѭ������
  CreateUniUnitListOld1(pUnitList, unitlist);

  // ����Ƿ���ѭ���γ�
  if (UnitListLoopDetect(&unitlist)) return(FALSE);

  // ����û��Դ��Ԫ�ĵ�Ԫ(����������ӵĵ�Ԫ)�Ĵ�������ֵΪ1(���ȴ���).
  SetNoSrcUnitExectivePriorityOld1(&unitlist);

  // ��ʼ����
  ExectiveCompilationOld1(unitlist, &rtUnitList);

  // ��װ����Ԫ�����еĲ���.��ʱ�����ڵĵ�Ԫ�����Ѿ���������
  EncapsulateUnitlistOld1(pUnitList, unitlist);

  // ��������ֻ�Ǽ����ѡ�
  // �򵥼�Ȿ��ĵ�Ԫ���У���������������������ִ�����ȼ�
  CheckUnitListCompiledStatusOld1(pUnitList);

  // �򵥼������ʱ��Ԫ���У��������⣩��������ִ�����ȼ�
  CheckRunTimeUnitListCompiledStatusOld1(&rtUnitList);

  // ��ʱ��Ԫ����pUnitList�Ѿ������ˣ��ʶ��ɷ�װ�Ĳ����Ѿ���װ�����ٽ��ڲ���Ԫ���м�������ʱ��Ԫ����.
  // ���ɷ�װ�Ĳ�����Ȼ���ڲ���Ԫ���м���unitlist�С�
  unitlist.clear();
  for (const auto pcunit : *pUnitList) {
    pcunit->AddToList(unitlist);
  }

  // ��rtUnitList�д�����unitlist��Ԫ���еĵ�Ԫ����pRunTimeUnitList��
  ASSERT(pRunTimeUnitList->empty());
  for (const auto pcunit : rtUnitList) {
    if (find(unitlist.begin(), unitlist.end(), pcunit) != unitlist.end()) {
      pRunTimeUnitList->push_back(pcunit);
    }
  }
  ASSERT(pRunTimeUnitList->size() == unitlist.size());

  unitlist.clear();
  rtUnitList.clear();
  return (true);
}
