#include"stdafx.h"
#include"pch.h"

#include"SQIUnitView.h"
//#include"SQIFileDoc.h"

#include"CSQIUnitViewTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

class CSQIFileDocTest : public CSQIFileDoc {
public :
	CSQIFileDocTest() : CSQIFileDoc() {
	}
	~CSQIFileDocTest() {}
};



namespace DACViewTest {

	class TestSQIUnitView : public testing::Test {
	protected:
		void SetUp() override {
			strFileName = "c:\\DACViewTestFile\\CompileUnitList\\可封装和不可封装部件嵌套四层参数有链接.sqi";
			CFile cFile;
			char buffer[512];

			CString strFileHeader;
			INT32 iObjectNumber;
			INT64 iUnitNumber;

			// 测试初始化时的变量初始值
			EXPECT_EQ(m_View.GetCurrentUnitList(), nullptr);
			EXPECT_EQ(m_View.GetTopUnitList(), nullptr);
			EXPECT_EQ(m_View.GetDoc(), nullptr);
			EXPECT_EQ(m_View.GetCurrentFunction(), UNIT_PRE_SELECT );
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

  TEST_F(TestSQIUnitView, TestInitialize) {

  }

  TEST_F(TestSQIUnitView, TestGetDocument) {

  }

  TEST_F(TestSQIUnitView, TestFindUnit) {

  }

  TEST_F(TestSQIUnitView, TestAddUnit) {

  }

  TEST_F(TestSQIUnitView, TestDeleteUnit) {

  }

  TEST_F(TestSQIUnitView, TestIsInRect) {

  }

  TEST_F(TestSQIUnitView, TestUnitToBack) {

    CUnitList *pUnitList = m_View.GetCurrentUnitList();
    CUnitAdd *pc1;

    pc1 = new CUnitAdd;
    pUnitList->push_back(pc1);

    m_View.UnitToBack(pUnitList, pc1);
    EXPECT_EQ(pUnitList->front(), pc1);

  }

  TEST_F(TestSQIUnitView, TestUnitToFront) {
    CUnitList *pUnitList = m_View.GetCurrentUnitList();
    CUnitAdd *pc1;

    pc1 = new CUnitAdd;
    pUnitList->push_front(pc1);

    m_View.UnitToFront(pUnitList, pc1);
    EXPECT_EQ(pUnitList->back(), pc1);
  }

  TEST_F(TestSQIUnitView, TestSetFocus) {

  }

  TEST_F(TestSQIUnitView, TestClearFocus) {

  }

  TEST_F(TestSQIUnitView, TestClearAllFocus) {
  }

  TEST_F(TestSQIUnitView, TestViewIn) {
    CSQIUnitViewTest c;

    CUnitComponent * pCpt = new CUnitComponent;

    c.ViewIn(pCpt);

		delete pCpt;

  }

  TEST_F(TestSQIUnitView, TestViewOut) {

  }

  TEST_F(TestSQIUnitView, TestCenterAlign) {

  }

  TEST_F(TestSQIUnitView, TestLeftAlign) {

  }

  TEST_F(TestSQIUnitView, TestRightAlign) {

  }

  TEST_F(TestSQIUnitView, TestGetUnitDocSize) {

  }

  TEST_F(TestSQIUnitView, TestSetClipRect) {

  }

  TEST_F(TestSQIUnitView, TestResetAll) {
    //CSQIUnitView  c;

    //c.CreateUniName(pc);

  }


  TEST_F(TestSQIUnitView, TestCreateUniName) {

  }

  TEST_F(TestSQIUnitView, TestDrawInvertDynLinkLine) {

  }

  TEST_F(TestSQIUnitView, TestAdjustDynLinkPoint) {

  }

  TEST_F(TestSQIUnitView, TestIsCreateFirstDynLinkPoint) {

  }

  TEST_F(TestSQIUnitView, TestDeleteDynLinkPointList) {

  }

  TEST_F(TestSQIUnitView, TestPreCreateWindow) {

  }

  TEST_F(TestSQIUnitView, TestOnDraw) {

  }

  TEST_F(TestSQIUnitView, TestOnInitialKUpdate) {

  }

  TEST_F(TestSQIUnitView, TestOnUpdate) {

  }

  TEST_F(TestSQIUnitView, TestDrawInvertLine) {

  }

  TEST_F(TestSQIUnitView, TestAddLinkPoint) {

  }

  /////////////////////////////////////////////////////////////////////////////////////
  // 
  // 以下为窗口响应函数
  //
  ///////////////////////////////////////////////////////////////////////////////////////

  TEST_F(TestSQIUnitView, TestOnLButtonDblClk) {

  }

  TEST_F(TestSQIUnitView, TestOnLButtonUp) {
    CSQIUnitViewTest c;
    UINT nFlags = 0;
    CPoint point(0, 0);

    //c.OnLButtonDown(nFlags, point);

  }

  TEST_F(TestSQIUnitView, TestOnMouseMove) {

  }

  TEST_F(TestSQIUnitView, TestOnEditCopy) {

  }

  TEST_F(TestSQIUnitView, TestOnEditCut) {

  }

  TEST_F(TestSQIUnitView, TestOnEditDelete) {

  }

  TEST_F(TestSQIUnitView, TestOnEditPaste) {

  }

  TEST_F(TestSQIUnitView, TestOnEditUndo) {

  }

  TEST_F(TestSQIUnitView, TestOnStyleCentered) {

  }

  TEST_F(TestSQIUnitView, TestOnStyleLeft) {

  }

  TEST_F(TestSQIUnitView, TestOnStyleRight) {

  }

  TEST_F(TestSQIUnitView, TestOnArrangToBack) {

  }

  TEST_F(TestSQIUnitView, TestOnArrangeToFront) {

  }

  TEST_F(TestSQIUnitView, TestOnUpdateArrageToBack) {

  }

  TEST_F(TestSQIUnitView, TestOnUpdateArrageToFront) {

  }

  TEST_F(TestSQIUnitView, TestOnUpdateEditCopy) {

  }

  TEST_F(TestSQIUnitView, TestOnUpdateEditCut) {

  }

  TEST_F(TestSQIUnitView, TestOnUpdateEditDelete) {

  }

  TEST_F(TestSQIUnitView, TestOnUpdateEditPaste) {

  }

  TEST_F(TestSQIUnitView, TestOnUpdateEditUndo) {

  }

  TEST_F(TestSQIUnitView, TestOnUpdateStyleCentered) {

  }

  TEST_F(TestSQIUnitView, TestOnUpdateStyleLeft) {

  }

  TEST_F(TestSQIUnitView, TestOnUpdateStyleRight) {

  }

  TEST_F(TestSQIUnitView, TestOnLButtonDown) {

  }

  TEST_F(TestSQIUnitView, TestOnBlockInputoutAin) {


  }

  TEST_F(TestSQIUnitView, TestOnUpdateBlock) {

  }

  TEST_F(TestSQIUnitView, TestONUpdateBlockI) {

  }

  TEST_F(TestSQIUnitView, TestOnBolckSimulateQuadratic) {

  }

  TEST_F(TestSQIUnitView, TestOnBlockSimulateSine) {

  }

  TEST_F(TestSQIUnitView, TestOnViewViewin) {

  }

  TEST_F(TestSQIUnitView, TestOnUpdateViewViewin) {

  }

  TEST_F(TestSQIUnitView, TestOnBlockMathmaticAdd) {

  }

  TEST_F(TestSQIUnitView, TestOnViewViewout) {

  }

  TEST_F(TestSQIUnitView, TestOnUpdateViewViewout) {

  }

  /*
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}

  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  TEST_F(TestSQIUnitView, TEST_F) {}
  */
}
