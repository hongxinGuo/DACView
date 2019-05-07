// showview.cpp : implementation of the CFBDObjView class
//
// CFBDObjView()            : Initialize.
// ~CFBDObjView()           : Clear up.
//
// PreCreateWindow()      : Register private window class.
// OnInitialUpdate()      : Initialize Scroll size.
//
// OnDraw()               : display my objects.
// OnPreparePrinting()    : not implemented
// OnBeginPrinting()			: not implemented
// OnEndPrinting()				: not implemented
//
// ViewIn()               : Look into compound object.
// ViewOut()              : Return to upper layer.
//
// IsInRect()             : Check weither current mouse position in rect of objects.
//
// OnSize()               : process change size message.
// OnLButtonDblClk()      : Set object property.
// OnRButtonUp()
// OnCreate()             : Set timer and initialize m_sizeObjectDoc( min is (2000, 15000) )
// OnDestroy()            : Kill timer.
// OnTimer()              : Display objects that need update.
// OnLButtondown()        :
// OnLButtonMove()        :
// OnLButtonUp()          :
//
// OnUpdateViewViewin()   :
// OnUpdateViewViewout()  :
// OnViewViewin()         : View into compound.
// OnViewViewout()        : Return to upper layer.
// OnUpdateObjectBar()
// OnOptionSystemShowmessage()
// OnUpdateOptionSystemShowmessagebar()
//
// OnSetFocus()         : Create caret
// OnKillFocus()        : Destroy caret
// OnChar()             : process char input message.
// OnUpdateSystemClose(): 
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "dacview.h"
#include "MainFrm.h"

#include "globedef.h"

#include "FBDFileDoc.h"
#include "FBDObjView.h"

#include "CObjGraph.h"
#include "CObjSymbol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum {  SELECTED         = 0, // prepare to change object's form
        TRACE_MOUSE_MOVE = 1,
        PRE_SELECT       = 16,// no object selected
        };

/////////////////////////////////////////////////////////////////////////////
// CFBDObjView

IMPLEMENT_DYNCREATE(CFBDObjView, CScrollView)

BEGIN_MESSAGE_MAP(CFBDObjView, CScrollView)
  //{{AFX_MSG_MAP(CFBDObjView)
  ON_WM_SIZE()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_LBUTTONUP()
  ON_WM_RBUTTONUP()
  ON_WM_CREATE()
  ON_WM_DESTROY()
  ON_WM_TIMER()
  ON_WM_LBUTTONDOWN()
  ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWIN, OnUpdateViewViewin)
  ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWOUT, OnUpdateViewViewout)
  ON_COMMAND(ID_VIEW_VIEWIN, OnViewViewin)
  ON_COMMAND(ID_VIEW_VIEWOUT, OnViewViewout)
  ON_WM_MOUSEMOVE()
	ON_UPDATE_COMMAND_UI(ID_OBJECT_BRUSH, OnUpdateObjectBar)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_COMMAND(ID_VIEW_STATUS, OnViewStatus)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS, OnUpdateViewStatus)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_COMPOUND, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_DYNAMICLINK, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_ERASE, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_FILLBAR, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_GAUGE, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_GRAPH, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_KNOB, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_LINE, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_METER, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_OVAL, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_PAINT, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_PEN, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_PUSHBUTTON, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_RECT, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_ROUNDRECT, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_SLIDER, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_TEXT, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_NUMBERINPUT, OnUpdateObjectBar)
	//}}AFX_MSG_MAP
  // Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFBDObjView construction/destruction
extern CDacviewApp theApp;

CFBDObjView::CFBDObjView()
{
  // TODO: add construction code here
  m_sizeObjectDoc = gl_sizeDoc;

  m_pCObjectCurrent = nullptr;
  m_pCObjectComponent = nullptr;
  m_uIntoObjectComponent = 0;
	m_fCreateMemoryDC = FALSE;

  VERIFY(m_hCursorComponent = theApp.LoadCursor("COMPOUND"));
  
  // get the normal cursor
  m_hCursorNormal = SetCursor( m_hCursorComponent );
  SetCursor( m_hCursorNormal );
  m_nCurrentFunction = PRE_SELECT;

  m_ulShowCaret = 0;
  VERIFY(m_bmpCaret.LoadBitmap(IDB_BITMAP_CARET_INSERT));

  m_ptCurrentScrollPosition.x = 0;
  m_ptCurrentScrollPosition.y = 0;

}

