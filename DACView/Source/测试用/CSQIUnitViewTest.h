////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//����SQIUnitView���д����õ���ϵͳ���������޷�ֱ�Ӳ��ԣ��ʶ�����һ�⸲��SQIUnitViewTest���ڲ���ʱ�����õ�ϵͳ�����û�����
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __CSQI_UNIT_VIEW_TEST__
#define __CSQI_UNIT_VIEW_TEST_


#include"stdafx.h"

//#include"pch.h"

#include"SQIUnitView.h"
#include"SQIFileDoc.h"

#include"CUnitAdd.h"

class CSQIUnitViewTest : public CSQIUnitView {
public:
	CSQIUnitViewTest() : CSQIUnitView() { }
	~CSQIUnitViewTest() { }

	// ������Щ�����ܹ����ڲ�protected������ȡ����Ϊ�������á�
	void SetDoc(CSQIFileDoc * pDoc) { m_pDoc = pDoc; }
	CSQIFileDoc * GetDoc(void) { return(m_pDoc); }
	void SetCurrentFunction(int nFunc) { m_nCurrentFunction = nFunc; }
	int GetCurrentFunction(void) { return m_nCurrentFunction; }
	void SetCurrentUnit(CUnitBasePtr pUnit) { m_pCUnitCurrent = pUnit; }
	CUnitBasePtr GetCurrentUnit(void) { return m_pCUnitCurrent; }
	void SetUnitNoDrag(CUnitBasePtr pUnit) { m_pCUnitNoDrag = pUnit; }
	CUnitBasePtr GetUnitNoDrag(void) { return m_pCUnitNoDrag; }
	void SetUnitMouseMove(CUnitBasePtr pUnit) { m_pCUnitMouseMove = pUnit; }
	CUnitBasePtr GetUnitMouseMove(void) { return m_pCUnitMouseMove; }
	CUnitBasePtr GetFirstUnit(void) { return m_pCUnitFirst; }
	CUnitBasePtr GetSecondUnit(void) { return m_pCUnitSecond; }

	CUnitList * GetCurrentUnitList(void) { return m_pCUnitListCurrent; }
	void SetCurrentUnitList(CUnitList * pUnitList) { m_pCUnitListCurrent = pUnitList; }
	CUnitList * GetTopUnitList(void) { return m_pCUnitListTop; }
	void SetTopUnitList(CUnitList * pUnitList) { m_pCUnitListTop = pUnitList; }

	INT64 GetDynLinkType(void) { return m_ulDynLinkType; }
	INT32 GetSrcIndex(void) { return m_lSrcIndex; }
	INT32 GetDestIndex(void) { return m_lDestIndex; }
	ULONG GetDynLinkClass(void) { return m_ulDynLinkClass; }
	shared_ptr<CUnitDynLink> GetCurrentDynLink(void) { return m_pCUnitDynLinkCurrent; }
	bool  IsLinkIntoSrcComponnet(void) { return m_fLinkIntoSourceComponent; }
	bool IsLinkIntoDestComponent(void) { return m_fLinkIntoDestComponent; }
	CUnitComponentPtr GetCurrentComponent(void) { return m_pCUnitComponentCurrent; }
	CPointListPtr GetLinkPointList(void) { return m_plistLinkPoint; }
	CPoint GetMousePosition(void) { return m_ptMousePosition; }
	CPoint GetFirstPt(void) { return m_ptFirst; }
	CPoint GetSecondPt(void) { return m_ptSecond; }
	CRect GetFirstUnitSize(void) { return m_rectFirstUnit; }
	CRect GetSecondUnitSize(void) { return m_rectSecondUnit; }
	CRect GetCurrentUnitSize(void) { return m_rectCurrent; }

	// ������Щ�麯������Ϊ�˷�ֹ����ʱ����ϵͳ����
	virtual CDC * __GetDC(void) override { return m_pDC; }
	virtual void __OnPrepareDC(CDC * )  override {  }
	virtual void __ClearFocus(CDC *) override {  }
	virtual void __Invalidate(void) override {  }
	virtual void __ReleaseDC(CDC *) override {  }
	//virtual CPoint __GetDeviceScrollPosition() override { return(m_pt); }
	virtual void __SetMousePosition(CPoint ptDevice) override { m_pt = ptDevice; }
	virtual void __ClipCursor(CRect *) override {  }
	virtual void __ReleaseCapture(void) override { }
	virtual bool __GetCursorPos(CPoint * pPT) override { pPT->x = m_ptCursor.x; pPT->y = m_ptCursor.y; return true; }
	virtual void __SetCursorPos(int x, int y) override { m_ptCursor.x = x; m_ptCursor.y = y; }
	virtual void __UpdateWindow(void) override {  }
	virtual CPoint __GetScrollPosition(void) override { return m_ptScrollPosition; }
	virtual void __ScrollToPosition(CPoint pt) override { m_ptScrollPosition = pt; }
	virtual void __SetDocModifiedFlag(void) override { m_fDocModifiedFlag = true; }
	virtual void __SetCurrentUnitList(CUnitList * ) override {  }
	virtual int __MouseHitTest(CPoint ) override { return m_iHitTest; }
	virtual BOOL __MouseTrackRubberBand(CSQIUnitView * , CPoint , BOOL ) override { return(m_fRubberBand); }
	virtual CRect __GetTrackerRect(void) override { return(m_rectTracker); }
	virtual BOOL __MouseTrack(CSQIUnitView * , CPoint , BOOL ) { return(m_fMouseTrack); }
	virtual void __SetTrackerRect(CRect rect) override { m_rectTracker = rect; }
	virtual void __SetStatusBarMessage(CString str) override { m_strStatusBar = str; }

	virtual void __DefaultOnLButtonDown(UINT , CPoint ) override { }
	virtual void __DefaultOnMouseMove(UINT , CPoint ) override {}
	virtual void __DefaultOnLButtonUp(UINT , CPoint ) override {}
	virtual void __DefaultOnLButtonDblClk(UINT , CPoint ) override { }
	virtual void __DefaultOnRButtonUp(UINT , CPoint ) override { }

	virtual INT_PTR __DlgChoicePara(CDlgChoiceParameter* Dlg) override { return IDOK; }



	//
	void OnLButtonDown(UINT nFlags, CPoint point) {	CSQIUnitView::OnLButtonDown(nFlags, point); }
	void OnMouseMove(UINT nFlags, CPoint point) { CSQIUnitView::OnMouseMove(nFlags, point); }
	void OnLButtonUp(UINT nFlags, CPoint point) { CSQIUnitView::OnLButtonUp(nFlags, point); }


	CDC * m_pDC;
	bool   m_fDocModifiedFlag;
	CPoint m_pt;			// ��ǰ����λ��
	CPoint m_ptPoint; // ��ǰ���λ��
	CPoint m_ptCursor;
	CPoint m_ptScrollPosition;

	CUnitList m_unitlist;
	CRect m_rectTracker;

	int m_iHitTest;			//����Ƿ�ѡ���˵�Ԫ
	bool m_fMouseTrack; // �Ƿ���ҷ���
	bool m_fRubberBand; // �Ƿ��ǿ�ѡ״̬
	CString m_strStatusBar;
};

#endif // !__CSQI_UNIT_VIEW_TEST__
