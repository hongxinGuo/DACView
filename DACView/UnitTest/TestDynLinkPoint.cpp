#include"stdafx.h"

#include"pch.h"

#include"DynLinkPoint.h"

// 测试生成动态链接线的函数
namespace DACViewTest {
  struct DLPointFindDest {
    DLPointFindDest(int rl1, int rt1, int rr1, int rb1, int pt3x, int pt3y, int pt1x, int pt1y, int pt2x, int pt2y) {
      rectSecond.left = rl1;
      rectSecond.top = rt1;
      rectSecond.right = rr1;
      rectSecond.bottom = rb1;
      ptSecond.x = pt1x;
      ptSecond.y = pt1y;
      ptFirst.x = pt2x;
      ptFirst.y = pt2y;
      ptCurrent.x = pt3x;
      ptCurrent.y = pt3y;
    }
    CRect rectSecond;
    CPoint ptFirst;
    CPoint ptSecond;
    CPoint ptCurrent;
  };
  DLPointFindDest sFindDest1(200, 210, 300, 320, 250, 250, 250, 150, 150, 150);  //左上，横向
  DLPointFindDest sFindDest9(200, 210, 300, 320, 250, 250, 150, 250, 150, 150);  //左上，纵向
  DLPointFindDest sFindDest2(200, 210, 300, 320, 250, 250, 250, 150, 150, 150);  //正上
  DLPointFindDest sFindDest3(200, 210, 300, 320, 250, 250, 250, 150, 400, 150);  //右上，横向
  DLPointFindDest sFindDest10(200, 210, 300, 320, 250, 250, 150, 250, 400, 150);  //右上，纵向
  DLPointFindDest sFindDest4(200, 210, 300, 320, 250, 250, 150, 250, 150, 250);  //左边
  DLPointFindDest sFindDest5(200, 210, 300, 320, 250, 250, 400, 250, 400, 250);  //右边
  DLPointFindDest sFindDest6(200, 210, 300, 320, 250, 250, 250, 400, 100, 400);  //左下 ，横向
  DLPointFindDest sFindDest11(200, 210, 300, 320, 250, 250, 100, 250, 100, 400);  //左下，纵向
  DLPointFindDest sFindDest7(200, 210, 300, 320, 250, 250, 250, 400, 250, 400);  //下边
  DLPointFindDest sFindDest8(200, 210, 300, 320, 250, 250, 250, 400, 400, 400);  //右下，横向
  DLPointFindDest sFindDest12(200, 210, 300, 320, 250, 250, 400, 250, 400, 400);  //右下，纵向

  class TestDLPointFindDest : public::testing::TestWithParam<DLPointFindDest*>
  {
  };

  INSTANTIATE_TEST_SUITE_P(TestCreateDLPoint, TestDLPointFindDest, testing::Values(&sFindDest1, &sFindDest2, &sFindDest3,
                                                                                   &sFindDest4, &sFindDest5, &sFindDest6, &sFindDest7, &sFindDest8, &sFindDest9, &sFindDest10, &sFindDest11, &sFindDest12));

  TEST_P(TestDLPointFindDest, TestCreateDynLinkPointFindDest1) { // 无起点有终点
    DLPointFindDest* pFindDest = GetParam();

    CRect rectSecond = pFindDest->rectSecond;
    CPoint ptSecond = pFindDest->ptSecond;
    CPoint ptFirst = pFindDest->ptFirst;
    CPoint ptCurrent = pFindDest->ptCurrent;
    CPointListPtr plistPoint = make_shared<CPointList>();
    shared_ptr<CPoint> ppt, ppt1, ppt2, ppt3;

    ppt = make_shared<CPoint>();
    *ppt = ptFirst;
    plistPoint->push_back(ppt);
    ppt = make_shared<CPoint>();
    *ppt = ptFirst;
    plistPoint->push_back(ppt);
    ppt = make_shared<CPoint>();
    *ppt = ptSecond;
    plistPoint->push_back(ppt);

    CreateDynLinkPoint(plistPoint, rectSecond, ptFirst, ptSecond, ptCurrent);

    EXPECT_LE(3, plistPoint->size()) << "无起点有终点的动态链接线至少有三个点";
    auto it = plistPoint->end();
    it--;
    ppt3 = *it--;
    ppt2 = *it--;
    ppt1 = *it;
    if (ptSecond.y < rectSecond.top) {
      EXPECT_EQ(ppt3->x, ppt2->x);
      EXPECT_EQ(ppt3->y, rectSecond.top);
    }
    else if (ptSecond.y >= rectSecond.bottom) {
      EXPECT_EQ(ppt3->x, ppt2->x);
      EXPECT_EQ(ppt3->y, rectSecond.bottom);
    }
    else if (ptSecond.x < rectSecond.left) {
      EXPECT_EQ(ppt3->y, ppt2->y);
      EXPECT_EQ(ppt3->x, rectSecond.left);
    }
    else {
      EXPECT_EQ(ppt3->y, ppt2->y);
      EXPECT_EQ(ppt3->x, rectSecond.right);
    }

    plistPoint->clear();
  }

