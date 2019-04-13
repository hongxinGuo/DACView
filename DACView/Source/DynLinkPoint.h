#pragma once

#include"typedef.h"

// 生成动态链接线，已有终点
void CreateDynLinkPoint(CPointList *m_plistLinkPoint, CRect rectSecond, CPoint ptFirst, CPoint ptSecond, CPoint ptCurrent);

// 生成动态连接线，尚无终点
void CreateDynLinkPoint(CPointList *plistLinkPoint, CPoint *pptFirst, CPoint *pptSecond, CPoint ptCurrent);

// 调整动态链接点。ptFirst和ptSecond的值被修改，需要传入指针。
void AdjustDynLinkPoint(CRect rectFirstUnit, CPoint &ptFirst, CPoint &ptSecond, CPoint ptCurrent); //第一次创建
void AdjustDynLinkPoint(CPoint &ptFirst, CPoint &ptSecond, CPoint ptCurrent); // 第二次及以后创建

