#ifndef __COMPILE_UNIT_LSIT_H__
#define __COMPILE_UNIT_LIST_H__

#include"stdafx.h"

#include"CUnitBase.h"
#include"CObjectBase.h"

// ���ñ����־
void ReSetCompileFlag(CUnitList *pUnitList);

// ���õ�Ԫ�����е��ڲ�״̬
void SetParaLockFlag(CUnitList * pUnitList, CObjectList * pObjectList); // �������������ʱ�õ�
void SetParaLockFlag(CUnitList * pUnitList);                            // �������Edit Paste�С�

//�˺��������ⲿ���õ�
bool Compilation(CUnitList * pUnitList, CObjectList & objectlist, CUnitList * pRunTimeUnitList);

// �˺����Ǳ�������ĵ�Ԫ����
bool CompileUnitList(CUnitList * pUnitList, CUnitList * pRunTimeUnitList);

// ���뵥Ԫ����pUnitList�еĿɱ��벿��
bool CompileInnerComponent(CUnitList * pUnitList);

// ���º������ڵ�Ԫ���еı��룬�������ⲿ���á����ڴ˴�ֻ��Ϊ�˲��Ժ����ĵ��÷���
void ClearLoopDetectFlag(CUnitList * pUnitList);
bool UnitListLoopDetect(CUnitList * pUnitList);
bool AlreadyHaveCutOff(CUnitBase * pCUnit, CUnitList * pUnitList);
bool CreateUniUnitList(CUnitList * pUnitList, CUnitList &listUniUnit);
bool SetNoSrcUnitExectivePriority(CUnitList * plistUnit);
bool ExectiveCompilation(CUnitList * pUUnitList, CUnitList * pRunTimeUnitList);
bool EncapsulateUnitList(CUnitList & unitlist);
bool SetEncapsulatingFlag(CUnitList & unitlist);

#endif