  TEST_P(TestDLPointFindDest, TestCreateDynLinkPointFindDest2) { // 有起点有终点
    DLPointFindDest* pFindDest = GetParam();

    CRect rectSecond = pFindDest->rectSecond;
    CPoint ptSecond = pFindDest->ptSecond;
    CPoint ptFirst = pFindDest->ptFirst;
    CPoint ptCurrent = pFindDest->ptCurrent;
    CPointListPtr plistPoint = make_shared<CPointList>();
    shared_ptr<CPoint> ppt, ppt1, ppt2, ppt3, ppt4;

    ppt = make_shared<CPoint>();
    *ppt = ptFirst;
    plistPoint->push_back(ppt);

    CreateDynLinkPoint(plistPoint, rectSecond, ptFirst, ptSecond, ptCurrent);

    EXPECT_LE(plistPoint->size(), 4); // 有起点有终点的动态链接线最多四个点
    auto it = plistPoint->end();
    ppt3 = *--it;
    ppt2 = *--it;
    ppt1 = *--it;
    if ((ptFirst.y >= rectSecond.top) && (ptFirst.y < rectSecond.bottom)) {
      ppt4 = *--it;
      if (ptFirst.x < rectSecond.left) {
        EXPECT_EQ(4, plistPoint->size());
        EXPECT_EQ(ppt3->y, ppt4->y);
        EXPECT_EQ(ppt3->y, ppt2->y);
        EXPECT_EQ(ppt3->x, rectSecond.left);
      }
      else {
        EXPECT_EQ(4, plistPoint->size());
        EXPECT_EQ(ppt3->y, ppt4->y);
        EXPECT_EQ(ppt3->y, ppt2->y);
        EXPECT_EQ(ppt3->x, rectSecond.right);
      }
    }
    else if ((ptFirst.x >= rectSecond.left) && (ptFirst.x < rectSecond.right)) {
      ppt4 = *--it;
      if (ptFirst.y < rectSecond.top) {
        EXPECT_EQ(4, plistPoint->size());
        EXPECT_EQ(ppt3->x, ppt4->x);
        EXPECT_EQ(ppt3->x, ppt2->x);
        EXPECT_EQ(ppt3->y, rectSecond.top);
      }
      else {
        EXPECT_EQ(4, plistPoint->size());
        EXPECT_EQ(ppt3->x, ppt4->x);
        EXPECT_EQ(ppt3->x, ppt2->x);
        EXPECT_EQ(ppt3->y, rectSecond.bottom);
      }
    }
    else { // 三个点
      EXPECT_EQ(3, plistPoint->size());
      if (ptSecond.y >= rectSecond.bottom) {
        EXPECT_EQ(ppt3->x, ppt2->x);
        EXPECT_EQ(ppt3->y, rectSecond.bottom);
      }
      else if (ptSecond.x < rectSecond.left) {
        EXPECT_EQ(ppt3->y, ppt2->y);
        EXPECT_EQ(ppt3->x, rectSecond.left);
      }
      else if (ptSecond.y < rectSecond.top) {
        EXPECT_EQ(ppt3->x, ppt2->x);
        EXPECT_EQ(ppt3->y, rectSecond.top);
      }
      else if (ptSecond.x >= rectSecond.right) {
        EXPECT_EQ(ppt3->y, ppt2->y);
        EXPECT_EQ(ppt3->x, rectSecond.right);
      }
    }
    plistPoint->clear();
  }

