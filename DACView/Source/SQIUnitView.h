// stratvw.h : interface of the CSQIUnitView class
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __SQI_UNIT_VIEW_H__
#define __SQI_UNIT_VIEW_H__

#include"Dacview.h"
#include "SQIFileDoc.h"

#include "cUnitDynLink.h"           
           
#include "CUnitBase.h" 
#include "cUnitComponent.h"
#include"DlgChoicePara.h"


enum {
  UNIT_SELECTED = 17,		                    // ѡ����һ����Ԫ��׼������
  UNIT_GROUP_SELECTED = 18,		              // ѡ����һ�鵥Ԫ��׼���������С����Ƶȣ�
  CREATE_NEW_UNIT = 20,		                  // ����һ���µ�Ԫ  
  DYNAMIC_LINK_FIRST_UNIT = 21,		          // ��ѡ���һ����̬���ӵ�Ԫ���˵�Ԫ����Ϊ�򵥵�Ԫ��
  DYNAMIC_LINK_SECOND_UNIT = 22,		        // ��ѡ��ڶ�����̬���ӵ�Ԫ���˵�Ԫ����Ϊ�򵥵�Ԫ��  
  DYNAMIC_LINK_TO_FIRST_COMPONENT = 25,		  // ��ѡ���һ����̬���ӵ�Ԫ���˵�Ԫ����Ϊ������
  DYNAMIC_LINK_TO_SECOND_COMPONENT = 26,	  // ��ѡ��ڶ�����̬���ӵ�Ԫ���˵�Ԫ����Ϊ������
  NO_DRAG = 31,		                          // ���������κ����� 
  UNIT_PRE_SELECT = 32,		                  // δѡ���κε�Ԫ  
};
 
class CSQIUnitView : public CScrollView
{
  friend class TestSQIUnitView;
protected: // create from serialization only

  CSQIUnitView();
  DECLARE_DYNCREATE(CSQIUnitView)

  // Attributes
public:
  CSQIFileDoc* GetDocument();

  // Operations
public:
  // change an object's position to ObjList's head
  bool UnitToBack(CUnitList * pUnitList, CUnitBase * const pCUnit);
  // change an object's position to ObjList's Tail
  bool UnitToFront(CUnitList * pUnitList, CUnitBase * const pCUnit);

  bool DeleteUnit(CUnitBase * pCUnit);
  bool IsInRect(CPoint const pt, CUnitBase*& pcunit);

  void SetFocus(CDC *pdc);			// object is on focus
  void ClearFocus(CDC *pdc);
  void ClearAllSelect(void);

  void ViewIn(CUnitComponent * pUnit);
  void ViewOut(void);

  void CenterAlign(void);
  void LeftAlign(void);
  void RightAlign(void);

  CSize GetUnitDocSize(void);
  void SetClipRect(CRect rectClip);

