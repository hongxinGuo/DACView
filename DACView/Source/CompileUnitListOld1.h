#pragma once

#include"stdafx.h"

#include"CUnitBase.h"
#include"CObjectBase.h"

// ���ñ����־
void ReSetCompileFlagOld1(CUnitList *pUnitList);
// ���õ�Ԫ�����е��ڲ�״̬
void SetParaLockFlagOLd1(CUnitList * pUnitList, CObjectList * pObjectList);

//�˺��������ⲿ���õ�
bool CompileUnitListOld1(CUnitList * pUnitList, CUnitList * pRunTimeUnitList);

// ���º������ڵ�Ԫ���еı��룬�������ⲿ���á����ڴ˴�ֻ��Ϊ�˲��Ժ����ĵ��÷���
void ClearLoopDetectFlagOld1(CUnitList * pUnitList);
bool UnitListLoopDetectOld1(CUnitList * pUnitList);
bool AlreadyHaveCutOffOld1(CUnitBase * pCUnit, CUnitList * pUnitList);
bool CreateUniUnitListOld1(CUnitList * pUnitList, CUnitList &listUniUnit);
bool SetNoSrcUnitExectivePriorityOld1(CUnitList * plistUnit);
bool ExectiveCompilationOld1(CUnitList & listUnit, CUnitList * pRunTimeUnitList);
bool EncapsulateUnitlistOld1(CUnitList * pUnitList, CUnitList & listTotalUnit);
