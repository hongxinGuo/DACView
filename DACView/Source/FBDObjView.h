// showview.h : interface of the CFBDObjView class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once 

#include "CObjComponent.h"
 
class CFBDObjView : public CScrollView
{
protected: // create from serialization only
	CFBDObjView();
	DECLARE_DYNCREATE(CFBDObjView)

// Attributes
public:
	CFBDFileDoc* GetDocument();

// Operations
protected:
	  
// Implementation
public:
	virtual 		    ~CFBDObjView();
	BOOL 				    PreCreateWindow(CREATESTRUCT& cs);
	virtual void    OnInitialUpdate();
	virtual void    OnDraw(CDC* pDC);  // overridden to draw this view
	
	void 				    ViewIn( CObjectBase * pcObject );
	void 				    ViewOut( void );
	bool				    IsInRect( POINT const pt, CObjectBase *& pobj );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Printing support
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

protected:
	UINT 							m_nTimerID;
	UINT_PTR					m_nMsgTimerID;
	UINT							m_nTimerRes;
	
	int 							m_nCurrentFunction;	// DragDrop, TraceMouse, ect.
	
	CObjectList *     m_pCObjectListCurrent;
	CObjectList *	    m_pCObjectListTop;
	CSize							m_sizeObjectDoc;
	CRect							m_crectViewClip;

	CObjectBase * 		m_pCObjectCurrent;
	UINT							m_uIntoObjectComponent;
	CObjectComponentBase *	m_pCObjectComponent;	// point to current porcess ObjectComponent
	CPoint						m_ptCurrentScrollPosition;

	HCURSOR						m_hCursorComponent;
	HCURSOR						m_hCursorNormal;

  ULONG             m_ulShowCaret;
	CBitmap           m_bmpCaret;

	BOOL							m_fCreateMemoryDC;
	CDC								m_MemoryDC;
	CBitmap						m_Bitmap;

// Generated message map functions
protected:
	//{{AFX_MSG(CFBDObjView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateViewViewin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewViewout(CCmdUI* pCmdUI);
	afx_msg void OnViewViewin();
	afx_msg void OnViewViewout();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnUpdateObjectBar(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewStatus();
	afx_msg void OnUpdateViewStatus(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG	// debug version in showview.cpp
inline CFBDFileDoc* CFBDObjView::GetDocument()
   { return (CFBDFileDoc*) m_pDocument; }
#endif


