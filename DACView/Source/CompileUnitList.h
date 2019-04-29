
#pragma once

#include"stdafx.h"

#include"CUnitBase.h"
#include"CObjectBase.h"

// ���ñ����־
void ReSetCompileFlag(CUnitList *pUnitList);
// ���õ�Ԫ�����е��ڲ�״̬
void SetParaLockFlag(CUnitList * pUnitList, CObjectList * pObjectList);

//�˺��������ⲿ���õ�
bool CompileUnitList(CUnitList * pUnitList, CUnitList * pRunTimeUnitList);

// ���º������ڵ�Ԫ���еı��룬�������ⲿ���á����ڴ˴�ֻ��Ϊ�˲��Ժ����ĵ��÷���
void ClearLoopDetectFlag(CUnitList * pUnitList);
bool UnitListLoopDetect(CUnitList * pUnitList);
bool AlreadyHaveCutOff(CUnitBase * pCUnit, CUnitList * pUnitList);
bool CreateUniUnitList(CUnitList * pUnitList, CUnitList &listUniUnit);
bool SetNoSrcUnitExectivePriority(CUnitList * plistUnit);
bool ExectiveCompilation(CUnitList & listUnit, CUnitList * pRunTimeUnitList);
bool EncapsulateUnitlist(CUnitList * pUnitList, CUnitList & listTotalUnit);
