
#pragma once

#include"stdafx.h"

#include"CUnitBase.h"
#include"CObjectBase.h"

// 重置编译标志
void ReSetCompileFlag(CUnitList *pUnitList);
// 设置单元序列中的内部状态
void SetParaLockFlag(CUnitList * pUnitList, CObjectList * pObjectList);

//此函数是由外部调用的
bool CompileUnitList(CUnitList * pUnitList, CUnitList * pRunTimeUnitList);

// 以下函数用于单元序列的编译，不允许外部调用。列于此处只是为了测试函数的调用方便
void ClearLoopDetectFlag(CUnitList * pUnitList);
bool UnitListLoopDetect(CUnitList * pUnitList);
bool AlreadyHaveCutOff(CUnitBase * pCUnit, CUnitList * pUnitList);
bool CreateUniUnitList(CUnitList * pUnitList, CUnitList &listUniUnit);
bool SetNoSrcUnitExectivePriority(CUnitList * plistUnit);
bool ExectiveCompilation(CUnitList & listUnit, CUnitList * pRunTimeUnitList);
bool EncapsulateUnitlist(CUnitList * pUnitList, CUnitList & listTotalUnit);