CFBDObjView::~CFBDObjView() {
  
} 

BOOL CFBDObjView::PreCreateWindow(CREATESTRUCT& cs)
{
  ASSERT(cs.lpszClass == nullptr);       // must not be specified
  CView::PreCreateWindow( cs );
  cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
    0, // no class cursor
    (HBRUSH)(COLOR_WINDOW+1),
    AfxGetApp()->LoadIcon(IDR_STRATEGYTYPE));

  return TRUE;
}

void CFBDObjView::OnInitialUpdate() {
  SetScrollSizes( MM_TEXT, m_sizeObjectDoc );
} 

/////////////////////////////////////////////////////////////////////////////
// CFBDObjView drawing

/* 
  display m_CObjectList's all object to pdc, head is the first ( means back).
*/

void CFBDObjView::OnDraw(CDC* pDC)
{
  OnPrepareDC(pDC); 
  // TODO: add draw code here
  
  pDC->GetClipBox(&m_crectViewClip);
  pDC->SetBkColor( RGB(192, 192, 192) );
  
  CObjectBase * pcobjTemp;
  CRect rectTemp, rectTemp2, rectTemp3;

  auto it = m_pCObjectListCurrent->end();
  do {       
    it--;
    pcobjTemp = *it;
    rectTemp = pcobjTemp->GetSize();
    rectTemp |= pcobjTemp->GetLastSize();
    rectTemp2 = rectTemp & m_crectViewClip;
    if ( !rectTemp2.IsRectEmpty() ) {
      pcobjTemp->SetUpdateFlag( TRUE );
    }
  } while (it != m_pCObjectListCurrent->begin());
}

/////////////////////////////////////////////////////////////////////////////
// CFBDObjView printing

BOOL CFBDObjView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

void CFBDObjView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add extra initialization before printing
}

void CFBDObjView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFBDObjView diagnostics

#ifdef _DEBUG
void CFBDObjView::AssertValid() const
{
  CScrollView::AssertValid();
}

void CFBDObjView::Dump(CDumpContext& dc) const
{
  CScrollView::Dump(dc);
}

CFBDFileDoc* CFBDObjView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFBDFileDoc)));
  return (CFBDFileDoc*) m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShoeView member function

void CFBDObjView::ViewIn(CObjectBase * pCObject) {
  CPoint pt(0,0);

  ASSERT( pCObject->CanViewIn() );
  ((CObjectComponent *)pCObject)->SetUpperObjectComponent( m_pCObjectComponent );   // reserve upper compound
  m_pCObjectComponent = (CObjectComponent *)pCObject;         // change to this compound  
  m_pCObjectComponent->SetUpperObjectList( m_pCObjectListCurrent );    // reserve upper list
  m_pCObjectComponent->SetUpperScrollPosition(GetScrollPosition());// reserve offset point
  m_pCObjectListCurrent = m_pCObjectComponent->GetObjectList();         
  ScrollToPosition( pt );     // set to origin point
  
  Invalidate();
}

void CFBDObjView::ViewOut( void ) {
  m_pCObjectListCurrent = m_pCObjectComponent->GetUpperObjectList();
  ScrollToPosition( m_pCObjectComponent->GetUpperScrollPosition() );
  m_pCObjectCurrent = m_pCObjectComponent;
  m_nCurrentFunction = SELECTED;
  m_pCObjectComponent = m_pCObjectComponent->GetUpperObjectComponent();

  Invalidate();
}

//////////////////////////////////////////////////////////////////
//
// IsInRect()
//
// Return : 
//    TRUE if in a rect, pcobj point to the object.
//    FALSE if point is blank area, pcobj point to nullptr.
//
// Parameter :
//      POINT const pt        : the device position of mouse.
//      CObjectBase *& pcobj  : the return pointer of selected object.
//
// Description :
//   check whether 'pt' is in a 'rect' of m_pCObjectListCurrent->
//   return TRUE if found, pcobj is the object. 
//
//////////////////////////////////////////////////////////////////////  
bool CFBDObjView::IsInRect( POINT const pt, CObjectBase* & pobj ) {
  CObjectBase* pc;
  
  if (m_pCObjectListCurrent->size() == 0) {
    pobj = nullptr;
    return false;
  }

  auto it = m_pCObjectListCurrent->end(); // Tail position is the top most
  
  do {            
    it--;
    pc = *it;
    if ( pc->InIt(pt, -1) ) {
      pobj = pc;
      return(true);
    }
  } while (it != m_pCObjectListCurrent->begin());
  pobj = nullptr; // 没有找到 
  return(false);
}            
                 
/////////////////////////////////////////////////////////////////////////////
// CFBDObjView message handlers

//////////////////////////////////////////////////////////////////////////////
//
//  when change window's size, I need process two things:
//  1 : change the scroll size
//  2 : tell the draw function the update whole window
//
//////////////////////////////////////////////////////////////////////////////
void CFBDObjView::OnSize(UINT nType, int cx, int cy)  {
  CView::OnSize(nType, cx, cy);

  // TODO: Add your message handler code here
  SetScrollSizes(MM_TEXT, m_sizeObjectDoc);
  Invalidate();
}

void CFBDObjView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CPoint ptDevice = GetDeviceScrollPosition();
  
  // set property
  ptDevice += point;
  if ( IsInRect(ptDevice, m_pCObjectCurrent) ) {  // select object to process  
    // change object's property 
    ASSERT( m_pCObjectCurrent != nullptr );
    m_pCObjectCurrent->SetProperty();
    m_nCurrentFunction = SELECTED;
    Invalidate();
  }
  else { 
    // do nothing
  }
  CView::OnLButtonDblClk(nFlags, point);
}


void CFBDObjView::OnRButtonUp(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  
  CView::OnRButtonUp(nFlags, point);
}

int CFBDObjView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CScrollView::OnCreate(lpCreateStruct) == -1)
    return -1;
  
  // TODO: Add your specialized creation code here
  ((CMainFrame *)theApp.m_pMainWnd)->InShowView();
  m_pCObjectListTop = m_pCObjectListCurrent = GetDocument()->GetObjectList();
    
  m_sizeObjectDoc = gl_sizeDoc;
  
  m_nMsgTimerID = SetTimer(1, 100, nullptr);
  
  return 0;
}

void CFBDObjView::OnDestroy()
{
  CScrollView::OnDestroy();
  
  // TODO: Add your message handler code here

  KillTimer(m_nMsgTimerID);
}

///////////////////////////////////////////////////////////////////////////////
//
// This is the actual screen update function,
// Update screen 100ms per time
// 64位函数需要把UINT改为UINT_PTR,只有OnTimer函数需要这么改。
// 
//////////////////////////////////////////////////////////////////////////////
void CFBDObjView::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: Add your message handler code here and/or call default
  CDC * pdc;
  pdc = GetDC();
  OnPrepareDC( pdc );
          
  CRect rectTemp, rectThis;
  ULONGLONG ulTick;
  CBrush cbb;
	CBitmap * pOldBitmap;
	CBrush cb;
  CPoint pt( 0, 0 ), ptScroll = GetScrollPosition();

  ulTick = GetTickCount64();

	// create memory DC
  if ( !m_fCreateMemoryDC ) {
    m_MemoryDC.CreateCompatibleDC( pdc );
    m_Bitmap.CreateCompatibleBitmap( pdc, gl_sizeDoc.cx, gl_sizeDoc.cy);
    pOldBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
    cb.CreateSolidBrush( gl_clrBkGrd );
		rectThis.left = rectThis.top =0;
		rectThis.right = gl_sizeDoc.cx;
		rectThis.bottom = gl_sizeDoc.cy;
    m_MemoryDC.FillRect(rectThis, &cb);
		m_MemoryDC.SelectObject( pOldBitmap );
    m_fCreateMemoryDC = TRUE;
  }
	
	pOldBitmap = m_MemoryDC.SelectObject( &m_Bitmap );

	// clear old graph
  cbb.CreateSolidBrush( gl_clrBkGrd );
  for (const auto pcobj : *m_pCObjectListCurrent ) {
    rectThis = pcobj->GetLastSize();
    m_MemoryDC.FillRect( rectThis, & cbb );
  }
  
	// draw current graph
  for (const auto pcobj : *m_pCObjectListCurrent) {
		if ( !pcobj->IsTransparent() ) {
			pcobj->ToShowDynamic( &m_MemoryDC );
		}
  }
  pdc->BitBlt(0, 0, m_sizeObjectDoc.cx, m_sizeObjectDoc.cy, 
              &m_MemoryDC, 0, 0, SRCCOPY);

	m_MemoryDC.SelectObject( pOldBitmap );

	ReleaseDC( pdc );
          
  gl_ulSystemProcessTime += GetTickCount64() - ulTick;

  CScrollView::OnTimer(nIDEvent);
}

void CFBDObjView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  // TODO: Add your message handler code here and/or call default
  CDC * pdc = GetDC();
  OnPrepareDC( pdc );
        
  CObjectBase* pc;
  auto it = m_pCObjectListCurrent->end(); // Tail position is the top most
	INT_PTR i;
  CPoint ptDevice, ptOffset = GetDeviceScrollPosition();  
  BOOL fFind = FALSE;
  
  ptDevice = ptOffset + point;    // set current mouse address  
  i = 0;
  while ( (!fFind) && (it != m_pCObjectListCurrent->begin()) ) {
    pc = *--it;
    if ( pc->InIt(ptDevice, 1) ) {
      m_pCObjectCurrent = pc;
      fFind = TRUE;
      m_nCurrentFunction = TRACE_MOUSE_MOVE;
    }
  }    
  ReleaseDC( pdc );
  if ( !fFind ) {
    m_pCObjectCurrent = nullptr;
    m_nCurrentFunction = PRE_SELECT;
  }

  CScrollView::OnLButtonDown(nFlags, point);
}

void CFBDObjView::OnMouseMove(UINT nFlags, CPoint point) 
{
  // TODO: Add your message handler code here and/or call default

  CDC * pdc;
  CObjectBase* pc;
  auto it = m_pCObjectListCurrent->end(); // Tail position is the top most
	INT_PTR i;
  CPoint ptDevice, ptOffset = GetDeviceScrollPosition();  
  BOOL fFind = FALSE;
  CString str = " ";
	CObjectBase * pObj = nullptr;

  ptDevice = ptOffset + point;
  switch ( m_nCurrentFunction ) {
  case PRE_SELECT :
    ASSERT( m_pCObjectCurrent == nullptr );
    if ( IsInRect( ptDevice, pObj ) ) {
      // show current object's name on status bar
      str = pObj->GetClassNameStr();
      str += "->";
      str += pObj->GetName();
		}
    SetStatusBarMessage( str );
    SetCursor( m_hCursorNormal );
    break;
  case TRACE_MOUSE_MOVE :
    pdc = GetDC();
    OnPrepareDC( pdc );
        
    i = 0;
    while ( (!fFind) && (it != m_pCObjectListCurrent->begin()) ) {
      pc = *--it;
      pc->InIt(ptDevice, 0);
    }    
    ReleaseDC( pdc );
  
    if ( IsInRect( ptDevice, m_pCObjectCurrent ) ) {
      if ( m_pCObjectCurrent->IsKindOf(RUNTIME_CLASS( CObjectComponent )) ) {
        SetCursor( m_hCursorComponent );
      }
      else {
        
      }
    }
    else {
      SetCursor( m_hCursorNormal );
    }
    break;
  case SELECTED :
    SetCursor( m_hCursorNormal );
    break;
  } // switch

  CScrollView::OnMouseMove(nFlags, point);
}

void CFBDObjView::OnLButtonUp(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CDC * pdc = GetDC();
  OnPrepareDC( pdc );
        
  CObjectBase* pc;
  auto it = m_pCObjectListCurrent->end(); // Tail position is the top most
	INT_PTR i;
  CPoint ptDevice, ptOffset = GetDeviceScrollPosition();  
  BOOL fFind = FALSE;
  
  ptDevice = ptOffset + point;    // set current mouse address  
  i = 0;
  while ( m_ulShowCaret > 0 ) {
    HideCaret();
    m_ulShowCaret--;
  }
  while ( (!fFind) && (it != m_pCObjectListCurrent->begin()) ) {
    pc = *--it;
    if ( pc->InIt(ptDevice, 2) ) {
      m_pCObjectCurrent = pc;
      fFind = TRUE;
      m_nCurrentFunction = SELECTED;
      if ( m_pCObjectCurrent->NeedShowCaret() ) {
        // adjust caret position, must return TRUE
        VERIFY(m_pCObjectCurrent->AdjustCaretPos( pdc, ptDevice ));
        SetCaretPos( point );
        ShowCaret();
        m_ulShowCaret++;
      }
    }
  }    
  if ( !fFind ) {
    m_pCObjectCurrent = nullptr;
    m_nCurrentFunction = PRE_SELECT;
  }
  ReleaseDC( pdc );

  CView::OnLButtonUp(nFlags, point);
}

void CFBDObjView::OnUpdateViewViewin(CCmdUI* pCmdUI) 
{
  // TODO: Add your command update UI handler code here
  if ( m_pCObjectCurrent != nullptr ) {
    if ( m_pCObjectCurrent->CanViewIn() ) {
      pCmdUI->Enable(TRUE);
    }
    else {
      pCmdUI->Enable(FALSE);
    }
  }
  else {
    pCmdUI->Enable(FALSE);
  }
}

void CFBDObjView::OnUpdateViewViewout(CCmdUI* pCmdUI) 
{
  // TODO: Add your command update UI handler code here
  if ( m_uIntoObjectComponent ) {
    pCmdUI->Enable(TRUE);
  }
  else {
    pCmdUI->Enable(FALSE);
  }
}

void CFBDObjView::OnViewViewin() 
{
  // TODO: Add your command handler code here
  CRect rectThis;
	CBrush cb;

	ViewIn(m_pCObjectCurrent);
  m_uIntoObjectComponent++;   // goto inner
  m_pCObjectCurrent = nullptr;     // set current process object to nullptr
	CBitmap * pOldBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
  cb.CreateSolidBrush( gl_clrBkGrd );
	rectThis.left = rectThis.top =0;
	rectThis.right = m_sizeObjectDoc.cx;
	rectThis.bottom = m_sizeObjectDoc.cy;
  m_MemoryDC.FillRect(rectThis, &cb);
	m_MemoryDC.SelectObject( pOldBitmap );
}

void CFBDObjView::OnViewViewout() 
{
  // TODO: Add your command handler code here
  if ( m_uIntoObjectComponent > 0 ) {
    ViewOut();
    m_uIntoObjectComponent--;
  }
	CRect rectThis;
	CBrush cb;

	CBitmap * pOldBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
  cb.CreateSolidBrush( gl_clrBkGrd );
	rectThis.left = rectThis.top =0;
	rectThis.right = m_sizeObjectDoc.cx;
	rectThis.bottom = m_sizeObjectDoc.cy;
  m_MemoryDC.FillRect(rectThis, &cb);
	m_MemoryDC.SelectObject( pOldBitmap );
}

void CFBDObjView::OnUpdateObjectBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(FALSE);
}

void CFBDObjView::OnSetFocus(CWnd* pOldWnd) 
{
	CScrollView::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	//CreateCaret( &m_bmpCaret );
}

void CFBDObjView::OnKillFocus(CWnd* pNewWnd) 
{
	CScrollView::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	while ( m_ulShowCaret > 0 ) {
    HideCaret();
    m_ulShowCaret--;
  }
  //DestroyCaret();
}

void CFBDObjView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CDC * pdc;
  CPoint pt, ptScrollPosition = GetScrollPosition();

  if ( m_pCObjectCurrent != nullptr ) {
    if ( m_pCObjectCurrent->ProcessChar( nChar, pt ) ) {
      pdc = GetDC();
      OnPrepareDC( pdc );
      pt -= ptScrollPosition;
      SetCaretPos( pt );
      ShowCaret();
      m_pCObjectCurrent->ToShowDynamic( pdc );
      ReleaseDC( pdc );
    }
  }

	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}

void CFBDObjView::OnViewStatus() 
{
	// TODO: Add your command handler code here
	GetDocument()->ToggleViewStatus( m_pCObjectCurrent );
}

void CFBDObjView::OnUpdateViewStatus(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if ( m_pCObjectCurrent != nullptr ) {
		pCmdUI->Enable(TRUE);
	}
	else {
		pCmdUI->Enable(FALSE);
	}
}