  void ResetAll(ULONG ulType);

protected:
  void CreateUniName(CUnitBase * pCUnit);
  void DrawInvertDynLinkLine(CDC * pdc, CPointList * plistLinkPoint, CPoint ptFirst, CPoint ptSecond, CPoint ptCurrent);
  void AdjustDynLinkLinePosition(CUnitBase * punitCurrent, CPoint ptStart, CPoint ptEnd);
  void DeleteDynLinkPointList(CPointList * plistLinkPoint);

public:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual ~CSQIUnitView();
  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
  void OnInitialUpdate();
  void OnUpdate(void);
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Printing support
protected:
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ����֧�ֺ�����Ϊ�˷�ֹ����ϵͳ��������ֱ�ӵ��ø�Ϊʹ���麯����Ȼ������������ʹ���޶�������ֱ�Ӹ�ֵ�滻
protected:
  virtual CDC *		__GetDC(void) { return(GetDC()); }
  virtual void		__OnPrepareDC(CDC * pDC) { OnPrepareDC(pDC); }
  virtual void		__ClearFocus(CDC *pDC) { ClearFocus(pDC); }
  virtual void		__Invalidate(void) { Invalidate(); }
  virtual void		__ReleaseDC(CDC *pDC) { ReleaseDC(pDC); }
  //virtual CPoint	__GetDeviceScrollPosition() { return(GetDeviceScrollPosition());}
  virtual void		__SetMousePosition(CPoint ptDevice) { SetMousePosition(ptDevice); }
  virtual void		__ClipCursor(CRect * pRect) { ClipCursor(pRect); }
  virtual void		__ReleaseCapture(void) { ReleaseCapture(); }
  virtual bool		__GetCursorPos(CPoint * pPT) { return(GetCursorPos(pPT)); }
  virtual void		__SetCursorPos(int x, int y) { SetCursorPos(x, y); }
  virtual void		__UpdateWindow(void) { UpdateWindow(); }
  virtual CPoint	__GetScrollPosition(void) { return(GetScrollPosition()); }
  virtual void		__ScrollToPosition(CPoint pt) { ScrollToPosition(pt); }
	virtual void		__SetCursor(HCURSOR hCursor) { SetCursor(hCursor); }
	virtual void		__SetDocModifiedFlag(void) { m_pDoc->SetModifiedFlag(); }
	virtual void		__SetCurrentUnitList(CUnitList * pUnitList) { m_pDoc->SetCurrentUnitList(pUnitList); }
	virtual int			__MouseHitTest(CPoint point) { return(m_pDoc->m_trackerUnit.HitTest(point)); }
	virtual BOOL		__MouseTrackRubberBand(CSQIUnitView * pView, CPoint pt, BOOL fAllowInvert) { return(m_pDoc->m_trackerUnit.TrackRubberBand(pView, pt, fAllowInvert)); }
	virtual CRect		__GetTrackerRect(void) { return(m_pDoc->m_trackerUnit.m_rect); }
	virtual BOOL		__MouseTrack(CSQIUnitView * pView, CPoint pt, BOOL fAllowInvert) { return(m_pDoc->m_trackerUnit.Track(pView, pt, fAllowInvert)); }
	virtual void		__SetTrackerRect(CRect rect) { m_pDoc->m_trackerUnit.m_rect = rect; }
	virtual void		__SetStatusBarMessage(CString str) { SetStatusBarMessage(str); }

	virtual void    __DefaultOnLButtonDown(UINT nFlags, CPoint point) { CView::OnLButtonDown(nFlags, point); }
	virtual void		__DefaultOnMouseMove(UINT nFlags, CPoint point) { CView::OnMouseMove(nFlags, point); }
	virtual void		__DefaultOnLButtonUp(UINT nFlags, CPoint point) { CView::OnLButtonUp(nFlags, point); }
	virtual void		__DefaultOnLButtonDblClk(UINT nFlags, CPoint point) { CView::OnLButtonDblClk(nFlags, point); }
	virtual void		__DefaultOnRButtonUp(UINT nFlags, CPoint point ) { CScrollView::OnRButtonUp(nFlags, point); }

	virtual INT_PTR __DlgChoicePara(CDlgChoiceParameter* Dlg) { return Dlg->DoModal(); }

private:
  void DrawInvertLine(CDC * pdc, ULONG ulWidth, CPoint ptStart, CPoint ptEnd);

public:

protected:
  CSQIFileDoc *       m_pDoc;               // �����ļ�ָ��
	int 								m_nCurrentFunction;		// DragDrop, TraceMouse, ect.
	int 								m_nSavedFunction;

	CRect 							m_rectCurrent;				// current selected object's rect area 
	CRect		 						m_rectFirstUnit;			// ���ɶ�̬����ʱ����һ����Ԫ��ռ������
  CRect               m_rectSecondUnit;     // ���ɶ�̬����ʱ���ڶ�����Ԫ��ռ������
	CUnitBase * 				m_pCUnitCurrent; 			// point to current selected object, nullptr if no selected object
	CUnitBase * 				m_pCUnitNoDrag;				// ��ָ��Ŀǰ����ûʹ��
	CUnitBase *					m_pCUnitMouseMove;		// ����ƶ���unit��ʱ��ָ���unit
	
