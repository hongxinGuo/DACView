// dacvivw.h : interface of the CSQIObjectView class
//
// ����Objects����ʾ
//
//
///////////////////////////////////////////////////////////////////////////// 
#pragma once

#include "cobjectbase.h"
#include "cObjComponentBase.h"

#include "SQIFileDoc.h"

// SQIObjectView��״̬����
// SQIObjectView��״̬�������֣��Ƚϼ򵥡�
//
//
enum {
  OBJECT_SELECTED = 0, // prepare to change object's form
  MAKE_SYMBOL = 4, // group objects into symbol 
  CREATE_OBJ_DYN_LINK = 5, // create dynamic link
  OBJECT_PRE_SELECT = 16,// no object selected
};

class CSQIObjectView : public CScrollView
{
protected: // create from serialization only
	CSQIObjectView();
	DECLARE_DYNCREATE(CSQIObjectView)

// Attributes
public:
	CSQIFileDoc* GetDocument();
     
// Operations
public:
  bool AddObject(CObjectBase * const pCObjectBase);
	INT_PTR  GetObjectsNumber( void ); 
	bool DeleteDynLinkObject( CObjectDynLink * pCObjectDynLink ) ;
	bool DeleteObject( CObjectBase * pCObjectBase );
	bool IsInRect( POINT const pt, CObjectBase* & pcobj );   
	
	bool ObjectToBack( CObjectBase * const pCObjectBase );		// change an object's position to ObjList's head	
	bool ObjectToFront( CObjectBase * const pCObjectBase );		// change an object's position to ObjList's Tail
	int  MakeSymbol( CObjectSymbol * pcobjSymbol );						// ��һ��Objects���һ��Symbol
	int  BreakSymbol( CObjectSymbol * pcobjSymbol );					// ��һ��Symbol���
    
  void CreateUniName( CObjectBase * pCObject );							// ΪObjectѡ��Ψһ������

  void CenterAlign( void );
  void LeftAlign( void );
  void RightAlign( void ); 
  
  CSize GetDocSize( void );

  void ClearAllSelect( void );
  void ClearAllFocus( void );

	void ViewIn( CObjectComponentBase * pcObject );
	void ViewOut( void );
  
// Implementation
protected:
	virtual ~CSQIObjectView();
	BOOL 		PreCreateWindow(CREATESTRUCT& cs);
	void 		OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void OnUpdate( void );
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	
#endif

// Printing support
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);  
	// virtual void UpdateScrollSizes();
	
protected: // selected object data nember          
	int 							m_nCurrentFunction;		// ��ǰ״̬��DragDrop, TraceMouse, ect.
	CPoint 						m_ptStart;						// used in trackMouse and select object to move
	CPoint						m_ptEnd;
	CRect 						m_rectCurrent;				// current selected object's rect area 
	CRect 						m_rectStart;
	UINT 							m_nCurrentObjNumber;	// trace object's number, used for temperary name 
	CObjectBase * 		m_pCObjectCurrent; 		// point to current selected object, nullptr if no selected object
	CObjectBase * 		m_pCObjectOther;			// used in group object
	CObjectBase *			m_pCObjectMouseMove;	// ����ƶ�ʱ�����λ��һ��Object�ڣ������ô�ָ��
	HCURSOR						m_hCursorComponent;
	HCURSOR						m_hCursorNormal;
	
	CObjectList *     m_pCObjectListCurrent;

	UINT							m_uIntoObjectComponent;	// ���ڼ�¼����CObjectComponent�Ĳ�����0ʱ��������㣨û�н���CObjectComponent����2ʱ���ڵڶ���CObjectComponent��
	CObjectComponentBase *	m_pCObjectComponentUpper;	// ָ��ǰ���������ϲ�ObjectComponent�����ʱΪnullptr��
	CPoint						m_ptCurrentScrollPosition;
  CRect 						m_crectObjectClip;

	static UINT 			m_uObjectFormat;				// my clipboard format
	HGLOBAL						hData;									// ���ڼ��а�����ݴ���
protected:	
	
// Generated message map functions
protected:
	//{{AFX_MSG(CSQIObjectView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnArrangeTofront();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnEditDelete();
	afx_msg void OnArrangeToback();
	afx_msg void OnArrangeMakesymbol();
	afx_msg void OnArrangeBreaksymbol();
	afx_msg void OnStyleCentered();
	afx_msg void OnStyleLeft();
	afx_msg void OnStyleRight();
	afx_msg void OnUpdateArrangeBreakSymbol(CCmdUI* pCmdUI);
	afx_msg void OnUpdateArrangeMakeSymbol(CCmdUI* pCmdUI);
	afx_msg void OnUpdateArrangeToback(CCmdUI* pCmdUI);
	afx_msg void OnUpdateArrangeTofront(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStyleCentered(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStyleLeft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStyleRight(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateViewViewin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewViewout(CCmdUI* pCmdUI);
	afx_msg void OnViewViewin();
	afx_msg void OnViewViewout();
	afx_msg void OnUpdateArrangeBreakdynlink(CCmdUI* pCmdUI);
	afx_msg void OnArrangeBreakdynlink();
	afx_msg void OnViewProperty();
	afx_msg void OnUpdateViewProperty(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnArrangeMakedynlink();
	afx_msg void OnUpdateArrangeMakedynlink(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG	// debug version in sequevw.cpp
inline CSQIFileDoc* CSQIObjectView::GetDocument()
   { return (CSQIFileDoc*) m_pDocument; }
#endif

