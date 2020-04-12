/////////////////////////////////////////////////////////////////////////////////
//
// 此文件中的测试函数，是测试鼠标左键动作的。每个测试函数，从鼠标左键按下开始，皆完成一个连续动作后方结束。
// 如建立简单单元之间的动态链接，其动作顺序是：LButtonDown->选择第一个单元的参数->LButtonUp->[动态链接线的一系列down->move->up]->选择第二个单元参数。
//
// 前期预定：
// 测试文件为：可封装和不可封装部件嵌套四层参数有链接有封装后的多个部件.sqi
// 测试单元为：UnitOr_4（坐标218， 249）
// 第二测试单元为：UnitAnd_7(414, 266)
//
/////////////////////////////////////////////////////////////////////////////////
#include"stdafx.h"

using namespace std;
#include<memory>

#include "gtest/gtest.h"

#include"CSQIUnitViewTest.h"

#include"accesory.h"

namespace DACViewTest {
  class TestSQIUnitViewMouseLButton : public testing::Test {
  protected:
    void SetUp() override {
      strFileName = "c:\\DACViewTestFile\\SQIUnitView\\可封装和不可封装部件嵌套四层参数有链接有封装后的多个部件.sqi";
      CFile cFile;
      char buffer[512];

      CString strFileHeader;
      INT32 iObjectNumber;
      INT64 iUnitNumber;

      // 测试初始化时的变量初始值
      EXPECT_EQ(m_View.GetCurrentUnitList(), nullptr);
      EXPECT_EQ(m_View.GetTopUnitList(), nullptr);
      EXPECT_EQ(m_View.GetDoc(), nullptr);
      EXPECT_EQ(m_View.GetCurrentFunction(), UNIT_PRE_SELECT);
      EXPECT_EQ(m_View.GetCurrentUnit(), nullptr);
      EXPECT_EQ(m_View.GetFirstUnit(), nullptr);
      EXPECT_EQ(m_View.GetSecondUnit(), nullptr);
      EXPECT_EQ(m_View.GetCurrentDynLink(), nullptr);
      EXPECT_FALSE(m_View.IsLinkIntoDestComponent());
      EXPECT_FALSE(m_View.IsLinkIntoSrcComponnet());
      EXPECT_EQ(m_View.GetSrcIndex(), -1);
      EXPECT_EQ(m_View.GetDestIndex(), -1);

      if (!cFile.Open(strFileName, CFile::modeRead)) {
        EXPECT_FALSE(true) << "read file %s failed" << strFileName;
      }
      CArchive ar(&cFile, CArchive::load, 512, buffer);
      LoadSQIFile(ar, &m_unitlist, &m_objectlist, &iUnitNumber, &iObjectNumber);

      m_View.SetTopUnitList(&m_unitlist);
      m_View.SetCurrentUnitList(&m_unitlist);
    }

    void TearDown() override {
      ReleaseSQIFile(&m_unitlist, &m_objectlist);
      EXPECT_TRUE(m_unitlist.empty());
      EXPECT_TRUE(m_objectlist.empty());
    }

    CString strFileName;
    CUnitList m_unitlist;
    CObjectList m_objectlist;

    CSQIUnitViewTest m_View;
  };