  struct DLPointNotFindDest {
    DLPointNotFindDest(int pt1x, int pt1y, int pt2x, int pt2y, int pt3x, int pt3y) {
      ptSecond.x = pt2x;
      ptSecond.y = pt2y;
      ptFirst.x = pt1x;
      ptFirst.y = pt1y;
      ptCurrent.x = pt3x;
      ptCurrent.y = pt3y;
    }
    CPoint ptCurrent;
    CPoint ptFirst;
    CPoint ptSecond;
  };
  DLPointNotFindDest sNotFindDest1(300, 300, 300, 450, 450, 450);  //右下，纵向
  DLPointNotFindDest sNotFindDest9(300, 300, 450, 300, 450, 450);  //右下，横向
  DLPointNotFindDest sNotFindDest2(300, 300, 300, 450, 300, 450);  //正下
  DLPointNotFindDest sNotFindDest3(300, 300, 300, 450, 150, 450);  //左下，纵向
  DLPointNotFindDest sNotFindDest10(300, 300, 150, 300, 150, 200);  //左下，横向
  DLPointNotFindDest sNotFindDest4(300, 300, 150, 300, 150, 300);  //左边
  DLPointNotFindDest sNotFindDest5(300, 300, 400, 300, 400, 300);  //右边
  DLPointNotFindDest sNotFindDest6(300, 300, 150, 300, 150, 200);  //左上 ，横向
  DLPointNotFindDest sNotFindDest11(300, 300, 300, 150, 100, 150);  //左上，纵向
  DLPointNotFindDest sNotFindDest7(300, 300, 300, 200, 300, 200);  //上边
  DLPointNotFindDest sNotFindDest8(300, 300, 550, 300, 550, 200);  //右上，横向
  DLPointNotFindDest sNotFindDest12(300, 300, 300, 50, 400, 50);  //右上，纵向

  class TestDLPointNotFindDest : public::testing::TestWithParam<DLPointNotFindDest*>
  {
  };

  INSTANTIATE_TEST_SUITE_P(TestCreateDLPointNotFind, TestDLPointNotFindDest, testing::Values(&sNotFindDest1, &sNotFindDest2,
                                                                                             &sNotFindDest3, &sNotFindDest4, &sNotFindDest5, &sNotFindDest6, &sNotFindDest7, &sNotFindDest8, &sNotFindDest9,
                                                                                             &sNotFindDest10, &sNotFindDest11, &sNotFindDest12));

  TEST_P(TestDLPointNotFindDest, TestCreateDynLinkPointNotFindDest1) { // 有起点无终点
    DLPointNotFindDest* pFindDest = GetParam();

    CPoint ptSecond1, ptSecond = pFindDest->ptSecond;
    CPoint ptFirst1, ptFirst = pFindDest->ptFirst;
    CPoint ptCurrent = pFindDest->ptCurrent;
    CPointListPtr plistPoint = make_shared<CPointList>();
    shared_ptr<CPoint> ppt, ppt1, ppt2, ppt3;

    ptSecond1 = ptSecond;
    ptFirst1 = ptFirst;

    ppt = make_shared<CPoint>();
    *ppt = ptFirst;
    plistPoint->push_back(ppt);

    if ((ptSecond.x == ptCurrent.x) && (ptSecond.y == ptCurrent.y)) { // 直线
      if (ptSecond.x == ptFirst.x) { // 纵向
        ppt->y += 3; // 故意移动一下，测试能否移至ptFirst.y
      }
      else {
        ppt->x += 3; // 故意移动一下，测试能否移至ptFirst.x
      }
    }
    CreateDynLinkPoint(plistPoint, &ptFirst1, &ptSecond1, ptCurrent);

    auto it = plistPoint->end();
    if ((ptSecond.x == ptCurrent.x) && (ptSecond.y == ptCurrent.y)) { // 直线
      EXPECT_EQ(2, plistPoint->size());
      ppt1 = *--it;
      EXPECT_EQ(ppt1->x, ptSecond.x);
      EXPECT_EQ(ppt1->y, ptSecond.y);
      EXPECT_EQ(ppt->x, ptFirst.x); // 测试是否移至ptFirst
      EXPECT_EQ(ppt->y, ptFirst.y);
    }
    else {
      EXPECT_EQ(3, plistPoint->size());
      it = plistPoint->begin();
      ppt = *it++;
      ppt2 = *it++;
      ppt3 = *it++;
      EXPECT_EQ(ppt2->x, ptFirst1.x); // 此时ptFirst1已经移至ptSecond
      EXPECT_EQ(ppt2->y, ptFirst1.y);
      EXPECT_EQ(ppt3->x, ptSecond1.x); // 此时ptSecond1已经移至当前点
      EXPECT_EQ(ppt3->y, ptSecond1.y);
      EXPECT_EQ(ptFirst1.x, ptSecond.x);
      EXPECT_EQ(ptFirst1.y, ptSecond.y);
    }
    plistPoint->clear();
  }

