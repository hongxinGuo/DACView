#ifndef __COMPILE_UNIT_LSIT_H__
#define __COMPILE_UNIT_LIST_H__

#include"stdafx.h"

#include"CUnitBase.h"
#include"CObjectBase.h"

// 重置编译标志
void ReSetCompileFlag(CUnitList *pUnitList);

// 设置单元序列中的内部状态
void SetParaLockFlag(CUnitList * pUnitList, CObjectList * pObjectList); // 这个是正常编译时用的
void SetParaLockFlag(CUnitList * pUnitList);                            // 这个用于Edit Paste中。

//此函数是由外部调用的
bool Compilation(CUnitList * pUnitList, CObjectList & objectlist, CUnitList * pRunTimeUnitList);

// 此函数是编译给定的单元序列
bool CompileUnitList(CUnitList * pUnitList, CUnitList * pRunTimeUnitList);

// 编译单元序列pUnitList中的可编译部件
bool CompileInnerComponent(CUnitList * pUnitList);

// 以下函数用于单元序列的编译，不允许外部调用。列于此处只是为了测试函数的调用方便
void ClearLoopDetectFlag(CUnitList * pUnitList);
bool UnitListLoopDetect(CUnitList * pUnitList);
bool AlreadyHaveCutOff(CUnitBase * pCUnit, CUnitList * pUnitList);
bool CreateUniUnitList(CUnitList * pUnitList, CUnitList &listUniUnit);
bool SetNoSrcUnitExectivePriority(CUnitList * plistUnit);
bool ExectiveCompilation(CUnitList * pUUnitList, CUnitList * pRunTimeUnitList);
bool EncapsulateUnitList(CUnitList & unitlist);
bool SetEncapsulatingFlag(CUnitList & unitlist);

#endif
