/////////////////////////////////////////////////////////////////////////////////
//
// ���ļ��еĲ��Ժ������ǲ��������������ġ�ÿ�����Ժ����������������¿�ʼ�������һ�����������󷽽�����
// �罨���򵥵�Ԫ֮��Ķ�̬���ӣ��䶯��˳���ǣ�LButtonDown->ѡ���һ����Ԫ�Ĳ���->LButtonUp->[��̬�����ߵ�һϵ��down->move->up]->ѡ��ڶ�����Ԫ������
//
// ǰ��Ԥ����
// �����ļ�Ϊ���ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ�����������з�װ��Ķ������.sqi
// ���Ե�ԪΪ��UnitOr_4������218�� 249��
// �ڶ����Ե�ԪΪ��UnitAnd_7(414, 266)
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
      strFileName = "c:\\DACViewTestFile\\SQIUnitView\\�ɷ�װ�Ͳ��ɷ�װ����Ƕ���Ĳ�����������з�װ��Ķ������.sqi";
      CFile cFile;
      char buffer[512];

      CString strFileHeader;
      INT32 iObjectNumber;
      INT64 iUnitNumber;

      // ���Գ�ʼ��ʱ�ı�����ʼֵ
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

  // UNIT_PRE_SELECT ��ʼû��ѡ��Ԫ��������գ�Ҳû��ѡ��Ԫ
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
    m_View.m_iHitTest = -1; // û��ѡ�н�������
    m_View.m_ptPoint.x = m_View.m_ptPoint.y = 0; // λ��Ϊ0��

    // ִ��mouse down
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

    // ִ��mouse move
    m_View.OnMouseMove(0, m_View.m_ptPoint);

    EXPECT_EQ(m_View.GetCurrentUnit(), nullptr);
    EXPECT_EQ(m_View.GetUnitMouseMove(), nullptr);

    CPointListPtr plistPoint = m_View.GetLinkPointList();
    shared_ptr<CPoint> pt1 = make_shared<CPoint>(0, 0);
    plistPoint->push_back(pt1); // �������һ��Ԫ��
    EXPECT_EQ(plistPoint->size(), 1);

    // ִ��LButtonUp
    m_View.OnLButtonUp(0, m_View.m_ptPoint);

    EXPECT_EQ(plistPoint->size(), 0);
    EXPECT_TRUE(m_View.GetLinkPointList()->empty()) << "��ʱ��̬������Ϊ��";

    // ��������
  }

  // UNIT_PRE_SELECT ��ʼû��ѡ��Ԫ�������UnitOr_4��Ԫ����ѡ���˵�Ԫ�м䣬���û���ƶ���
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
    m_View.m_fMouseTrack = true; // ����ѡ״̬
    m_View.m_iHitTest = 8; // ѡ�����м�

    //���λ����UnitOr_4������
    m_View.m_ptPoint.x = 218;
    m_View.m_ptPoint.y = 248;

    // ִ��mouse down
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

    // ִ��mouse move
    m_View.OnMouseMove(0, m_View.m_ptPoint);

    EXPECT_STREQ(m_View.GetCurrentUnit()->GetName(), _T("UnitOr_4"));
    EXPECT_STREQ(m_View.GetUnitMouseMove()->GetName(), _T("UnitOr_4"));

    CPointListPtr plistPoint = m_View.GetLinkPointList();
    shared_ptr<CPoint> pt1 = make_shared<CPoint>(0, 0);
    plistPoint->push_back(pt1); // �������һ��Ԫ��
    EXPECT_EQ(plistPoint->size(), 1);

    // ִ��LButtonUp
    m_View.OnLButtonUp(0, m_View.m_ptPoint);

    EXPECT_EQ(plistPoint->size(), 1) << "����״̬ʱû������";
    EXPECT_TRUE(!m_View.GetLinkPointList()->empty()) << "��ʱ��̬�����в�Ϊ��";
  }

  // UNIT_SELECTED.��ʼʱѡ��Ԫ��������մ���û��ѡ��Ԫ
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
    CUnitBase* pUnit = FindUnit(m_View.GetCurrentUnitList(), "UnitOr_4", fFind); // �����ļ��б����д˵�Ԫ����
    EXPECT_TRUE(pUnit != nullptr);
    EXPECT_STREQ(pUnit->GetName(), _T("UnitOr_4")); // ��ǰѡ��ԪΪpUnit

    m_View.SetCurrentUnit(pUnit);
    m_View.m_iHitTest = -1;  // û��ѡ�е�Ԫ
    m_View.m_ptPoint.x = m_View.m_ptPoint.y = 0;

    //ִ��mouse LButton down
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

    // ִ��mouse move
    m_View.OnMouseMove(0, m_View.m_ptPoint);

    EXPECT_EQ(m_View.GetCurrentUnit(), nullptr);
    EXPECT_EQ(m_View.GetUnitMouseMove(), nullptr);

    CPointListPtr plistPoint = m_View.GetLinkPointList();
    shared_ptr<CPoint> pt1 = make_shared<CPoint>(0, 0);
    plistPoint->push_back(pt1); // �������һ��Ԫ��
    EXPECT_EQ(plistPoint->size(), 1);
    // ִ��LButtonUp
    m_View.OnLButtonUp(0, m_View.m_ptPoint);

    EXPECT_EQ(plistPoint->size(), 0);
    EXPECT_TRUE(m_View.GetLinkPointList()->empty()); // ��ʱ��̬������Ϊ��
    // ��������
  }

  // ����ѡ�е�Ԫ��ɹ����ж�̬����.��Ԫ��UnitOr_4,λ��218��249)�뵥Ԫ(UnitAnd_7,λ��414��266)֮�䣬
  // �м����һ�ζ�̬�����ߵ�ѡ����λ��418,240)
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
    CUnitBase* pUnit = FindUnit(m_View.GetCurrentUnitList(), "UnitOr_4", fFind); // �����ļ��б����д˵�Ԫ����
    EXPECT_TRUE(pUnit != nullptr);
    EXPECT_STREQ(pUnit->GetName(), _T("UnitOr_4")); // ��ǰѡ��ԪΪpUnit

    m_View.SetCurrentUnit(pUnit);
    m_View.m_iHitTest = -1;  // û��ѡ�е�Ԫ
    m_View.m_ptPoint.x = m_View.m_ptPoint.y = 0;

    //ִ��mouse LButton down
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

    // ִ��mouse move
    m_View.OnMouseMove(0, m_View.m_ptPoint);

    EXPECT_EQ(m_View.GetCurrentUnit(), nullptr);
    EXPECT_EQ(m_View.GetUnitMouseMove(), nullptr);

    CPointListPtr plistPoint = m_View.GetLinkPointList();
    shared_ptr<CPoint> pt1 = make_shared<CPoint>(0, 0);
    plistPoint->push_back(pt1); // �������һ��Ԫ��
    EXPECT_EQ(plistPoint->size(), 1);
    // ִ��LButtonUp
    m_View.OnLButtonUp(0, m_View.m_ptPoint);

    EXPECT_EQ(plistPoint->size(), 0);
    EXPECT_TRUE(m_View.GetLinkPointList()->empty()); // ��ʱ��̬������Ϊ��
    // ��������
  }
}