  TEST_P(TestDLPointNotFindDest, TestCreateDynLinkPointNotFindDest2) { // 无起点无终点
    DLPointNotFindDest* pFindDest = GetParam();

    CPoint ptSecond = pFindDest->ptSecond;
    CPoint ptSecond1, ptFirst = pFindDest->ptFirst;
    CPoint ptCurrent = pFindDest->ptCurrent;
    CPointListPtr plistPoint = make_shared<CPointList>();
    shared_ptr<CPoint> ppt;

    ptSecond1 = ptSecond;

    ppt = make_shared<CPoint>();
    *ppt = ptFirst;
    plistPoint->push_back(ppt);
    ppt = make_shared<CPoint>();
    *ppt = ptSecond;
    plistPoint->push_back(ppt);

    CreateDynLinkPoint(plistPoint, &ptFirst, &ptSecond, ptCurrent);

    ppt = plistPoint->back();
    EXPECT_EQ(ppt->x, ptSecond.x); // 此时ptFirst已经移至ptSecond
    EXPECT_EQ(ppt->y, ptSecond.y);
    EXPECT_EQ(ptSecond1.x, ptFirst.x);
    EXPECT_EQ(ptSecond1.y, ptFirst.y);

    plistPoint->clear();
  }
}

namespace DACViewTest {
  struct AdjustDLPoint {
    AdjustDLPoint(int rl1, int rt1, int rr1, int rb1, int pt1x, int pt1y, int pt2x, int pt2y, int pt3x, int pt3y) {
      rectFirst.left = rl1;
      rectFirst.top = rt1;
      rectFirst.right = rr1;
      rectFirst.bottom = rb1;
      ptSecond.x = pt1x;
      ptSecond.y = pt1y;
      ptFirst.x = pt2x;
      ptFirst.y = pt2y;
      ptCurrent.x = pt3x;
      ptCurrent.y = pt3y;
    }
    CRect rectFirst;
    CPoint ptFirst;
    CPoint ptSecond;
    CPoint ptCurrent;
  };
  AdjustDLPoint sAdjustDLPoint1(200, 210, 300, 320, 230, 250, 150, 250, 100, 100);  //上边，横向
  AdjustDLPoint sAdjustDLPoint2(200, 210, 300, 320, 230, 210, 230, 150, 100, 100);  //上边，纵向
  AdjustDLPoint sAdjustDLPoint3(200, 210, 300, 320, 250, 150, 150, 150, 270, 100);  //正上
  AdjustDLPoint sAdjustDLPoint4(200, 210, 300, 320, 150, 250, 150, 250, 50, 240);  //左边
  AdjustDLPoint sAdjustDLPoint5(200, 210, 300, 320, 400, 250, 400, 250, 500, 260);  //右边
  AdjustDLPoint sAdjustDLPoint6(200, 210, 300, 320, 250, 400, 250, 400, 230, 500);  //下边
  AdjustDLPoint sAdjustDLPoint7(200, 210, 300, 320, 200, 270, 400, 270, 100, 500);  //下边，横向
  AdjustDLPoint sAdjustDLPoint8(200, 210, 300, 320, 220, 250, 220, 400, 500, 500);  //下边，纵向