	HCURSOR 						m_hCursorArrow;				// save old cursor
	HCURSOR 						m_hCursorDynamicLink;	//
	HCURSOR							m_hCursorNoDrag;
  HCURSOR             m_hCursorComponent;
	         	
	ULONG	   						m_ulDynLinkType;
	INT32  	  		   		m_lSrcIndex;
	INT32 			 				m_lDestIndex;
	ULONG								m_ulDynLinkClass;
	CUnitDynLink *      m_pCUnitDynLinkCurrent;
	CUnitBase * 				m_pCUnitFirst;					// ָ���һ����Ԫ�����ѡ�еĵ�Ԫ��δ��װ�Ĳ��������һ����Ԫ�����ڴ˲���
	CUnitBase * 				m_pCUnitSecond;					// ָ��ڶ�����Ԫ��
	
  // my unit clipboard format
	CFont 							m_fontStrategyView;
	static UINT 				m_uUnitFormat;	
	HGLOBAL							hData;

	bool								m_fLinkIntoSourceComponent;	// �Ƿ�����Դ������־�����ڴ�����̬����
	bool								m_fLinkIntoDestComponent;		// �Ƿ�����Ŀ�ز�����־�����ڴ�����̬����

  INT32								m_lComponentLayer;			// ����Ĳ������������ϲ�����Ϊ0��

	CUnitList *					m_pCUnitListTop;					// ���ϲ�ĵ�Ԫ���У�������SQIUnitViewʱ��ֵ������SQIFileDoc�е��Ǹ���Ԫ���У�������ı�
  CUnitList *	        m_pCUnitListCurrent;		  // ��ǰ����ĵ�Ԫ����
	CObjectList *				m_pObjectList;						// Object���е�ָ��
  CUnitComponent * 		m_pCUnitComponentCurrent;	// ��ǰ����Ĳ����� �˲��������ĵ�Ԫ���У����ǵ�ǰ����ĵ�Ԫ���С�
  CPoint							m_ptCurrentScrollPosition;
  CRect 							m_crectClip;

  CPointList *        m_plistLinkPoint;
	CPoint 							m_ptMousePosition;		// ��¼���ɶ�̬����ʱ�������λ��  
  CPoint              m_ptCurrent;
  CPoint              m_ptFirst;
  CPoint              m_ptSecond;