  // UNIT_PRE_SELECT 初始没有选择单元。点击到空，也没有选择单元
  TEST_F(TestSQIUnitViewMouseLButton, LButtonTestPreSelect1) {
    CPoint pt;

    EXPECT_EQ(m_View.GetCurrentFunction(), UNIT_PRE_SELECT);
    EXPECT_EQ(m_View.GetCurrentUnit(), nullptr);
    EXPECT_EQ(m_View.GetFirstUnit(), nullptr);
    EXPECT_EQ(m_View.GetSecondUnit(), nullptr);
    EXPECT_TRUE(m_View.GetCurrentUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetFirstUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetSecondUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetLinkPointList()->empty());
    EXPECT_EQ(m_View.GetDynLinkType(), 0);
    EXPECT_EQ(m_View.GetDynLinkClass(), 0);
    EXPECT_EQ(-1, m_View.GetSrcIndex());
    EXPECT_EQ(-1, m_View.GetDestIndex());

    m_View.SetCurrentFunction(UNIT_PRE_SELECT);
    m_View.m_iHitTest = -1; // 没有选中焦点区域
    m_View.m_ptPoint.x = m_View.m_ptPoint.y = 0; // 位置为0，

    // 执行mouse down
    m_View.OnLButtonDown(0, m_View.m_ptPoint);

    EXPECT_EQ(m_View.GetCurrentFunction(), UNIT_PRE_SELECT);
    EXPECT_EQ(m_View.GetCurrentUnit(), nullptr);
    EXPECT_EQ(m_View.GetFirstUnit(), nullptr);
    EXPECT_EQ(m_View.GetSecondUnit(), nullptr);
    EXPECT_TRUE(m_View.GetCurrentUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetFirstUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetSecondUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetLinkPointList()->empty());
    EXPECT_EQ(m_View.GetDynLinkClass(), 0);
    EXPECT_EQ(m_View.GetDynLinkType(), 0);
    EXPECT_EQ(-1, m_View.GetSrcIndex());
    EXPECT_EQ(-1, m_View.GetDestIndex());
    pt = m_View.GetFirstPt();
    EXPECT_EQ(pt.x, 0);
    EXPECT_EQ(pt.y, 0);
    pt = m_View.GetSecondPt();
    EXPECT_EQ(pt.x, 0);
    EXPECT_EQ(pt.y, 0);

    // 执行mouse move
    m_View.OnMouseMove(0, m_View.m_ptPoint);

    EXPECT_EQ(m_View.GetCurrentUnit(), nullptr);
    EXPECT_EQ(m_View.GetUnitMouseMove(), nullptr);

    CPointListPtr plistPoint = m_View.GetLinkPointList();
    shared_ptr<CPoint> pt1 = make_shared<CPoint>(0, 0);
    plistPoint->push_back(pt1); // 故意加入一个元素
    EXPECT_EQ(plistPoint->size(), 1);

    // 执行LButtonUp
    m_View.OnLButtonUp(0, m_View.m_ptPoint);

    EXPECT_EQ(plistPoint->size(), 0);
    EXPECT_TRUE(m_View.GetLinkPointList()->empty()) << "此时动态点序列为空";

    // 动作结束
  }

  // UNIT_PRE_SELECT 初始没有选择单元。点击到UnitOr_4单元处，选择了单元中间，鼠标没有移动。
  TEST_F(TestSQIUnitViewMouseLButton, LButtonTestPreSelect2) {
    CPoint pt;

    EXPECT_EQ(m_View.GetCurrentFunction(), UNIT_PRE_SELECT);
    EXPECT_EQ(m_View.GetCurrentUnit(), nullptr);
    EXPECT_EQ(m_View.GetFirstUnit(), nullptr);
    EXPECT_EQ(m_View.GetSecondUnit(), nullptr);
    EXPECT_TRUE(m_View.GetCurrentUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetFirstUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetSecondUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetLinkPointList()->empty());
    EXPECT_EQ(m_View.GetDynLinkType(), 0);
    EXPECT_EQ(m_View.GetDynLinkClass(), 0);
    EXPECT_EQ(-1, m_View.GetSrcIndex());
    EXPECT_EQ(-1, m_View.GetDestIndex());

    m_View.SetCurrentFunction(UNIT_PRE_SELECT);
    m_View.m_fMouseTrack = true; // 鼠标框选状态
    m_View.m_iHitTest = 8; // 选中了中间

    //这个位置是UnitOr_4的坐标
    m_View.m_ptPoint.x = 218;
    m_View.m_ptPoint.y = 248;

    // 执行mouse down
    m_View.OnLButtonDown(0, m_View.m_ptPoint);

    EXPECT_EQ(m_View.GetCurrentFunction(), UNIT_SELECTED);
    EXPECT_STREQ(m_View.GetCurrentUnit()->GetName(), _T("UnitOr_4"));
    EXPECT_TRUE(m_View.m_fDocModifiedFlag);
    EXPECT_EQ(m_View.GetFirstUnit(), nullptr);
    EXPECT_EQ(m_View.GetSecondUnit(), nullptr);
    EXPECT_FALSE(m_View.GetCurrentUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetFirstUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetSecondUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetLinkPointList()->size() == 0);
    EXPECT_EQ(m_View.GetDynLinkClass(), 0);
    EXPECT_EQ(m_View.GetDynLinkType(), 0);
    EXPECT_EQ(-1, m_View.GetSrcIndex());
    EXPECT_EQ(-1, m_View.GetDestIndex());
    pt = m_View.GetFirstPt();
    EXPECT_EQ(pt.x, 0);
    EXPECT_EQ(pt.y, 0);
    pt = m_View.GetSecondPt();
    EXPECT_EQ(pt.x, 0);
    EXPECT_EQ(pt.y, 0);

    // 执行mouse move
    m_View.OnMouseMove(0, m_View.m_ptPoint);

    EXPECT_STREQ(m_View.GetCurrentUnit()->GetName(), _T("UnitOr_4"));
    EXPECT_STREQ(m_View.GetUnitMouseMove()->GetName(), _T("UnitOr_4"));

    CPointListPtr plistPoint = m_View.GetLinkPointList();
    shared_ptr<CPoint> pt1 = make_shared<CPoint>(0, 0);
    plistPoint->push_back(pt1); // 故意加入一个元素
    EXPECT_EQ(plistPoint->size(), 1);

    // 执行LButtonUp
    m_View.OnLButtonUp(0, m_View.m_ptPoint);

    EXPECT_EQ(plistPoint->size(), 1) << "此种状态时没有重置";
    EXPECT_TRUE(!m_View.GetLinkPointList()->empty()) << "此时动态点序列不为空";
  }