  class TestAdjustDLPoint : public::testing::TestWithParam<AdjustDLPoint*>
  {
  };

  INSTANTIATE_TEST_SUITE_P(TestAdjustDLPoint, TestAdjustDLPoint, testing::Values(&sAdjustDLPoint1, &sAdjustDLPoint2,
                                                                                 &sAdjustDLPoint3, &sAdjustDLPoint4, &sAdjustDLPoint5, &sAdjustDLPoint6, &sAdjustDLPoint7, &sAdjustDLPoint8));

  TEST_P(TestAdjustDLPoint, TestAdjustDynLinkPoint11) {
    AdjustDLPoint* pAdjust = GetParam();

    CRect rectFirst = pAdjust->rectFirst;
    CPoint ptSecond = pAdjust->ptSecond;
    CPoint ptFirst1, ptSecond1, ptFirst = pAdjust->ptFirst;
    CPoint ptCurrent = pAdjust->ptCurrent;

    ptSecond1 = ptSecond;
    ptFirst1 = ptFirst;

    AdjustDynLinkPoint(rectFirst, ptFirst, ptSecond, ptCurrent);

    if ((ptCurrent.x >= rectFirst.left) && (ptCurrent.x < rectFirst.right)) { // 位于正上或者正下
      if (ptCurrent.y < rectFirst.top) { // 位于正上
        EXPECT_EQ(ptFirst.y, rectFirst.top);
        EXPECT_EQ(ptSecond.y, ptCurrent.y);
        EXPECT_EQ(ptFirst.x, ptSecond.x);
        EXPECT_EQ(ptSecond.x, ptCurrent.x);
      }
      else { // 位于正下
        EXPECT_EQ(ptFirst.y, rectFirst.bottom);
        EXPECT_EQ(ptSecond.y, ptCurrent.y);
        EXPECT_EQ(ptFirst.x, ptSecond.x);
        EXPECT_EQ(ptSecond.x, ptCurrent.x);
      }
    }
    else if ((ptCurrent.y >= rectFirst.top) && (ptCurrent.y < rectFirst.bottom)) { // 位于正左正右
      if (ptCurrent.x < rectFirst.left) { // 位于正左
        EXPECT_EQ(ptFirst.x, rectFirst.left);
        EXPECT_EQ(ptSecond.x, ptCurrent.x);
        EXPECT_EQ(ptFirst.y, ptSecond.y);
        EXPECT_EQ(ptSecond.y, ptCurrent.y);
      }
      else { // 位于正右
        EXPECT_EQ(ptFirst.x, rectFirst.right);
        EXPECT_EQ(ptSecond.x, ptCurrent.x);
        EXPECT_EQ(ptFirst.y, ptSecond.y);
        EXPECT_EQ(ptSecond.y, ptCurrent.y);
      }
    }
    else { // 位于四角
      if (ptFirst.x == ptSecond.x) { // 先上下后左右
        EXPECT_EQ(ptSecond.y, ptCurrent.y);
        if (ptCurrent.x < rectFirst.left) { // 位于左边
          EXPECT_EQ(ptFirst.x, ptSecond.x);
          EXPECT_EQ(ptSecond.x, rectFirst.left);
        }
        else { // 位于右边
          EXPECT_EQ(ptFirst.x, ptSecond.x);
          EXPECT_EQ(ptSecond.x, rectFirst.right);
        }
      }
      else if (ptFirst.y == ptSecond.y) { // 先左右后上下
        EXPECT_EQ(ptCurrent.x, ptSecond.x);
        if (ptCurrent.y < rectFirst.top) { // 位于上边
          EXPECT_EQ(ptFirst.y, ptSecond.y);
          EXPECT_EQ(ptSecond.y, rectFirst.top);
        }
        else { // 位于下边
          EXPECT_EQ(ptFirst.y, ptSecond.y);
          EXPECT_EQ(ptSecond.y, rectFirst.bottom);
        }
      }
    }
  }
}