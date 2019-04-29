#pragma once

#include"stdafx.h"

#include"CUnitBase.h"
#include"CObjectBase.h"

// 重置编译标志
void ReSetCompileFlagOld1(CUnitList *pUnitList);
// 设置单元序列中的内部状态
void SetParaLockFlagOLd1(CUnitList * pUnitList, CObjectList * pObjectList);

//此函数是由外部调用的
bool CompileUnitListOld1(CUnitList * pUnitList, CUnitList * pRunTimeUnitList);

// 以下函数用于单元序列的编译，不允许外部调用。列于此处只是为了测试函数的调用方便
void ClearLoopDetectFlagOld1(CUnitList * pUnitList);
bool UnitListLoopDetectOld1(CUnitList * pUnitList);
bool AlreadyHaveCutOffOld1(CUnitBase * pCUnit, CUnitList * pUnitList);
bool CreateUniUnitListOld1(CUnitList * pUnitList, CUnitList &listUniUnit);
bool SetNoSrcUnitExectivePriorityOld1(CUnitList * plistUnit);
bool ExectiveCompilationOld1(CUnitList & listUnit, CUnitList * pRunTimeUnitList);
bool EncapsulateUnitlistOld1(CUnitList * pUnitList, CUnitList & listTotalUnit);
