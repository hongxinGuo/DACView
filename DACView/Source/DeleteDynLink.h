#pragma once

// 删除本单元所有的动态链接
void DeleteDynLinkFromMe(CUnitBase * pUnit);
// 删除pUnitList和pObjectList中所有指向本单元的动态链接
void DeleteDynLinkToMe(CUnitList * pUnitList, CObjectList * pObjectList, CUnitBase * pUnit);
