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

	// 以下这些函数能够把内部protected变量提取出来为测试所用。
	void SetDoc(CSQIFileDoc * pDoc) { m_pDoc = pDoc; }
	CSQIFileDoc * GetDoc(void) { return(m_pDoc); }
	void SetCurrentFunction(int nFunc) { m_nCurrentFunction = nFunc; }
	int GetCurrentFunction(void) { return m_nCurrentFunction; }
	void SetCurrentUnit(CUnitBase * pUnit) { m_pCUnitCurrent = pUnit; }
	CUnitBase * GetCurrentUnit(void) { return m_pCUnitCurrent; }
	void SetUnitNoDrag(CUnitBase * pUnit) { m_pCUnitNoDrag = pUnit; }
	CUnitBase * GetUnitNoDrag(void) { return m_pCUnitNoDrag; }
	void SetUnitMouseMove(CUnitBase * pUnit) { m_pCUnitMouseMove = pUnit; }
	CUnitBase * GetUnitMouseMove(void) { return m_pCUnitMouseMove; }
	CUnitBase * GetFirstUnit(void) { return m_pCUnitFirst; }
	CUnitBase * GetSecondUnit(void) { return m_pCUnitSecond; }

	CUnitList * GetCurrentUnitList(void) { return m_pCUnitListCurrent; }
	void SetCurrentUnitList(CUnitList * pUnitList) { m_pCUnitListCurrent = pUnitList; }
	CUnitList * GetTopUnitList(void) { return m_pCUnitListTop; }
	void SetTopUnitList(CUnitList * pUnitList) { m_pCUnitListTop = pUnitList; }

	INT64 GetDynLinkType(void) { return m_ulDynLinkType; }
	INT32 GetSrcIndex(void) { return m_lSrcIndex; }
	INT32 GetDestIndex(void) { return m_lDestIndex; }
	ULONG GetDynLinkClass(void) { return m_ulDynLinkClass; }
	CUnitDynLink * GetCurrentDynLink(void) { return m_pCUnitDynLinkCurrent; }
	bool  IsLinkIntoSrcComponnet(void) { return m_fLinkIntoSourceComponent; }
	bool IsLinkIntoDestComponent(void) { return m_fLinkIntoDestComponent; }
	CUnitComponent * GetCurrentComponent(void) { return m_pCUnitComponentCurrent; }
	CPointList * GetLinkPointList(void) { return m_plistLinkPoint; }
	CPoint GetMousePosition(void) { return m_ptMousePosition; }
	CPoint GetFirstPt(void) { return m_ptFirst; }
	CPoint GetSecondPt(void) { return m_ptSecond; }
	CRect GetFirstUnitSize(void) { return m_rectFirstUnit; }
	CRect GetSecondUnitSize(void) { return m_rectSecondUnit; }
	CRect GetCurrentUnitSize(void) { return m_rectCurrent; }

	bool IsInTestMode(void) { return CSQIUnitView::IsInTestMode(); }
	void SetTestModeFlag(bool fFlag) { CSQIUnitView::SetTestModeFlag(fFlag); }

	// 以下这些虚函数，是为了防止测试时调用系统函数
	virtual CDC * __GetDC(void) override { return m_pDC; }
	virtual void __OnPrepareDC(CDC * pDC)  override {  }
	virtual void __ClearFocus(CDC *pDC) override {  }
	virtual void __Invalidate(void) override {  }
	virtual void __ReleaseDC(CDC *pDC) override {  }
	virtual CPoint __GetDeviceScrollPosition() override { return(m_pt); }
	virtual void __SetMousePosition(CPoint ptDevice) override { m_pt = ptDevice; }
	virtual void __ClipCursor(CRect * pRect) override {  }
	virtual void __ReleaseCapture(void) override { }
	virtual bool __GetCursorPos(CPoint * pPT) override { pPT->x = m_ptCursor.x; pPT->y = m_ptCursor.y; return true; }
	virtual void __SetCursorPos(int x, int y) override { m_ptCursor.x = x; m_ptCursor.y = y; }
	virtual void __UpdateWindow(void) override {  }
	virtual CPoint __GetScrollPosition(void) override { return m_ptScrollPosition; }
	virtual void __ScrollToPosition(CPoint pt) override { m_ptScrollPosition = pt; }
	virtual void __SetDocModifiedFlag(void) override { m_fDocModifiedFlag = true; }
	virtual void __SetCurrentUnitList(CUnitList * pUnitList) override {  }
	virtual int __MouseHitTest(CPoint point) override { return m_iHitTest; }
	virtual BOOL __MouseTrackRubberBand(CSQIUnitView * pView, CPoint pt, BOOL fAllowInvert) override { return(m_fRubberBand); }
	virtual CRect __GetTrackerRect(void) override { return(m_rectTracker); }
	virtual BOOL __MouseTrack(CSQIUnitView * pView, CPoint pt, BOOL fAllowInvert) { return(m_fMouseTrack); }
	virtual void __SetTrackerRect(CRect rect) override { m_rectTracker = rect; }
	virtual void __SetStatusBarMessage(CString str) override { m_strStatusBar = str; }

	//
	void OnLButtonDown(UINT nFlags, CPoint point) {	CSQIUnitView::OnLButtonDown(nFlags, point); }
	void OnMouseMove(UINT nFlags, CPoint point) { CSQIUnitView::OnMouseMove(nFlags, point); }
	void OnLButtonUp(UINT nFlags, CPoint point) { CSQIUnitView::OnLButtonUp(nFlags, point); }


	CDC * m_pDC;
	bool   m_fDocModifiedFlag;
	CPoint m_pt;			// 当前滚动位置
	CPoint m_ptPoint; // 当前鼠标位置
	CPoint m_ptCursor;
	CPoint m_ptScrollPosition;

	CUnitList m_unitlist;
	CRect m_rectTracker;

	int m_iHitTest;			//鼠标是否选中了单元
	bool m_fMouseTrack; // 是否拖曳鼠标
	bool m_fRubberBand; // 是否是框选状态
	CString m_strStatusBar;
};

#endif // !__CSQI_UNIT_VIEW_TEST__
