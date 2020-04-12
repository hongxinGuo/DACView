#include"stdafx.h"
#include"pch.h"

#include"CUnitBase.h"
#include"CUnitAdd.h"
#include"CUnitComponent.h"
#include"CUnitComponent.h"
#include"CUnitDynLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace DACViewTest {
  TEST(TestCUnitBase, TestCUnitBaseInitialize) {
    CPoint pt(100, 100);
    CUnitBase c("CUnitBase", pt);
    CRect rect;

    EXPECT_STREQ(_T("CUnitBase"), c.GetName());
    EXPECT_TRUE(c.GetUpperUnitList() == nullptr);
    rect = c.GetSize();
    EXPECT_TRUE(rect.left == pt.x - 20);
    EXPECT_TRUE(rect.right == pt.x + 20);
    EXPECT_TRUE(rect.top == pt.y - 20);
    EXPECT_TRUE(rect.bottom == pt.y + 20);
    EXPECT_EQ(100, c.GetScanRate());
    EXPECT_EQ(0, c.GetExectivePriority());
    EXPECT_EQ(0, c.HowManyLinkToComponent());
    CUDLList* pList;
    pList = c.GetDynLinkList();
    EXPECT_EQ(0, pList->size());
    EXPECT_FALSE(c.IsDynLinkFromUpper());
    EXPECT_TRUE(c.IsEncapsulated());
    EXPECT_FALSE(c.IsLinkToComponent());
    EXPECT_FALSE(c.IsHaveSourceUnit());
    EXPECT_FALSE(c.IsCutoff());
    EXPECT_FALSE(c.CanLinkIn());
    EXPECT_FALSE(c.CanViewIn());
    EXPECT_FALSE(c.IsOverFlow());
    EXPECT_EQ(0, c.GetExectivePriority());

    CUnitBase c2;
    rect = c2.GetSize();
    EXPECT_EQ(rect.left, 0);
    EXPECT_EQ(rect.right, 0);
    EXPECT_EQ(rect.top, 0);
    EXPECT_EQ(rect.bottom, 0);
    EXPECT_EQ(100, c2.GetScanRate());
    EXPECT_EQ(0, c2.GetExectivePriority());
    EXPECT_EQ(0, c2.HowManyLinkToComponent());
    pList = c2.GetDynLinkList();
    EXPECT_EQ(0, pList->size());
    EXPECT_FALSE(c2.IsDynLinkFromUpper());
    EXPECT_TRUE(c2.IsEncapsulated());
    EXPECT_FALSE(c2.IsLinkToComponent());
    EXPECT_FALSE(c2.IsHaveSourceUnit());
    EXPECT_FALSE(c2.IsCutoff());
    EXPECT_FALSE(c2.CanLinkIn());
    EXPECT_FALSE(c2.CanViewIn());
    EXPECT_FALSE(c2.IsOverFlow());
    EXPECT_EQ(0, c.GetExectivePriority());
  }

  TEST(TestCUnitBase, TestSerialize) {
    CUnitBase c;
    CFile cFile1, cFile2;
    char buffer[512];
    CString strFileName = "CUnitBase.tst";
    CUnitBase* pc = new CUnitBase, * pc2;
    CPoint pt1(100, 100), pt2(1000, 1000);
    CRect rect(pt1, pt2);

    cFile1.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
    CArchive ar(&cFile1, CArchive::store, 512, buffer);
    pc->SetComment(strFileName);
    pc->SetSize(rect);
    ar << pc; //
    ar.Flush(); // ����flush�������п���û���д洢
    cFile1.Close();
    pc->SetComment("");
    pt1.x = 200;
    pt1.y = 200;
    rect.top = 200;
    rect.left = 200;
    rect.bottom = 500;
    rect.right = 500;
    pc->SetSize(rect);
    if (!cFile2.Open(strFileName, CFile::modeRead)) {
      ASSERT_FALSE(true) << "Can not load " << strFileName;
    }
    CArchive ar2(&cFile2, CArchive::load, 512, buffer);
    ar2 >> pc2;
    rect = pc2->GetSize();
    EXPECT_EQ(100, rect.top);
    EXPECT_EQ(100, rect.left);
    EXPECT_EQ(1000, rect.bottom);
    EXPECT_EQ(1000, rect.right);
    EXPECT_STREQ(strFileName, pc2->GetComment());

    delete pc2;
    delete pc;
  }

  TEST(TestCUnitBase, TestSetUpperUnitList) {
    CUnitList list, * plist, * plist2;
    CUnitBase c;

    plist = c.GetUpperUnitList();
    plist2 = nullptr;
    EXPECT_EQ(plist, plist2);
    c.SetUpperUnitList(&list);
    plist = c.GetUpperUnitList();
    plist2 = &list;
    EXPECT_EQ(plist, plist2);
  }

  TEST(TestCUnitBase, TestSetArea) {
    CPoint pt(0, 0), pt2(100, 100);
    CRect rect(pt, pt2);
    CUnitBase c("CUnitBase", pt);
    c.SetSize(rect);
    rect.bottom = 1000;
    rect.top = 200;
    rect.left = 1000;
    rect.right = 2000;
    rect = c.GetSize();
    EXPECT_EQ(0, rect.top);
    EXPECT_EQ(0, rect.left);
    EXPECT_EQ(100, rect.bottom);
    EXPECT_EQ(100, rect.right);
  }

  TEST(TestCUnitBase, TestSetComment) {
    CUnitBase c;
    CString str = "comment";
    c.SetComment(str);
    EXPECT_STREQ(_T("comment"), c.GetComment());
  }

  TEST(TestCUnitBase, TestGetDynLinkList) {
    // do nothing
  }

  TEST(TestCUnitBase, TestLinkToComponent) {
    CPoint pt(100, 100);
    CUnitBase c("CUnitBase", pt);

    EXPECT_FALSE(c.IsLinkToComponent()) << "��ʼ״̬ʱû�����ӵ�����";
    c.LinkToComponent(true);						//���ñ�־Ϊ��
    EXPECT_TRUE(c.IsLinkToComponent());
    c.LinkToComponent(false);						//
    EXPECT_FALSE(c.IsLinkToComponent()) << "��ʱû�����ӵ�����";
    EXPECT_EQ(c.HowManyLinkToComponent(), 0) << "��ֵΪ��";
    c.LinkToComponent(false);						// �ٴ����ñ�־Ϊ��
    EXPECT_FALSE(c.IsLinkToComponent()) << "��ʱû�����ӵ�����";
    EXPECT_EQ(c.HowManyLinkToComponent(), 0) << "Ϊ��ʱ�ٴ�����Ϊ���޶�����������-1";
  }

  TEST(TestCUnitBase, TestLinkFromComponent) {
    CPoint pt(100, 100);
    CUnitBase c("CUnitBase", pt);

    EXPECT_FALSE(c.IsLinkFromComponent());
    c.LinkFromComponent(true);
    EXPECT_TRUE(c.IsLinkFromComponent());
    c.LinkFromComponent(true);
    EXPECT_TRUE(c.IsLinkFromComponent());
    EXPECT_EQ(2, c.HowManyLinkFromComponent());
    c.LinkFromComponent(false);
    EXPECT_TRUE(c.IsLinkFromComponent());
    c.LinkFromComponent(false);
    EXPECT_FALSE(c.IsLinkFromComponent());
    EXPECT_EQ(0, c.HowManyLinkFromComponent());
    c.LinkFromComponent(false); // ��û���������벿��ʱ���ٴ�����Ϊ���޶������ƺ�Ӧ����Ϊ�Ǵ��󣿣�
    EXPECT_EQ(0, c.HowManyLinkFromComponent()) << "��û���������벿��ʱ���ٴ�����Ϊ���޶���";
  }

  TEST(TestCUnitBase, TestIsLinkToComponent) {
    CUnitBase c;
    CUnitComponent cpt;
    EXPECT_FALSE(c.IsLinkToComponent());
    c.LinkToComponent(true);
    EXPECT_TRUE(c.IsLinkToComponent());
    c.LinkToComponent(true);
    EXPECT_TRUE(c.IsLinkToComponent());
    EXPECT_EQ(2, c.HowManyLinkToComponent());
    c.LinkToComponent(false);
    EXPECT_TRUE(c.IsLinkToComponent());
    c.LinkToComponent(false);
    EXPECT_FALSE(c.IsLinkToComponent());
    EXPECT_EQ(0, c.HowManyLinkToComponent());
    c.LinkToComponent(false); // ��û���������벿��ʱ���ٴ�����Ϊ���޶������ƺ�Ӧ����Ϊ�Ǵ��󣿣�
    EXPECT_EQ(0, c.HowManyLinkToComponent());
  }

  TEST(TestCUnitBase, TestHowManyLinkToComponent) {
    CUnitBase c;
    EXPECT_EQ(0, c.HowManyLinkToComponent());
    c.LinkToComponent(true);
    EXPECT_EQ(1, c.HowManyLinkToComponent());
    c.LinkToComponent(false);
    EXPECT_EQ(0, c.HowManyLinkToComponent());
    c.LinkToComponent(false);
    EXPECT_EQ(0, c.HowManyLinkToComponent()) << "��û�ж�̬����������ʱ��Ϊ0�����ٴ�ȡ��״̬��־��������";
  }

  TEST(TestCUnitBase, TestSetComponentThatHaveMe) {
    CPoint pt(100, 100);
    CUnitBase c("CUnitBase", pt);
    CUnitComponent* pcc1;
    CUnitComponent* pcc;
    pcc1 = new CUnitComponent("this compound", pt, true);
    CString str = "this";
    c.SetComponentUpper(pcc1);
    pcc = c.GetComponentUpper();
    EXPECT_EQ(pcc, pcc1);
    delete pcc1;
  }

  TEST(TestCUnitBase, TestClearDynamicLinkFlag) {
    CUnitAdd c1, c2, c3;
    CUDLList* plist = c1.GetDynLinkList();
    shared_ptr<CUnitDynLink> pDL, pDL2;
    CUnitList list;

    list.push_back(&c2);

    pDL = make_shared<CUnitDynLink>();
    pDL->SetDestUnit(&c2);
    pDL->SetDestIndex(1);
    pDL->SetSrcUnit(&c1);
    pDL->SetSrcIndex(2);
    pDL->SetDynLinkType(tDOUBLE);
    plist->push_back(pDL);

    pDL2 = make_shared<CUnitDynLink>();
    pDL2->SetDestUnit(&c3);
    pDL2->SetDestIndex(0);
    pDL2->SetSrcUnit(&c1);
    pDL2->SetSrcIndex(2);
    pDL2->SetDynLinkType(tDOUBLE);
    plist->push_back(pDL2);

    EXPECT_FALSE(pDL->IsDeleteMe());
    EXPECT_FALSE(pDL2->IsDeleteMe());
    c1.SetDeleteDynLinkFlag(list);
    EXPECT_FALSE(pDL->IsDeleteMe()) << "c2���ڱ�ѡ��Ԫ�����У���Ӧ������ɾ����־"; // c2������ѡ��ĵ�Ԫ�����У�������ɾ����־
    EXPECT_TRUE(pDL2->IsDeleteMe()); // c3�����ⲿ��������ɾ����־
    c1.ClearDeleteDynLinkFlag();
    EXPECT_FALSE(pDL->IsDeleteMe());
    EXPECT_FALSE(pDL2->IsDeleteMe());

    plist->clear();
  }

  TEST(TestCUnitBase, TestSetDeleteDynLinkFlag) {
  }

  TEST(TestCUnitBase, TestSetCutOff) {
    CPoint pt(100, 100);
    CUnitBase c("CUnitBase", pt);
    c.SetCutOff(true);
    EXPECT_TRUE(c.IsCutoff());
    c.SetCutOff(true);
    EXPECT_TRUE(c.IsCutoff());
    c.SetCutOff(false);
    EXPECT_FALSE(c.IsCutoff());
    c.SetCutOff(false);
    EXPECT_FALSE(c.IsCutoff());
  }

  TEST(TestCUnitBase, TestSetExectivePriorityDirect) {
    CPoint pt(100, 100);
    CUnitBase c("CUnitBase", pt);
    c.SetExectivePriorityDirect(100);
    EXPECT_EQ(100, c.GetExectivePriority());
  }

  TEST(TestCUnitBase, TestEncapsulation) {
    // ��װ���Ե������У����ڴ˴���
  }

  TEST(TestCUnitBase, TestIsEncapsulated) {
    CPoint pt(100, 100);
    CUnitBase c("CUnitBase", pt);
    EXPECT_TRUE(c.IsEncapsulated()); // ��Զ���棬����Ҫ����
  }

  TEST(TestCUnitBase, TestIsEncapsulating) {
    CUnitBase c;
    EXPECT_FALSE(c.IsEncapsulating());
    c.SetEncapsulatingFlag(true);
    EXPECT_TRUE(c.IsEncapsulating());
    c.SetEncapsulatingFlag(false);
    EXPECT_FALSE(c.IsEncapsulating());
  }

  TEST(TestCUnitBase, TestPrepareRunTimeList) {
    // no nothing
  }

  TEST(TestCUnitBase, TestAddDynLink) {
    CUnitBase c;
    shared_ptr<CUnitDynLink> pDL2 = make_shared<CUnitDynLink>(), pDL;
    CUDLList* pDLList = c.GetDynLinkList();
    int iCount = pDLList->size();
    pDL2->SetName("aaa");
    pDLList->push_back(pDL2);
    EXPECT_EQ(pDLList->size(), iCount + 1);
    pDL = pDLList->back();
    CString str = pDL->GetName();
    EXPECT_STREQ(pDL->GetName(), _T("aaa"));
  }

  TEST(TestCUnitBase, TestGetSelectedPtr) {
  }

  TEST(TestCUnitBase, TestArrangeDynLInk) {
    // ������������˶Ի������ʶ������������
  }

  TEST(TestCUnitBase, TestDeleteDynLink) {
    // �Ժ��ٲ���
  }

  TEST(TestCUnitBase, TestCreateUniName) {
    // �Ժ��ٲ���
  }

  TEST(TestCUnitBase, TestAdjustDynamicLinkPosition) {
    //
  }

  TEST(TestCUnitBase, TestSetSelectParaFlag) {
  }

  TEST(TestCUnitBase, TestGetInputParaNumber) {
  }

  TEST(TestCUnitBase, TestIsDynLinkFromOutSide) {
  }

  TEST(TestCUnitBase, TestOverFlow) {
    CUnitBase c;
  }

  TEST(TestCUnitBase, TestSetDestUnitPriority) {
    CUnitBase cc, * pc = new CUnitBase;
    shared_ptr<CUnitDynLink> pdl = make_shared<CUnitDynLink>();
    cc.SetInputParameterNumber(1);// �ڵ���״̬�£�SetHaveSourceUnit�ĺ�������һ������m_ulDynLinkToNumber����0���ʶ���Ҫֱ�����������0
    pdl->SetDestUnit(&cc);
    pdl->SetSrcUnit(pc);
    pc->SetExectivePriorityDirect(1);
    pc->AddDynLink(pdl);
    pc->SetDestUnitPriority();
    EXPECT_EQ(2, cc.GetExectivePriority());
    pc->SetExectivePriorityDirect(100);
    cc.SetInputParameterNumber(2);// ��ʱ��Ҫαװ��������ĸ���Ϊ2����Ϊ�����������ȼ�ʱ���Զ�����������ĵ�ǰ����һ��
    pc->SetDestUnitPriority();			// ����״̬ʱ�˺�����һ�����ԡ������αװ�����������Ϊ2�Ļ����˶���Ϊ�١�
    EXPECT_EQ(101, cc.GetExectivePriority());

    delete pc;
  }

  TEST(TestCUnitBase, TestIsHaveSourceUnit) {
    CUnitBase c;

    c.SetInputParameterNumber(1); // ����m_ulDynLinkToNumberΪ1, ��ֹ���溯���еĶ���Ϊ�١�
    EXPECT_TRUE(c.IsHaveSourceUnit());
    c.SetInputParameterNumber(0);
    EXPECT_FALSE(c.IsHaveSourceUnit());
  }

  TEST(TestCUnitBase, TestSetLoopDetectFlag) {
    CUnitBase c;
  }

  TEST(TestCUnitBase, TestClearLoopDetectFlag) {
  }

  TEST(TestCUnitBase, TestGetClassName) {
    CPoint pt(100, 100);
    CUnitBase c("CUnitBase", pt);
    CString str = "UnitBase";
    EXPECT_STREQ(str, c.GetClassNameStr());
  }

  TEST(TestCUnitBase, TestCanLinkIn) {
    CUnitBase c;
    EXPECT_FALSE(c.CanLinkIn()); // ��ԶΪ��
  }

  TEST(TestCUnitBase, TestIsMe) {
    CPoint pt(100, 100);
    CUnitBase c("CUnitBase", pt);
    CString str = "this name";
    c.SetName(str);
    EXPECT_STREQ(_T("this name"), c.GetName());
    //XPECT_STREQ("thsi name", c.GetName());
  }

  TEST(TestCUnitBase, TestGetUnitType) {
    CPoint pt(100, 100);
    CUnitBase c("CUnitBase", pt);
    ULONG u = tINPUT | tOUTPUT;
    EXPECT_EQ(u, c.GetUnitType());
  }

  TEST(TestCUnitBase, TestExectiveDynLink) {
  }

  TEST(TestCUnitBase, TestSetParameterLock) {
    // �麯������Ҫ�����ṩ����������ݡ�
  }

  TEST(TestCUnitBase, TestPrepareParameterDictonary) {
  }

  TEST(TestCUnitBase, TestSelectParameter) {
  }

  TEST(TestCUnitBase, TestGetParaNameAddress) {
  }

  TEST(TestCUnitBase, TestCreateParaDictionary) {
  }

  TEST(TestCUnitBase, TestInIt) {
    CPoint pt(10, 300);
    CUnitBase c("CUnitBase", pt);
    CRect rect;
    rect.top = 100;
    rect.bottom = 1000;
    rect.left = 300;
    rect.right = 3000;
    c.SetSize(rect);
    EXPECT_FALSE(c.InIt(pt));
    pt.x = 300;
    EXPECT_TRUE(c.InIt(pt));
    // RECT�ķ�Χ�ǰ���left��top����ֵ���籾���е�300�� 100���� ������right��bottom����ֵ���籾���е�3000��1000��
    pt.x = 3000;
    EXPECT_FALSE(c.InIt(pt)); // ������3000
    pt.x = 2999;
    EXPECT_TRUE(c.InIt(pt));	// right������2999
  }

  TEST(TestCUnitBase, TestAddToList) {
    CPoint pt(100, 100);
    CUnitBase c("CUnitBase", pt);
    CUnitList cList;
    CUnitBase* pc, * pc2;
    pc = &c;
    cList.push_back(pc);
    pc2 = cList.front();
    EXPECT_EQ(pc, pc2);
  }

  TEST(TestCunitBase, TestSetInputParameterNumber) {
    CUnitBase c;

    c.SetInputParameterNumber(2);
    EXPECT_EQ(2, c.GetInputParameterNumber());
  }

  TEST(TestCunitBase, TestAddDynLink) {
    CUnitBase c;
    CUnitDynLink dl;

    //EXPECT_DEATH(c.AddDynLink(nullptr), "");
  }

  TEST(TestCUnitBase, TestReSetCompileFlag) {
    CUnitAdd c; // ����ʹ��CUnitbase������ֶ��Ա���

    c.SetCompiledFlag(true);
    c.SetDynLinkToNumber(4);
    c.SetExectivePriority(5);
    c.ResetCompileFlag();
    EXPECT_FALSE(c.IsCompiled());
    EXPECT_EQ(0, c.GetExectivePriority());
  }
  TEST(TestCUnitBase, TestGetParaName) {
  }

  TEST(TestCUnitBase, TestGetParaType) {
  }

  TEST(TestCUnitBase, TestExective) {
  }

  TEST(TestCUnitBase, TestGetIndex) {
  }

  TEST(TestCUnitBase, TestGetArrayIndex) {
  }

  TEST(TestCUnitBase, TestGetDynLinkType) {
  }

  TEST(TestCUnitBase, TestBool) {
  }
}