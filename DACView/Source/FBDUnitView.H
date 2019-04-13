// runstrat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFBDUnitView view
#pragma once

#include "FBDFileDoc.h"

#include "CUnitComponent.h"

class CFBDUnitView : public CScrollView
{
	DECLARE_DYNCREATE(CFBDUnitView)
protected:
	CFBDUnitView();			// protected constructor used by dynamic creation

// Attributes
public:
	CFBDFileDoc* GetDocument();

// Operations
public:

protected:
	CUnitBase * FindUnit( CString TagName );
  CSize       GetUnitDocSize( void );
  void 	      SetStrategyClipRect( CRect rectClip );
	BOOL        IsInRect( POINT const pt, CUnitBase* & pcobj );

private:
  
protected:
	UINT_PTR						m_uIdTimer;

	CUnitBase * 				m_pCUnitCurrent;

	UINT								m_uIntoComponentProcess;
  CUnitList * 		    m_pCUnitListCurrent;// point to current process CUnitList
  CUnitComponent * 		m_pCUnitComponentCurrent;		// point to current process UnitComponent
  CPoint							m_ptCurrentScrollPosition;
  ULONG								m_ulUnitListCurrentCount;
	CSize               m_sizeUnitDoc;
	CRect	              m_crectStrategyClip;
 
// Implementation
protected:
	virtual ~CFBDUnitView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate( void );
  virtual void OnUpdate( void );
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFBDUnitView)
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnViewViewin();
	afx_msg void OnViewViewout();
	afx_msg void OnUpdateViewViewin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewViewout(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateSCClose(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjectBar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewStatus(CCmdUI* pCmdUI);
	afx_msg void OnViewStatus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; 

#ifndef _DEBUG	// debug version in showview.cpp
inline CFBDFileDoc* CFBDUnitView::GetDocument( void )
   { return (CFBDFileDoc*) m_pDocument; }
#endif


