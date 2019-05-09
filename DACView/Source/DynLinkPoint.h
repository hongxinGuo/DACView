#ifndef __DYNAMIC_LINK_POINT_H__
#define __DYNAMIC_LINK_POINT_H__

#include"typedef.h"

// ���ɶ�̬�����ߣ������յ�
void CreateDynLinkPoint(CPointListPtr m_plistLinkPoint, CRect rectSecond, CPoint ptFirst, CPoint ptSecond, CPoint ptCurrent);

// ���ɶ�̬�����ߣ������յ�
void CreateDynLinkPoint(CPointListPtr plistLinkPoint, CPoint *pptFirst, CPoint *pptSecond, CPoint ptCurrent);

// ������̬���ӵ㡣ptFirst��ptSecond��ֵ���޸ģ���Ҫ����ָ�롣
void AdjustDynLinkPoint(CRect rectFirstUnit, CPoint &ptFirst, CPoint &ptSecond, CPoint ptCurrent); //��һ�δ���
void AdjustDynLinkPoint(CPoint &ptFirst, CPoint &ptSecond, CPoint ptCurrent); // �ڶ��μ��Ժ󴴽�

#endif
