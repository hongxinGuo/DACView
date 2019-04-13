#include"stdafx.h"

#include"DynLinkPoint.h"

//////////////////////////////////////////////////////////////////////////////////////
//
// 生成动态链接的动态链接线。
//
/////////////////////////////////////////////////////////////////////////////////////////

// 有起始点
void CreateDynLinkPoint(CPointList *plistLinkPoint, CPoint *pptFirst, CPoint *pptSecond, CPoint ptCurrent) {
  CPoint *ppt2, *ppt1, *ppt3;

  if (plistLinkPoint->GetCount() == 1) { // 起点开始 
    ppt1 = plistLinkPoint->GetTail(); // 此时list中已经预存了起始点
    *ppt1 = *pptFirst;		// 设置起始点的位置为第一个点的位置
    ppt2 = new CPoint;
    plistLinkPoint->AddTail(ppt2);		// 将第二个点加入list中

    if ((ptCurrent.x == pptFirst->x) || (ptCurrent.y == pptFirst->y)) { // 如果当前点（m_ptCurrent)位于第一个单元的正上、正下、正左或者正右方
      *ppt2 = ptCurrent; // 设第二个点就是当前点，此时list中有两个点
    }
    else { // 第一个点位于第一个单元的左上、左下、右上或者右下方，此时需要画折线
      *ppt2 = *pptSecond; // 设第二个点是第二点的位置
      ppt3 = new CPoint;		// 生成第三个点
      *ppt3 = ptCurrent;	// 第三个点的位置为当前点
      plistLinkPoint->AddTail(ppt3); // 此时list中有三个点
      // 三个点时，控制点就要往下移一个
      *pptFirst = *pptSecond;
      *pptSecond = ptCurrent;
    }
  }
  else { // 无起点
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
    //控制点往下移一个
    *pptFirst = *pptSecond;
    *pptSecond = ptCurrent;
  }
}

// 有终点
void CreateDynLinkPoint(CPointList *plistLinkPoint, CRect rectSecond,
                        CPoint ptFirst, CPoint ptSecond, CPoint ptCurrent) {
  CPoint *ppt, *ppt1, *ppt2;

  // 表中已经有至少一个点，生成最后的一个点或者两个点（折线）
  ASSERT(plistLinkPoint->GetCount() >= 1);
  if (plistLinkPoint->GetCount() == 1) { // 只有一个点，那么就是起点和终点都有了
    ppt = plistLinkPoint->GetHead();
    *ppt = ptFirst; // 修正起点坐标
    ppt = new CPoint;
    *ppt = ptSecond;
    plistLinkPoint->AddTail(ppt); // 需要把ptSecond加进去。
  }
  ppt1 = plistLinkPoint->GetTail();
  if ((ptFirst.x == ptCurrent.x) || (ptFirst.y == ptCurrent.y)) { // 生成直线
    if (ppt1->x < rectSecond.left) ppt1->x = rectSecond.left;
    else if (ppt1->x > rectSecond.right) ppt1->x = rectSecond.right;
    else if (ppt1->y < rectSecond.top) ppt1->y = rectSecond.top;
    else if (ppt1->y > rectSecond.bottom) ppt1->y = rectSecond.bottom;
  }
  else { // 生成折线
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

  if (plistLinkPoint->GetCount() == 2) { // 只有两个点,则加上两个中点（这样当移动单元时，不会出现斜线）
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
// 当单元或者单元动态链接的单元位置发生变化时，计算动态链接线的位置。
// 
// 参数ptFirst、ptSecond需要修改。
//
// 这个是第一次创建时的情况，需要判断是创建一条直线还是一条折线。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AdjustDynLinkPoint(CRect rectFirstUnit, CPoint &ptFirst, CPoint &ptSecond, CPoint ptCurrent) {
  if ((ptCurrent.x >= rectFirstUnit.left) && (ptCurrent.x < rectFirstUnit.right)) { // 位于正上或者正下
    if (ptCurrent.y < rectFirstUnit.top) { // 位于正上
      ptFirst.y = rectFirstUnit.top;
      ptSecond.y = ptCurrent.y;
      ptFirst.x = ptSecond.x = ptCurrent.x;
    }
    else { // 位于正下
      ptFirst.y = rectFirstUnit.bottom;
      ptSecond.y = ptCurrent.y;
      ptFirst.x = ptSecond.x = ptCurrent.x;
    }
  }
  else if ((ptCurrent.y >= rectFirstUnit.top) && (ptCurrent.y < rectFirstUnit.bottom)) { // 位于正左正右
    if (ptCurrent.x < rectFirstUnit.left) { // 位于正左
      ptFirst.x = rectFirstUnit.left;
      ptSecond.x = ptCurrent.x;
      ptFirst.y = ptSecond.y = ptCurrent.y;
    }
    else { // 位于正右
      ptFirst.x = rectFirstUnit.right;
      ptSecond.x = ptCurrent.x;
      ptFirst.y = ptSecond.y = ptCurrent.y;
    }
  }
  else { // 位于四角
    if ( ptFirst.x == ptSecond.x ) { // 先上下后左右
      ptSecond.y = ptCurrent.y;
      if (ptCurrent.x < rectFirstUnit.left) { // 位于左边
        ptFirst.x = ptSecond.x = rectFirstUnit.left;
        //ptFirst.y = rectFirstUnit.bottom;
      }
      else { // 位于右边
        ptFirst.x = ptSecond.x = rectFirstUnit.right;
      }
    }
    else if (ptFirst.y == ptSecond.y) { // 先左右后上下
      ptSecond.x = ptCurrent.x;
      if (ptCurrent.y < rectFirstUnit.top) { // 位于上边
        ptFirst.y = ptSecond.y = rectFirstUnit.top;
      }
      else { // 位于下边
        ptFirst.y = ptSecond.y = rectFirstUnit.bottom;
      }
    }
  }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当单元或者单元动态链接的单元位置发生变化时，计算动态链接线的位置。
// 
// 参数ptFirst、ptSecond需要修改。
//
// 这个是第二次创建及随后时的情况，只是创建一条直线
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AdjustDynLinkPoint(CPoint &ptFirst, CPoint &ptSecond, CPoint ptCurrent) {
  if (ptSecond.x == ptFirst.x) ptSecond.y = ptCurrent.y;
  else ptSecond.x = ptCurrent.x;
}