	CRect								m_rectTracker;
  
// Generated message map functions
protected:
	//{{AFX_MSG(CSQIUnitView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditDelete();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
	afx_msg void OnStyleCentered();
	afx_msg void OnStyleLeft();
	afx_msg void OnStyleRight();
	afx_msg void OnArrangeToback();
	afx_msg void OnArrangeTofront();
	afx_msg void OnUpdateArrangeToback(CCmdUI* pCmdUI);
	afx_msg void OnUpdateArrangeTofront(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStyleCentered(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStyleRight(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBlockInputoutputAin();
	afx_msg void OnUpdateBlock(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBlockI(CCmdUI* pCmdUI);
	afx_msg void OnBlockMathmaticAdd();
	afx_msg void OnBlockSimulateQuadratic();
	afx_msg void OnBlockSimulateSine();
	afx_msg void OnViewViewin();
	afx_msg void OnUpdateViewViewin(CCmdUI* pCmdUI);
	afx_msg void OnViewViewout();
	afx_msg void OnUpdateViewViewout(CCmdUI* pCmdUI);
	afx_msg void OnBlockComponent();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateObjectBar(CCmdUI* pCmdUI);
	afx_msg void OnBlockMathmaticMultiple();
	afx_msg void OnBlockBasiclogicAnd();
	afx_msg void OnBlockBasiclogicDlay();
	afx_msg void OnBlockBasiclogicDrot();
	afx_msg void OnBlockBasiclogicInv();
	afx_msg void OnBlockBasiclogicOr();
	afx_msg void OnBlockBasiclogicPuls();
	afx_msg void OnBlockBasiclogicXor();
	afx_msg void OnBlockAdvancelogicFflp();
	afx_msg void OnBlockAdvancelogicIcnt();
	afx_msg void OnBlockArithmaticQflt();
	afx_msg void OnBlockArithmaticHflt();
	afx_msg void OnBlockAdvancecontrolSchd();
	afx_msg void OnBlockAdvancecontrolSeq();
	afx_msg void OnBlockAdvancecontrolSeqe();
	afx_msg void OnBlockAdvancelogicTtb();
	afx_msg void OnBlockSelectHsel();
	afx_msg void OnBlockSelectLsel();
	afx_msg void OnBlockSelectMsel();
	afx_msg void OnBlockSelectSwch();
	afx_msg void OnBlockMathmaticExpr();
	afx_msg void OnBlockMathmaticDiv();
	afx_msg void OnBlockMathmaticAvgt();
	afx_msg void OnBlockAdvancelogicTtbe();
	afx_msg void OnBlockArithmaticTot();
	afx_msg void OnBlockArithmaticAlrm();
	afx_msg void OnBlockArithmaticChar();
	afx_msg void OnBlockArithmaticCond();
	afx_msg void OnBlockArithmaticDtim();
	afx_msg void OnBlockArithmaticLlag();
	afx_msg void OnBlockBasiccontrolAmb();
	afx_msg void OnBlockBasiccontrolIntd();
	afx_msg void OnBlockBasiccontrolPd();
	afx_msg void OnBlockBasiccontrolPid();
	afx_msg void OnBlockBasiccontrolTime();
	afx_msg void OnBlockHistorytrendAhst();
	afx_msg void OnBlockHistorytrendDhst();
	afx_msg void OnBlockHistorytrendRprt();
	afx_msg void OnBlockHistorytrendSpc();
	afx_msg void OnBlockInputoutputAins();
	afx_msg void OnBlockInputoutputAkbd();
	afx_msg void OnBlockInputoutputAots();
	afx_msg void OnBlockInputoutputAout();
	afx_msg void OnBlockInputoutputDin();
	afx_msg void OnBlockInputoutputDins();
	afx_msg void OnBlockInputoutputDkbd();
	afx_msg void OnBlockInputoutputDots();
	afx_msg void OnBlockInputoutputDout();
	afx_msg void OnBlockInputoutputHorn();
	afx_msg void OnBlockInputoutputTpo();
	afx_msg void OnBlockMathmaticAvg();
	afx_msg void OnBlockSimulateLogarithm();
	afx_msg void OnUpdateArrangeBreakdynlink(CCmdUI* pCmdUI);
	afx_msg void OnArrangeBreakdynlink();
	afx_msg void OnBlockArithmaticHpbg();
	afx_msg void OnBlockInputoutputSim();
	afx_msg void OnUpdateArrangeTogglecutoff(CCmdUI* pCmdUI);
	afx_msg void OnArrangeTogglecutoff();
	afx_msg void OnViewProperty();
	afx_msg void OnUpdateViewProperty(CCmdUI* pCmdUI);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnArrangeMakedynlink();
	afx_msg void OnUpdateArrangeMakedynlink(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStyleLeft(CCmdUI* pCmdUI);
	afx_msg void OnBlockCompound();
	afx_msg void OnUpdateArrangeLinkIntoComponent(CCmdUI* pCmdUI);
	afx_msg void OnArrangeLinkIntoComponent();
	afx_msg void OnUpdateBreakLinkOfComponent(CCmdUI* pCmdUI);
	afx_msg void OnBreakLinkOfComponent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnArrangeEncapsulation();
//	afx_msg void OnUpdateArrangeEncapsulation(CCmdUI *pCmdUI);
//	afx_msg void OnArrangeDecapsulation();
//	afx_msg void OnUpdateArrangeDecapsulation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateArrangeLinkfromcomponent(CCmdUI *pCmdUI);
	afx_msg void OnArrangeLinkfromcomponent();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG	// debug version in CSQIUnitView.cpp
inline CSQIFileDoc* CSQIUnitView::GetDocument()
   { return (CSQIFileDoc*) m_pDocument; }
#endif

#endif // !__SQI_UNIT_VIEW_H__

                      