  // UNIT_SELECTED.初始时选择单元。点击到空处，没有选择单元
  TEST_F(TestSQIUnitViewMouseLButton, LButtonTestPreSelect10) {
    CPoint pt;

    EXPECT_EQ(m_View.GetCurrentFunction(), UNIT_PRE_SELECT);
    EXPECT_EQ(m_View.GetCurrentUnit(), nullptr);
    EXPECT_EQ(m_View.GetFirstUnit(), nullptr);
    EXPECT_EQ(m_View.GetSecondUnit(), nullptr);
    EXPECT_TRUE(m_View.GetCurrentUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetFirstUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetSecondUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetLinkPointList()->empty());
    EXPECT_EQ(m_View.GetDynLinkClass(), 0);
    EXPECT_EQ(m_View.GetDynLinkType(), 0);
    EXPECT_EQ(-1, m_View.GetSrcIndex());
    EXPECT_EQ(-1, m_View.GetDestIndex());

    m_View.SetCurrentFunction(UNIT_SELECTED);
    bool fFind = false;
    CUnitBase* pUnit = FindUnit(m_View.GetCurrentUnitList(), "UnitOr_4", fFind); // 数据文件中必须有此单元名称
    EXPECT_TRUE(pUnit != nullptr);
    EXPECT_STREQ(pUnit->GetName(), _T("UnitOr_4")); // 当前选择单元为pUnit

    m_View.SetCurrentUnit(pUnit);
    m_View.m_iHitTest = -1;  // 没有选中单元
    m_View.m_ptPoint.x = m_View.m_ptPoint.y = 0;

    //执行mouse LButton down
    m_View.OnLButtonDown(0, m_View.m_ptPoint);

    EXPECT_EQ(m_View.GetCurrentFunction(), UNIT_PRE_SELECT);
    EXPECT_EQ(m_View.GetCurrentUnit(), nullptr);
    EXPECT_EQ(m_View.GetFirstUnit(), nullptr);
    EXPECT_EQ(m_View.GetSecondUnit(), nullptr);
    EXPECT_TRUE(m_View.GetCurrentUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetFirstUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetSecondUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetLinkPointList()->empty());
    EXPECT_EQ(m_View.GetDynLinkClass(), 0);
    EXPECT_EQ(m_View.GetDynLinkType(), 0);
    EXPECT_EQ(-1, m_View.GetSrcIndex());
    EXPECT_EQ(-1, m_View.GetDestIndex());
    pt = m_View.GetFirstPt();
    EXPECT_EQ(pt.x, 0);
    EXPECT_EQ(pt.y, 0);
    pt = m_View.GetSecondPt();
    EXPECT_EQ(pt.x, 0);
    EXPECT_EQ(pt.y, 0);

    // 执行mouse move
    m_View.OnMouseMove(0, m_View.m_ptPoint);

    EXPECT_EQ(m_View.GetCurrentUnit(), nullptr);
    EXPECT_EQ(m_View.GetUnitMouseMove(), nullptr);

    CPointListPtr plistPoint = m_View.GetLinkPointList();
    shared_ptr<CPoint> pt1 = make_shared<CPoint>(0, 0);
    plistPoint->push_back(pt1); // 故意加入一个元素
    EXPECT_EQ(plistPoint->size(), 1);
    // 执行LButtonUp
    m_View.OnLButtonUp(0, m_View.m_ptPoint);

    EXPECT_EQ(plistPoint->size(), 0);
    EXPECT_TRUE(m_View.GetLinkPointList()->empty()); // 此时动态点序列为空
    // 动作结束
  }

  // 测试选中单元后成功进行动态链接.单元（UnitOr_4,位置218，249)与单元(UnitAnd_7,位置414，266)之间，
  // 中间进行一次动态链接线的选定（位置418,240)
  TEST_F(TestSQIUnitViewMouseLButton, LButtonTestUnitDynLinkToUnit) {
    CPoint pt;

    EXPECT_EQ(m_View.GetCurrentFunction(), UNIT_PRE_SELECT);
    EXPECT_EQ(m_View.GetCurrentUnit(), nullptr);
    EXPECT_EQ(m_View.GetFirstUnit(), nullptr);
    EXPECT_EQ(m_View.GetSecondUnit(), nullptr);
    EXPECT_TRUE(m_View.GetCurrentUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetFirstUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetSecondUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetLinkPointList()->empty());
    EXPECT_EQ(m_View.GetDynLinkClass(), 0);
    EXPECT_EQ(m_View.GetDynLinkType(), 0);
    EXPECT_EQ(-1, m_View.GetSrcIndex());
    EXPECT_EQ(-1, m_View.GetDestIndex());

    m_View.SetCurrentFunction(UNIT_SELECTED);
    bool fFind = false;
    CUnitBase* pUnit = FindUnit(m_View.GetCurrentUnitList(), "UnitOr_4", fFind); // 数据文件中必须有此单元名称
    EXPECT_TRUE(pUnit != nullptr);
    EXPECT_STREQ(pUnit->GetName(), _T("UnitOr_4")); // 当前选择单元为pUnit

    m_View.SetCurrentUnit(pUnit);
    m_View.m_iHitTest = -1;  // 没有选中单元
    m_View.m_ptPoint.x = m_View.m_ptPoint.y = 0;

    //执行mouse LButton down
    m_View.OnLButtonDown(0, m_View.m_ptPoint);

    EXPECT_EQ(m_View.GetCurrentFunction(), UNIT_PRE_SELECT);
    EXPECT_EQ(m_View.GetCurrentUnit(), nullptr);
    EXPECT_EQ(m_View.GetFirstUnit(), nullptr);
    EXPECT_EQ(m_View.GetSecondUnit(), nullptr);
    EXPECT_TRUE(m_View.GetCurrentUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetFirstUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetSecondUnitSize().IsRectEmpty());
    EXPECT_TRUE(m_View.GetLinkPointList()->empty());
    EXPECT_EQ(m_View.GetDynLinkClass(), 0);
    EXPECT_EQ(m_View.GetDynLinkType(), 0);
    EXPECT_EQ(-1, m_View.GetSrcIndex());
    EXPECT_EQ(-1, m_View.GetDestIndex());
    pt = m_View.GetFirstPt();
    EXPECT_EQ(pt.x, 0);
    EXPECT_EQ(pt.y, 0);
    pt = m_View.GetSecondPt();
    EXPECT_EQ(pt.x, 0);
    EXPECT_EQ(pt.y, 0);

    // 执行mouse move
    m_View.OnMouseMove(0, m_View.m_ptPoint);

    EXPECT_EQ(m_View.GetCurrentUnit(), nullptr);
    EXPECT_EQ(m_View.GetUnitMouseMove(), nullptr);

    CPointListPtr plistPoint = m_View.GetLinkPointList();
    shared_ptr<CPoint> pt1 = make_shared<CPoint>(0, 0);
    plistPoint->push_back(pt1); // 故意加入一个元素
    EXPECT_EQ(plistPoint->size(), 1);
    // 执行LButtonUp
    m_View.OnLButtonUp(0, m_View.m_ptPoint);

    EXPECT_EQ(plistPoint->size(), 0);
    EXPECT_TRUE(m_View.GetLinkPointList()->empty()); // 此时动态点序列为空
    // 动作结束
  }
}