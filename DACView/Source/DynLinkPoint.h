#pragma once

#include"typedef.h"

// ���ɶ�̬�����ߣ������յ�
void CreateDynLinkPoint(CPointList *m_plistLinkPoint, CRect rectSecond, CPoint ptFirst, CPoint ptSecond, CPoint ptCurrent);

// ���ɶ�̬�����ߣ������յ�
void CreateDynLinkPoint(CPointList *plistLinkPoint, CPoint *pptFirst, CPoint *pptSecond, CPoint ptCurrent);

// ������̬���ӵ㡣ptFirst��ptSecond��ֵ���޸ģ���Ҫ����ָ�롣
void AdjustDynLinkPoint(CRect rectFirstUnit, CPoint &ptFirst, CPoint &ptSecond, CPoint ptCurrent); //��һ�δ���
void AdjustDynLinkPoint(CPoint &ptFirst, CPoint &ptSecond, CPoint ptCurrent); // �ڶ��μ��Ժ󴴽�

