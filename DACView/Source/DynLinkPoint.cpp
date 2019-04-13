#include"stdafx.h"

#include"DynLinkPoint.h"

//////////////////////////////////////////////////////////////////////////////////////
//
// ���ɶ�̬���ӵĶ�̬�����ߡ�
//
/////////////////////////////////////////////////////////////////////////////////////////

// ����ʼ��
void CreateDynLinkPoint(CPointList *plistLinkPoint, CPoint *pptFirst, CPoint *pptSecond, CPoint ptCurrent) {
  CPoint *ppt2, *ppt1, *ppt3;

  if (plistLinkPoint->GetCount() == 1) { // ��㿪ʼ 
    ppt1 = plistLinkPoint->GetTail(); // ��ʱlist���Ѿ�Ԥ������ʼ��
    *ppt1 = *pptFirst;		// ������ʼ���λ��Ϊ��һ�����λ��
    ppt2 = new CPoint;
    plistLinkPoint->AddTail(ppt2);		// ���ڶ��������list��

    if ((ptCurrent.x == pptFirst->x) || (ptCurrent.y == pptFirst->y)) { // �����ǰ�㣨m_ptCurrent)λ�ڵ�һ����Ԫ�����ϡ����¡�����������ҷ�
      *ppt2 = ptCurrent; // ��ڶ�������ǵ�ǰ�㣬��ʱlist����������
    }
    else { // ��һ����λ�ڵ�һ����Ԫ�����ϡ����¡����ϻ������·�����ʱ��Ҫ������
      *ppt2 = *pptSecond; // ��ڶ������ǵڶ����λ��
      ppt3 = new CPoint;		// ���ɵ�������
      *ppt3 = ptCurrent;	// ���������λ��Ϊ��ǰ��
      plistLinkPoint->AddTail(ppt3); // ��ʱlist����������
      // ������ʱ�����Ƶ��Ҫ������һ��
      *pptFirst = *pptSecond;
      *pptSecond = ptCurrent;
    }
  }
  else { // �����
    ppt1 = plistLinkPoint->GetTail();
    if (ptCurrent.x == pptFirst->x) {
      ppt1->y = pptSecond->y;
    }
    else if (ptCurrent.y == pptFirst->y) {
      ppt1->x = pptSecond->x;
    }
    else {
      *ppt1 = *pptSecond;
    }
    ppt2 = new CPoint;
    *ppt2 = ptCurrent;
    plistLinkPoint->AddTail(ppt2);
    //���Ƶ�������һ��
    *pptFirst = *pptSecond;
    *pptSecond = ptCurrent;
  }
}

// ���յ�
void CreateDynLinkPoint(CPointList *plistLinkPoint, CRect rectSecond,
                        CPoint ptFirst, CPoint ptSecond, CPoint ptCurrent) {
  CPoint *ppt, *ppt1, *ppt2;

  // �����Ѿ�������һ���㣬��������һ������������㣨���ߣ�
  ASSERT(plistLinkPoint->GetCount() >= 1);
  if (plistLinkPoint->GetCount() == 1) { // ֻ��һ���㣬��ô���������յ㶼����
    ppt = plistLinkPoint->GetHead();
    *ppt = ptFirst; // �����������
    ppt = new CPoint;
    *ppt = ptSecond;
    plistLinkPoint->AddTail(ppt); // ��Ҫ��ptSecond�ӽ�ȥ��
  }
  ppt1 = plistLinkPoint->GetTail();
  if ((ptFirst.x == ptCurrent.x) || (ptFirst.y == ptCurrent.y)) { // ����ֱ��
    if (ppt1->x < rectSecond.left) ppt1->x = rectSecond.left;
    else if (ppt1->x > rectSecond.right) ppt1->x = rectSecond.right;
    else if (ppt1->y < rectSecond.top) ppt1->y = rectSecond.top;
    else if (ppt1->y > rectSecond.bottom) ppt1->y = rectSecond.bottom;
  }
  else { // ��������
    *ppt1 = ptSecond;
    ppt2 = new CPoint;
    if (ptSecond.x < rectSecond.left) {
      ppt2->x = rectSecond.left;
      ppt2->y = ptSecond.y;
    }
    else if (ptSecond.x > rectSecond.right) {
      ppt2->x = rectSecond.right;
      ppt2->y = ptSecond.y;
    }
    else if (ptSecond.y < rectSecond.top) {
      ppt2->y = rectSecond.top;
      ppt2->x = ptSecond.x;
    }
    else if (ptSecond.y > rectSecond.bottom) {
      ppt2->y = rectSecond.bottom;
      ppt2->x = ptSecond.x;
    }
    plistLinkPoint->AddTail(ppt2);
  }

  if (plistLinkPoint->GetCount() == 2) { // ֻ��������,����������е㣨�������ƶ���Ԫʱ���������б�ߣ�
    ppt1 = plistLinkPoint->GetTail();
    ppt2 = new CPoint;
    ppt2->x = (ptFirst.x + ppt1->x) / 2;
    ppt2->y = (ptFirst.y + ppt1->y) / 2;
    POSITION po = plistLinkPoint->GetHeadPosition();
    plistLinkPoint->InsertAfter(po, ppt2);
    ppt = new CPoint;
    *ppt = *ppt2;
    plistLinkPoint->InsertAfter(po, ppt);
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����Ԫ���ߵ�Ԫ��̬���ӵĵ�Ԫλ�÷����仯ʱ�����㶯̬�����ߵ�λ�á�
// 
// ����ptFirst��ptSecond��Ҫ�޸ġ�
//
// ����ǵ�һ�δ���ʱ���������Ҫ�ж��Ǵ���һ��ֱ�߻���һ�����ߡ�
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AdjustDynLinkPoint(CRect rectFirstUnit, CPoint &ptFirst, CPoint &ptSecond, CPoint ptCurrent) {
  if ((ptCurrent.x >= rectFirstUnit.left) && (ptCurrent.x < rectFirstUnit.right)) { // λ�����ϻ�������
    if (ptCurrent.y < rectFirstUnit.top) { // λ������
      ptFirst.y = rectFirstUnit.top;
      ptSecond.y = ptCurrent.y;
      ptFirst.x = ptSecond.x = ptCurrent.x;
    }
    else { // λ������
      ptFirst.y = rectFirstUnit.bottom;
      ptSecond.y = ptCurrent.y;
      ptFirst.x = ptSecond.x = ptCurrent.x;
    }
  }
  else if ((ptCurrent.y >= rectFirstUnit.top) && (ptCurrent.y < rectFirstUnit.bottom)) { // λ����������
    if (ptCurrent.x < rectFirstUnit.left) { // λ������
      ptFirst.x = rectFirstUnit.left;
      ptSecond.x = ptCurrent.x;
      ptFirst.y = ptSecond.y = ptCurrent.y;
    }
    else { // λ������
      ptFirst.x = rectFirstUnit.right;
      ptSecond.x = ptCurrent.x;
      ptFirst.y = ptSecond.y = ptCurrent.y;
    }
  }
  else { // λ���Ľ�
    if ( ptFirst.x == ptSecond.x ) { // �����º�����
      ptSecond.y = ptCurrent.y;
      if (ptCurrent.x < rectFirstUnit.left) { // λ�����
        ptFirst.x = ptSecond.x = rectFirstUnit.left;
        //ptFirst.y = rectFirstUnit.bottom;
      }
      else { // λ���ұ�
        ptFirst.x = ptSecond.x = rectFirstUnit.right;
      }
    }
    else if (ptFirst.y == ptSecond.y) { // �����Һ�����
      ptSecond.x = ptCurrent.x;
      if (ptCurrent.y < rectFirstUnit.top) { // λ���ϱ�
        ptFirst.y = ptSecond.y = rectFirstUnit.top;
      }
      else { // λ���±�
        ptFirst.y = ptSecond.y = rectFirstUnit.bottom;
      }
    }
  }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ����Ԫ���ߵ�Ԫ��̬���ӵĵ�Ԫλ�÷����仯ʱ�����㶯̬�����ߵ�λ�á�
// 
// ����ptFirst��ptSecond��Ҫ�޸ġ�
//
// ����ǵڶ��δ��������ʱ�������ֻ�Ǵ���һ��ֱ��
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AdjustDynLinkPoint(CPoint &ptFirst, CPoint &ptSecond, CPoint ptCurrent) {
  if (ptSecond.x == ptFirst.x) ptSecond.y = ptCurrent.y;
  else ptSecond.x = ptCurrent.x;
}