//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// dacvivw.cpp : implementation of the CSQIObjectView class
//
// Functions :
//
// CSQIObjectView()           : Initialize variables
// ~CSQIObjectView()          : 
// PreCreateWindow()        : Register private window class
// OnInitialUpdate()        : Set up document size
// OnUpdate()               : Set up document size
// OnDraw()                 : Show object on DC
//
// GetDocument()            : Get document pointer
// GetDocSize()							: Get 
//
//
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
                              
#include "Dacview.h"
#include "MainFrm.h"

#include "SQIFileDoc.h"
#include "SQIObjView.h"

#include "cObjText.h"

#include "cObjectRect.h"
#include "cObjRoundRect.h"
#include "cObjOval.h" 

#include "cObjline.h"

#include "cObjsymbol.h"
#include "cObjComponent.h"

#include "cObjGraph.h" 
#include "cObjGauge.h"
#include "cObjButton.h"
#include "cObjSlider.h"
#include "cObjKnob.h"
#include "cObjMeter.h"
#include "cObjFillBar.h"
#include "cObjNumInput.h"
#include "CObjText.h"

#include "DlgObjUnitDynLink.h"

#include "globedef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

/////////////////////////////////////////////////////////////////////////////
// CSQIObjectView

IMPLEMENT_DYNCREATE(CSQIObjectView, CScrollView)

BEGIN_MESSAGE_MAP(CSQIObjectView, CScrollView)
  //{{AFX_MSG_MAP(CSQIObjectView)
  ON_WM_LBUTTONDOWN()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONUP()
  ON_COMMAND(ID_ARRANGE_TOFRONT, OnArrangeTofront)
  ON_WM_LBUTTONDBLCLK()
  ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
  ON_COMMAND(ID_ARRANGE_TOBACK, OnArrangeToback)
  ON_COMMAND(ID_ARRANGE_MAKESYMBOL, OnArrangeMakesymbol)
  ON_COMMAND(ID_ARRANGE_BREAKSYMBOL, OnArrangeBreaksymbol)
  ON_COMMAND(ID_STYLE_CENTERED, OnStyleCentered)
  ON_COMMAND(ID_STYLE_LEFT, OnStyleLeft)
  ON_COMMAND(ID_STYLE_RIGHT, OnStyleRight)
  ON_UPDATE_COMMAND_UI(ID_ARRANGE_BREAKSYMBOL, OnUpdateArrangeBreakSymbol)
  ON_UPDATE_COMMAND_UI(ID_ARRANGE_MAKESYMBOL, OnUpdateArrangeMakeSymbol)
  ON_UPDATE_COMMAND_UI(ID_ARRANGE_TOBACK, OnUpdateArrangeToback)
  ON_UPDATE_COMMAND_UI(ID_ARRANGE_TOFRONT, OnUpdateArrangeTofront)
  ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
  ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
  ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
  ON_COMMAND(ID_EDIT_CUT, OnEditCut)
  ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
  ON_UPDATE_COMMAND_UI(ID_STYLE_CENTERED, OnUpdateStyleCentered)
  ON_UPDATE_COMMAND_UI(ID_STYLE_LEFT, OnUpdateStyleLeft)
  ON_UPDATE_COMMAND_UI(ID_STYLE_RIGHT, OnUpdateStyleRight)
  ON_WM_SIZE()
  ON_WM_CREATE()
  ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWIN, OnUpdateViewViewin)
  ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWOUT, OnUpdateViewViewout)
  ON_COMMAND(ID_VIEW_VIEWIN, OnViewViewin)
  ON_COMMAND(ID_VIEW_VIEWOUT, OnViewViewout)
  ON_UPDATE_COMMAND_UI(ID_ARRANGE_BREAKDYNLINK, OnUpdateArrangeBreakdynlink)
  ON_COMMAND(ID_ARRANGE_BREAKDYNLINK, OnArrangeBreakdynlink)
	ON_COMMAND(ID_VIEW_PROPERTY, OnViewProperty)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTY, OnUpdateViewProperty)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_ARRANGE_MAKEDYNLINK, OnArrangeMakedynlink)
	ON_UPDATE_COMMAND_UI(ID_ARRANGE_MAKEDYNLINK, OnUpdateArrangeMakedynlink)
	//}}AFX_MSG_MAP
  // Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSQIObjectView construction/destruction
extern CDacviewApp theApp;
UINT CSQIObjectView::m_uObjectFormat = 0;

CSQIObjectView::CSQIObjectView() {
  // TODO: add construction code here 
  static BOOL fRegistered = FALSE;
  
  // register clipboard format, only once
  if ( !fRegistered ) {  // register my clipboard format
    m_uObjectFormat = RegisterClipboardFormat((LPCSTR)"CF_DACVIEW_VIEW_FORMAT");
    fRegistered = TRUE;
  }                        
  // reset variables
  m_nCurrentFunction = OBJECT_PRE_SELECT; 
  m_ptStart.x = m_ptStart.y = m_ptEnd.x = m_ptEnd.y = 0;
  m_ptCurrentScrollPosition.SetPoint(0, 0);
  m_rectCurrent.SetRectEmpty();
  m_rectStart.SetRectEmpty();
  m_crectObjectClip.SetRectEmpty();
  m_pCObjectCurrent = m_pCObjectOther = nullptr;
	m_pCObjectMouseMove = nullptr;
  m_nCurrentObjNumber = 1;            

  m_pCObjectComponentUpper = nullptr; // 初始状态时外层ObjectComponent为空指针
  m_uIntoObjectComponent = 0;  
  m_pCObjectListCurrent = nullptr;

	VERIFY(m_hCursorComponent = theApp.LoadCursor("COMPOUND"));
	// get the normal cursor
	m_hCursorNormal = SetCursor( m_hCursorComponent );
	SetCursor( m_hCursorNormal );

  m_ptCurrentScrollPosition.x = 0;
  m_ptCurrentScrollPosition.y = 0;

}

CSQIObjectView::~CSQIObjectView() 
{
} 

///////////////////////////////////////////////////////////////
//
// PreCreateWindow()
//
// Parameter :
//    CREATESTRUCT& cs    : window create structure
// Return :
//    BOOL            : Always TRUE
//
// Description :
//    Register dacview view own window class
//
////////////////////////////////////////////////////////////////
BOOL CSQIObjectView::PreCreateWindow(CREATESTRUCT& cs)
{
  ASSERT(cs.lpszClass == nullptr);       // must not be specified
  CView::PreCreateWindow( cs );
  cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
    0,                                // no class cursor
    (HBRUSH)(COLOR_WINDOW+1),
    AfxGetApp()->LoadIcon(IDR_VIEWTYPE));
  
  return TRUE;
}

// CSQIObjectView operation

////////////////////////////////////////////////////////////////
//
// OnInitialUpdate()
//
// Parameter : no
//
// Return : no
//
// Description :
//    Initialize document's size
//
/////////////////////////////////////////////////////////////////
void CSQIObjectView::OnInitialUpdate() {
  SetScrollSizes( MM_TEXT, GetDocSize( ) );
  OnUpdate();
} 
  
////////////////////////////////////////////////////////////////
//
// OnUpdate()
//
// Parameter : no
//
// Return : no
//
// Description :
//    Set up document's size
//
/////////////////////////////////////////////////////////////////
void CSQIObjectView::OnUpdate( )
{
   // ...
   // Implement a GetDocSize( ) member function in 
   // your document class; it returns a CSize.
   SetScrollSizes( MM_TEXT, GetDocSize( ) );
   ResizeParentToFit( );   // Default bShrinkOnly argument
   // ...
}            

/////////////////////////////////////////////////////////////////////////////
// CSQIObjectView drawing

////////////////////////////////////////////////////////////
//
// OnDraw()
// 
// Parameter :
//    CDC * pDC     : point to DC
//
// Return : No
//
// Description :
//    Draw Grid and show all objects that need update.
//    Show unit file name on title
//
///////////////////////////////////////////////////////////////
void CSQIObjectView::OnDraw(CDC* pDC)
{ 
  // TODO: add draw code here 
  CRect crectClip;
	CPen p(PS_DOT, 1, RGB(64, 64, 64) ), *pp;
	
	pDC->SetBkColor( RGB(192, 192, 192) );
	pDC->GetClipBox(&crectClip);
  m_crectObjectClip = crectClip;
	pp = pDC->SelectObject( &p );
	// draw background grid
	for ( int i = 10; i <= crectClip.right; i += 10 ) {
		if ( (i <= crectClip.right) && (i >= crectClip.left) ) { 
      for ( int j = 10; j <= crectClip.bottom; j += 10 ) {
    		if ( (j <= crectClip.bottom) && (j >= crectClip.top) ) { 
  		  	pDC->SetPixel( i, j, RGB(0, 0, 0) );
    		}
  	  }
    }
	}
	pDC->SelectObject( pp );
  // display all objects
  CRect rect;
  
  for (const auto pcobj : *m_pCObjectListCurrent) {
    rect = m_crectObjectClip;
    rect &= pcobj->GetSize() + pcobj->GetOffset();
    if ( !rect.IsRectEmpty() ) {
      pcobj->ToShowStatic( pDC, m_ptCurrentScrollPosition );
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// CSQIObjectView printing

BOOL CSQIObjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

void CSQIObjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add extra initialization before printing
}

void CSQIObjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSQIObjectView diagnostics

#ifdef _DEBUG
void CSQIObjectView::AssertValid() const
{
  CView::AssertValid();
}

void CSQIObjectView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CSQIFileDoc* CSQIObjectView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSQIFileDoc)));
  return (CSQIFileDoc*) m_pDocument;
}   

#endif //_DEBUG 

////////////////////////////////////////////////////////////////////////////
//
//  get th maxium size of view. the maxium size is 1280X1024.
//
////////////////////////////////////////////////////////////////////////////
CSize CSQIObjectView::GetDocSize( void ) {
  return( gl_sizeDoc );
}

////////////////////////////////////////////////////////////////////////////
//
//  add an object to m_CObjectList's tail.
//  RETURN : TRUE if sucess, FALSE if memory overflow
//
//////////////////////////////////////////////////////////////////////////////////
BOOL CSQIObjectView::AddObject(CObjectBase * const pCObjectBase) { 
  m_pCObjectListCurrent->push_back(pCObjectBase); 
  return(TRUE);
}

INT_PTR CSQIObjectView::GetObjectsNumber( void ) {
  return( m_pCObjectListCurrent->size() );
}

///////////////////////////////////////////////////////////////////////////////////
//
// Delete a object in m_CObjectList.
// return TRUE if filfull, FALSE if not filfull.
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CSQIObjectView::DeleteObject( CObjectBase * pCObjectBase ) {
  list<CObjectBase*>::iterator it;
  
  ASSERT( pCObjectBase != nullptr );
  if ((it = find(m_pCObjectListCurrent->begin(), m_pCObjectListCurrent->end(), pCObjectBase)) != m_pCObjectListCurrent->end() ) {
    delete pCObjectBase;
		pCObjectBase = nullptr;
    m_pCObjectListCurrent->erase(it);
  }
  return ( TRUE );
}

////////////////////////////////////////////////////////////////////////////
//
//  check whether 'pt' is in a 'rect' of m_pCObjectListCurrent->
//  Return : 
//    TRUE if in a rect, pcobj point to the object.
//    FALSE if point is blank area, pcobj point to nullptr.
//
///////////////////////////////////////////////////////////////////////////
BOOL CSQIObjectView::IsInRect( POINT const pt, CObjectBase* & pcobj ) {
  CDC * pdc = GetDC();

  CObjectBase* pc;
  auto it = m_pCObjectListCurrent->end(); // Tail position is the top most
  
  do {                            
    pc = *--it;
    if ( pc->InIt( pdc, pt, -1 ) ) {
      pcobj = pc;
      ReleaseDC(pdc);
      return ( true );
    }
  } while (it != m_pCObjectListCurrent->begin());
  ReleaseDC( pdc );    
  pcobj = nullptr;
  return ( false );
}            

//////////////////////////////////////////////////////////////////////////////////////////
//
//  change an object's position to m_CObjectList's head, and redraw all objects.
// m_cObjectList是倒序存储，最前面的位于显示的最后面。
//
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CSQIObjectView::ObjectToBack( CObjectBase * const pCObjectBase ) {
  ASSERT( pCObjectBase != nullptr );
  CObjectBase * pcobj = pCObjectBase;
  list<CObjectBase *>::iterator it;

  it = find(m_pCObjectListCurrent->begin(), m_pCObjectListCurrent->end(), pCObjectBase);
  m_pCObjectListCurrent->erase(it);           // remove from current position.
  m_pCObjectListCurrent->push_front( pcobj );        // head position is the back.
  return ( true );
}
  
//////////////////////////////////////////////////////////////////////////////////
//
//  change an object's position to m_CObjectList's tail, and redraw all objects.
//
////////////////////////////////////////////////////////////////////////////////////
BOOL CSQIObjectView::ObjectToFront( CObjectBase * const pCObjectBase ) {   
  ASSERT( pCObjectBase != nullptr );
  CObjectBase * pcobj = pCObjectBase;
  list<CObjectBase *>::iterator it;

  it = find(m_pCObjectListCurrent->begin(), m_pCObjectListCurrent->end(), pCObjectBase);
  m_pCObjectListCurrent->erase(it);           // remove from current position.
  m_pCObjectListCurrent->push_back(pcobj);        // head position is the back.
  return ( TRUE );
} 

/////////////////////////////////////////////////////////////////////////////////////////
//  group selectedobjects in symbol and then delete them.
//  before function :
//    m_CObjectList->( a, b, c, d, e, f, g, h, i);
//                     |     |     |     |
//                     ---------------------------> selected objects to make pcObjSymbol
//  after function :
//    m_CObjectList->( b, d, f, h, i, pcObjSymbol->(a, c, e, g));
//
//////////////////////////////////////////////////////////////////////////////////////////
int CSQIObjectView::MakeSymbol( CObjectSymbol * pcObjSymbol ) {
  CRect rectTemp, rect;
  
  // add all selected objects into symbol              
  rectTemp.SetRectEmpty();                
  for (const auto pcobj : *m_pCObjectListCurrent) {
    if ( pcobj->IsSelect() ) {    // if selected
      pcObjSymbol->InsertObject( pcobj ); // 将此Object加入Symbol中
      pcobj->SetSymbolThatHaveMe( pcObjSymbol ); // 
      rectTemp |= pcobj->GetSize();			// 显示区域是所有区域的总和
    }
  }
  // remove all selected objects
  for (const auto pcobj : *m_pCObjectListCurrent) {
    if ( pcobj->IsSelect() ) {    // if selected 
      pcobj->SetSelect( FALSE );
      rect = pcobj->GetSize();
      rect -= rectTemp.TopLeft();
      pcobj->SetAllSize( rect );
      m_pCObjectListCurrent->erase(find(m_pCObjectListCurrent->begin(), m_pCObjectListCurrent->end(), pcobj) ); // 从当前list中删除此Object
    }
  }                                         
  pcObjSymbol->SetAllSize(rectTemp); // symbol's area is union of all its children
  TRACE("DacviewDoc.MakeSymbol filfulled, Objects in m_CObjectList is %d\n",m_pCObjectListCurrent->size()+1);
  return ( 0 );
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  break selected symbol in objects and then delete this symbol.
//  before operate :
//    m_CObjectList->( b, d, f, h, i, j_symbol->(a, c, e, g), h, i); 
//                                   |
//                                    -----------> selected object
//  after operate :
//    m_CObjectList->( b, d, f, h, i, a, c, e, g);
//                                    |  |  |  |
//                                    ------------> selected objects 
//
///////////////////////////////////////////////////////////////////////////////////////////
int CSQIObjectView::BreakSymbol( CObjectSymbol * pcobjSymbol ) {
  ASSERT( pcobjSymbol->IsKindOf(RUNTIME_CLASS(CObjectSymbol)) );
  pcobjSymbol->BreakSymbol( m_pCObjectListCurrent );
  DeleteObject(pcobjSymbol);
  TRACE("DacviewDoc.BreakSymbol filfulled, Objects in m_CObjectList is %d\n",m_pCObjectListCurrent->size());
  return ( 0 );
} 

/////////////////////////////////////////////////////////////////////
//
// clear all selected flags
//
/////////////////////////////////////////////////////////////////////
void CSQIObjectView::ClearAllSelect( void ) {
  for (const auto pcobjTemp : *m_pCObjectListCurrent) {                            
    if ( pcobjTemp->IsSelect() ) {    // if selected
      pcobjTemp->SetSelect( FALSE );
    }
  }
}

////////////////////////////////////////////////////////////////
//
//  tell ToShow to update me, and clear flag
//
////////////////////////////////////////////////////////////////
void CSQIObjectView::ClearAllFocus( void ) {
  for (const auto pcobjTemp : *m_pCObjectListCurrent) {
    if ( pcobjTemp->IsSelect() ) {    // if selected
      pcobjTemp->SetSelect( FALSE );
      InvalidateRect( pcobjTemp->GetSize() - GetScrollPosition() );
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  create an name for pCObject that different from all other objects.
//
/////////////////////////////////////////////////////////////////////////////////////////
void CSQIObjectView::CreateUniName( CObjectBase * pCObject ) {
  CObjectList listObject, * plistTop = GetDocument()->GetObjectList();
  // make a single list from graph ObjectList
  for (const auto pcObj : *plistTop) {
    pcObj->AddToList( listObject );
  }

	pCObject->CreateUniName( listObject );
}

/////////////////////////////////////////////////////////////////////////////
// CSQIObjectView commands

/////////////////////////////////////////////////////////////////////////
//
//  align slected objects by center.
//  used the backest object as standard.
//
////////////////////////////////////////////////////////////////////////////
void CSQIObjectView::CenterAlign()
{
  CObjectBase * pcobjTemp;
  auto it = m_pCObjectListCurrent->begin();                          
  CRect rectTemp;
  int w, i1;
  
  // find the first selected object.
  do {
    pcobjTemp = *it++;
    if ( pcobjTemp->IsSelect() ) {    // if selected
      rectTemp = pcobjTemp->GetSize();         
    }
  } while ( !pcobjTemp->IsSelect() );
  // calculate the center of the object             
  w = rectTemp.left + (rectTemp.right - rectTemp.left) / 2;
  // center the selected objects
  for (const auto pcobjTemp1 : *m_pCObjectListCurrent) {
    if ( pcobjTemp1->IsSelect() ) {    // if selected
      rectTemp = pcobjTemp1->GetSize();
      i1  = w - ((rectTemp.right - rectTemp.left) / 2);
      rectTemp.right = w + (rectTemp.right - rectTemp.left) - ((rectTemp.right - rectTemp.left)/2);
      rectTemp.left  = i1;
      pcobjTemp1->SetAllSize(rectTemp);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
//
//  align slected objects by left.
//  used the backest object as standard.
//
//////////////////////////////////////////////////////////////////////////////
void CSQIObjectView::LeftAlign()
{
  CObjectBase * pcobjTemp;
  auto it = m_pCObjectListCurrent->begin();                          
  CRect rectTemp;  
  int w, h;
  
  // find the first selected object
  do {
    pcobjTemp = *it++;
    if ( pcobjTemp->IsSelect() ) {    // if selected
      rectTemp = pcobjTemp->GetSize();
    }
  } while ( !pcobjTemp->IsSelect() );
  w = rectTemp.left;
  // left align selected objects
  for (const auto pcobjTemp1 : *m_pCObjectListCurrent) {
    if ( pcobjTemp1->IsSelect() ) {    // if selected
      rectTemp = pcobjTemp1->GetSize();
      h = rectTemp.right - rectTemp.left;
      rectTemp.left = w;
      rectTemp.right = h + w;
      pcobjTemp1->SetAllSize(rectTemp);
    }
  }
}
                               
////////////////////////////////////////////////////////////////////////////////////////
//
//  align slected objects by right.
//  used the backest(frontest) object as standard.
//
///////////////////////////////////////////////////////////////////////////////////////
void CSQIObjectView::RightAlign() {
  CObjectBase * pcobjTemp;
  auto it = m_pCObjectListCurrent->begin();                          

  CRect rectTemp;
  int w, h;
  
  do {
    pcobjTemp = *it++;
    if ( pcobjTemp->IsSelect() ) {    // if selected
      rectTemp = pcobjTemp->GetSize();
    }
  } while ( !pcobjTemp->IsSelect() );             
  w = rectTemp.right;
  for (const auto pcobjTemp1 : *m_pCObjectListCurrent) {
    if ( pcobjTemp1->IsSelect() ) {    // if selected
      rectTemp = pcobjTemp1->GetSize();
      h = rectTemp.right - rectTemp.left;
      rectTemp.right = w;
      rectTemp.left = w - h;
      pcobjTemp1->SetAllSize(rectTemp);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//  view into compound.
//
////////////////////////////////////////////////////////////////////////////////
void CSQIObjectView::ViewIn(CObjectComponentBase * pCObject) {
  CPoint pt(0,0);
  CRect rect;

  ASSERT( pCObject->CanViewIn() );
  ((CObjectComponent *)pCObject)->SetUpperObjectComponent( m_pCObjectComponentUpper );   // reserve upper compound
  m_pCObjectComponentUpper = pCObject;         // change to this compound  
  m_pCObjectComponentUpper->SetUpperObjectList( m_pCObjectListCurrent );    // reserve upper list
  m_pCObjectComponentUpper->SetUpperScrollPosition(GetScrollPosition());// reserve offset point
  m_pCObjectListCurrent = m_pCObjectComponentUpper->GetObjectList();         
  ScrollToPosition( pt );     // set to origin point
	m_pCObjectCurrent = nullptr;     // set current process object to nullptr
	m_nCurrentFunction = OBJECT_PRE_SELECT; // 当前状态为未选择

  GetDocument()->m_trackerObject.m_rect.SetRectEmpty();
  Invalidate();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  return to upper layer
//
////////////////////////////////////////////////////////////////////////////////////////
void CSQIObjectView::ViewOut( void ) {
  CPoint pt, ptScrollPosition = GetScrollPosition();
  CRect rect;

  m_pCObjectListCurrent = m_pCObjectComponentUpper->GetUpperObjectList(); // 切换到上层list
  m_pCObjectComponentUpper->ViewOut();		// 
  ScrollToPosition( m_pCObjectComponentUpper->GetUpperScrollPosition() ); // 滚动到上层偏移位置
  
	m_pCObjectCurrent = m_pCObjectComponentUpper;		// 当前处理Object为此复合单元
	m_nCurrentFunction = OBJECT_SELECTED;			// 当前状态为已选择
  
	pt = m_pCObjectCurrent->GetOffset();
  rect = m_pCObjectCurrent->GetSize();
  rect += pt;
  rect -= ptScrollPosition;
  GetDocument()->m_trackerObject.m_rect = rect;
  m_pCObjectComponentUpper = m_pCObjectComponentUpper->GetUpperObjectComponent(); // 当前复合单元为此复合单元的上层复合单元（如果有的话）
  Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// CSQIObjectView message handlers

//////////////////////////////////////////////////////////////////////////////
//
// OnLButtonDown和OnLButtonUp操作处理Object。
//
/////////////////////////////////////////////////////////////////////////////
void CSQIObjectView::OnLButtonDown(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default 
  CRect WinRect, rectScreen;
  CSQIFileDoc* pDoc = GetDocument();
  CDC * pdc = GetDC();
  OnPrepareDC(pdc);
  CPoint ptScrollPosition = GetDeviceScrollPosition(), ptDevice;
  CObjectBase * pcobj;
  
  // set current mouse address 
  m_ptStart = ptScrollPosition + point;
  switch ( gl_ulDrawTool ) {		// 根据当前画图工具决定如何执行任务
	case ID_OBJECT_SELECT:		// 工具选择任务，看是否是生成Symbol
    if ( IsInRect(m_ptStart, pcobj) ) {  // select object to process
      if ( (m_nCurrentFunction == OBJECT_SELECTED) 
        && (pDoc->m_trackerObject.HitTest(point) >= 0)
        && (pDoc->m_trackerObject.HitTest(point) < 8 ) ) {  // in change size ?
        if ( nFlags == (MK_SHIFT + MK_LBUTTON) ) m_nCurrentFunction = MAKE_SYMBOL;
        else m_nCurrentFunction = OBJECT_PRE_SELECT;
      }
      else {
        if ( nFlags == (MK_SHIFT + MK_LBUTTON) ) m_nCurrentFunction = MAKE_SYMBOL;
        else m_nCurrentFunction = OBJECT_PRE_SELECT;
        m_pCObjectCurrent = pcobj;
        rectScreen = m_pCObjectCurrent->GetSize();
        rectScreen += m_pCObjectCurrent->GetOffset() - ptScrollPosition;
        pDoc->m_trackerObject.m_rect = rectScreen;
      }
    }
    else {
			m_nCurrentFunction = OBJECT_PRE_SELECT;
			m_pCObjectCurrent = nullptr;
		}
		break;
	case ID_OBJECT_DYNAMICLINK :	// 生成一个动态连接
    if ( IsInRect(m_ptStart, m_pCObjectCurrent) ) {
      m_nCurrentFunction = CREATE_OBJ_DYN_LINK ;
    }
		break;
	default:	// 选择了Objects
    m_nCurrentFunction = OBJECT_PRE_SELECT;
    m_pCObjectCurrent = nullptr;
    pDoc->m_trackerObject.m_rect.SetRectEmpty();
		break;
  }

  switch (m_nCurrentFunction) {
	case OBJECT_PRE_SELECT:
		if (pDoc->m_trackerObject.HitTest(point) < 0) { // is some object been selected ?
			if (pDoc->m_trackerObject.TrackRubberBand(this, point, FALSE)) { // create new object
				if (gl_ulDrawTool == ID_OBJECT_SELECT) { // group select
					ClearAllFocus();
					rectScreen = pDoc->m_trackerObject.m_rect;
					rectScreen += ptScrollPosition;
					CObjectBase * pcobjTemp = nullptr;
					CRect rect;
          for (const auto pcobjTemp1 : *m_pCObjectListCurrent) {
						rect = pcobjTemp1->GetSize() + pcobjTemp1->GetOffset();
						if ((rectScreen & rect) == rect) {
							pcobjTemp1->SetSelect(TRUE);
						}
					}
					pDoc->m_trackerObject.m_rect.SetRectEmpty();
					if (pcobjTemp != nullptr) {
						m_pCObjectCurrent = pcobjTemp;
						InvalidateRect(rectScreen - ptScrollPosition);
						m_nCurrentFunction = MAKE_SYMBOL;
					}
					else {
						m_pCObjectCurrent = nullptr;
						m_nCurrentFunction = OBJECT_PRE_SELECT;
					}
				}
				else {  // create new object
					ClearAllFocus();
					m_rectCurrent = pDoc->m_trackerObject.m_rect;
					m_rectCurrent += ptScrollPosition;
					ptDevice.x = m_rectCurrent.right;
					ptDevice.y = m_rectCurrent.bottom;
					CString strTemp;
					char s[10];

          pDoc->SetObjNumber(pDoc->GetObjNumber() + 1);
					_itoa(pDoc->GetObjNumber(), s, 10);
					strTemp = "_";
					strTemp += s;
					switch (gl_ulDrawTool) {
						///////////////////////////////////////////////////////////////////      
					case ID_OBJECT_RECT:
						m_pCObjectCurrent = new CObjectRect(strTemp, m_rectCurrent);
						break;
						/////////////////////////////////////////////////////////////////////
					case ID_OBJECT_ROUNDRECT:
						m_pCObjectCurrent = new CObjectRoundRect(strTemp, m_rectCurrent);
						break;
						//////////////////////////////////////////////////////////////////
					case ID_OBJECT_OVAL:
						m_pCObjectCurrent = new CObjectOval(strTemp, m_rectCurrent);
						break;
						////////////////////////////////////////////////////////////////////
					case ID_OBJECT_TEXT:
						m_pCObjectCurrent = new CObjectText(strTemp, m_rectCurrent);
						break;
						//////////////////////////////////////////////////////////////////////
					case ID_OBJECT_LINE:
						m_pCObjectCurrent = new CObjectLine(strTemp, m_ptStart, ptDevice);
						break;
						///////////////////////////////////////////////////////////////////////
						//      case ID_OBJECT_PICTURE :
						//        m_pCObjectCurrent = new CObjectPicture( strTemp, m_rectCurrent );
						//        break;
						//////////////////////////////////////////////////////////////
					case ID_OBJECT_GRAPH:
						m_pCObjectCurrent = new CObjectGraph(strTemp, m_rectCurrent);
						break;
						//////////////////////////////////////////////////////////////////////
					case ID_OBJECT_GAUGE:
						m_pCObjectCurrent = new CObjectGauge(strTemp, m_rectCurrent);
						break;
						///////////////////////////////////////////////////////////////////////
					case ID_OBJECT_KNOB:
						m_pCObjectCurrent = new CObjectKnob(strTemp, m_rectCurrent);
						break;
						/////////////////////////////////////////////////////////////////////
					case ID_OBJECT_METER:
						m_pCObjectCurrent = new CObjectMeter(strTemp, m_rectCurrent);
						break;
						//////////////////////////////////////////////////////////////////////
					case ID_OBJECT_PUSHBUTTON:
						m_pCObjectCurrent = new CObjectButton(strTemp, m_rectCurrent);
						break;
						///////////////////////////////////////////////////////////////////////
					case ID_OBJECT_SLIDER:
						m_pCObjectCurrent = new CObjectSlider(strTemp, m_rectCurrent);
						break;
						///////////////////////////////////////////////////////////////////////
					case ID_OBJECT_FILLBAR:
						m_pCObjectCurrent = new CObjectFillBar(strTemp, m_rectCurrent);
						break;
						/////////////////////////////////////////////////////////////////////
					case ID_OBJECT_NUMBERINPUT:
						m_pCObjectCurrent = new CObjectNumberInput(strTemp, m_rectCurrent);
						break;
						//////////////////////////////////////////////////////////////////////
					case ID_OBJECT_COMPOUND:
						m_pCObjectCurrent = new CObjectComponent(strTemp, m_rectCurrent);
						break;
					default:
						ASSERT(0);
					} // switch gl_ulDrawTool
					pDoc->SetModifiedFlag(TRUE); // document's content is changed
					strTemp = m_pCObjectCurrent->GetClassNameStr() + strTemp;
					m_pCObjectCurrent->SetName(strTemp);
					m_pCObjectCurrent->SetSelect(TRUE);
					if (m_pCObjectComponentUpper != nullptr) {
						if (m_pCObjectComponentUpper->IsKindOf(RUNTIME_CLASS(CObjectSymbol))) {
							m_pCObjectCurrent->SetSymbolThatHaveMe((CObjectSymbol *)m_pCObjectComponentUpper);
							rectScreen = m_pCObjectCurrent->GetSize() - m_pCObjectCurrent->GetOffset();
							m_pCObjectCurrent->SetAllSize(rectScreen);
						}
					}
					AddObject(m_pCObjectCurrent);
					// update screen
					rectScreen = m_pCObjectCurrent->GetSize() + m_pCObjectCurrent->GetOffset() - ptScrollPosition;
					InvalidateRect(rectScreen);
					m_nCurrentFunction = OBJECT_SELECTED;
					gl_ulDrawTool = ID_OBJECT_SELECT;
				}
			}
			else {        // no selected object
				ClearAllFocus();
				pDoc->m_trackerObject.m_rect.SetRectEmpty();
				m_nCurrentFunction = OBJECT_PRE_SELECT;
			}
		}
	  else if (pDoc->m_trackerObject.Track(this, point, TRUE)) {  // change object's size and/or position
      ClearAllFocus();
      CPoint pt = m_pCObjectCurrent->GetOffset();
      m_rectCurrent = pDoc->m_trackerObject.m_rect;
      rectScreen = m_pCObjectCurrent->GetSize();
      rectScreen += pt;
      rectScreen -= ptScrollPosition;
      InvalidateRect( rectScreen );
      m_pCObjectCurrent->SetAllSize(m_rectCurrent - pt + ptScrollPosition);
      m_pCObjectCurrent->AdjustInnerSize();
      pDoc->SetModifiedFlag( TRUE ); // document's content is changed
      m_pCObjectCurrent->SetSelect( TRUE );
      m_nCurrentFunction = OBJECT_SELECTED;
      InvalidateRect( m_rectCurrent );
    }
    else {  // 选择了一个新的对象
      ClearAllFocus();
      CPoint pt = GetScrollPosition();
      m_pCObjectCurrent->SetSelect( TRUE );
      InvalidateRect( m_pCObjectCurrent->GetSize() -pt );
      m_nCurrentFunction = OBJECT_SELECTED;
      pDoc->m_trackerObject.m_rect = m_pCObjectCurrent->GetSize() - pt;
    }
		break; // case OBJECT_PRE_SELECT
	case MAKE_SYMBOL:  // make symbol
    ASSERT( m_pCObjectCurrent != nullptr );
    m_pCObjectCurrent->SetSelect(TRUE);
    m_rectCurrent = m_pCObjectCurrent->GetSize();
    InvalidateRect( m_pCObjectCurrent->GetSize() - GetScrollPosition() );
		break;
	default :
		TRACE("Error in select function\n");
		break;
  } // switch (m_nCurrentFunction)

	// 报告当前状态
  switch ( m_nCurrentFunction ) {
  case OBJECT_PRE_SELECT :
    m_rectCurrent.SetRectEmpty();
    TRACE("Current function is OBJECT_PRE_SELECT\n");
    break;
  case OBJECT_SELECTED :
  	SetObjectRect(m_pCObjectCurrent);
    TRACE("Current function is SELELCTED\n");
    break;          
  case CREATE_OBJ_DYN_LINK :
    TRACE("Current function is CREATE_OBJ_DYN_LINK\n");
    break;
  case MAKE_SYMBOL :
    ASSERT( m_pCObjectCurrent != nullptr );
    TRACE("Current function is MAKE_SYMBOL\n");
    break;
	default :
		TRACE("Current funciton out of range, Error.\n");
		break;
  } // switch  

  ReleaseDC(pdc);
  CScrollView::OnLButtonDown(nFlags, point);
}  

void CSQIObjectView::OnMouseMove(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
	CString str = "  ";

  CDC *pdc = GetDC();
  OnPrepareDC(pdc);             
  CPoint ptDevice, ptOffset = GetDeviceScrollPosition();
  
  m_pCObjectMouseMove = nullptr;	// 重置此指针
	ptDevice = ptOffset + point;
  switch ( m_nCurrentFunction ) {
	case OBJECT_PRE_SELECT :
		if ( IsInRect( ptDevice, m_pCObjectMouseMove) ) {  // current mouse position is in object ?
      // show current object's name
      str = m_pCObjectMouseMove->GetClassNameStr();
      str += "->";
      str += m_pCObjectMouseMove->GetName();
    }
    SetStatusBarMessage( str );
    // when found mouse in one object, if this object can viewIn,
    // that is this object if either a Component or Symbol,
    // the ViewIn Tool button was enabled.
    // if you can push viewIn button this time, you was in trouble,
    // so I set m_pCObjectCurrent = nullptr to disable the ViewIn button. 
		// 现在采用了另一个变量m_pCObjectMouseMove代替m_pCObjectCurrent，也能解决这个问题。
		break;
	default :
		break;
  } // switch ( m_nCurrentFunction )
  ReleaseDC(pdc);
  
  // show current mouse position
  SetMousePosition(ptDevice);
  
  CScrollView::OnMouseMove(nFlags, point);
}

void CSQIObjectView::OnLButtonUp(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CDynamicLinkDlg CDynLinkDlg;

  CSQIFileDoc* pDoc = GetDocument();
  CDC * pdc = GetDC();
  OnPrepareDC(pdc);
  CString strTemp; 
  CRect rectScreen;
  CPoint ptDevice, ptScrollPosition = GetDeviceScrollPosition();
  
  // calculate current absolute mouse position
  ptDevice = ptScrollPosition + point;
  switch (m_nCurrentFunction ) {
  case CREATE_OBJ_DYN_LINK :
    CDynLinkDlg.SetLink(pDoc->GetUnitDictionaryList(tMODIFIABLE | tDOUBLE | tBOOL | tWORD | tSTRING, m_pCObjectCurrent),
												m_pCObjectCurrent,
                        m_pCObjectCurrent->GetDynLinkList(),
                        m_pCObjectCurrent->GetParaNameAddress());
    CDynLinkDlg.DoModal();
    m_nCurrentFunction = OBJECT_PRE_SELECT;
    break;

  case OBJECT_PRE_SELECT :
    if ( m_pCObjectCurrent != nullptr ) {
      rectScreen = m_pCObjectCurrent ->GetSize();
      rectScreen -= ptScrollPosition;
      InvalidateRect( rectScreen );
      m_pCObjectCurrent = nullptr;
    }
    m_rectCurrent.SetRectEmpty();
    break;
	default :
		ASSERT( FALSE );
  } // switch m_nCurrentFunction
  ReleaseDC(pdc);
  
  CScrollView::OnLButtonUp(nFlags, point);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 双击鼠标为设置Object的内部参数
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSQIObjectView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  
  CSQIFileDoc* pDoc = GetDocument();

  m_pCObjectCurrent = nullptr;
  CPoint ptAbsolute, ptOffset = GetDeviceScrollPosition();  
  
  ptAbsolute = ptOffset + point;    // set current mouse address  
  
  if ( IsInRect(ptAbsolute, m_pCObjectCurrent) ) {  // select object to process  
    // change object's property 
    ASSERT( m_pCObjectCurrent != nullptr );
    if ( m_pCObjectCurrent->SetProperty() ) { // 设置此Object的内部参数
      pDoc->SetModifiedFlag( TRUE ); // document's content is changed
    }
    InvalidateRect(nullptr);
  }
  else { 
    // do nothing
  }
  CView::OnLButtonDblClk(nFlags, point);
}

void CSQIObjectView::OnEditDelete()
{
  // TODO: Add your command handler code here
  
  GetDocument()->SetModifiedFlag( TRUE ); // document's content is changed
  DeleteObject( m_pCObjectCurrent );
  m_pCObjectCurrent = nullptr;
  m_nCurrentFunction = OBJECT_PRE_SELECT;
	GetDocument()->m_trackerObject.m_rect.SetRectEmpty();

  Invalidate(); 
}

void CSQIObjectView::OnArrangeTofront()
{
  // TODO: Add your command handler code here
  GetDocument()->SetModifiedFlag( TRUE ); // document's content is changed
  ASSERT( m_pCObjectCurrent != nullptr );                                          
  ObjectToFront( m_pCObjectCurrent );
  InvalidateRect( nullptr );
}

void CSQIObjectView::OnArrangeToback()
{
  // TODO: Add your command handler code here
  GetDocument()->SetModifiedFlag( TRUE ); // document's content is changed
  ASSERT( m_pCObjectCurrent != nullptr );                                          
  ObjectToBack( m_pCObjectCurrent ); 
  InvalidateRect( nullptr );
}

void CSQIObjectView::OnArrangeMakesymbol()
{
  // TODO: Add your command handler code here  
  CSQIFileDoc * pDoc = GetDocument();
  CDC * pDC;
  CRect rectTemp;
  CString strTemp; 
  char s[10];
  CPoint ptOffset = GetDeviceScrollPosition();

  BOOL fCanMakeSymbol = TRUE;
  
  // 复合对象和输出类对象(按钮,滚动条等)不能组成符号, 检查被选对象中是否有.
  for (const auto pcobjTemp : *m_pCObjectListCurrent) {
    if ( pcobjTemp->IsSelect() ) {
      if ( !pcobjTemp->CanInSymbol() ) {
        fCanMakeSymbol = FALSE; // 不能生成符号.
			}
    }
  }
  if ( !fCanMakeSymbol ) {
		ShowMessage(ID_ERROR_DACVIEW_CANNOT_MAKE_SYMBOL);
		return;
	}

	// 生成符号类.
  pDC = GetDC();
	pDoc->SetModifiedFlag( TRUE ); // document's content is changed
  strTemp = "CObject_";
  _itoa(m_nCurrentObjNumber++, s, 10);
  strTemp += s;
  m_rectCurrent.SetRectEmpty();
  m_pCObjectCurrent = new CObjectSymbol(strTemp, m_rectCurrent) ;                              
  MakeSymbol( (CObjectSymbol *)m_pCObjectCurrent );
  AddObject(m_pCObjectCurrent);
  m_pCObjectCurrent->SetSelect( TRUE );
  rectTemp = m_pCObjectCurrent->GetSize() + m_pCObjectCurrent->GetOffset();
  rectTemp -= ptOffset;       // change to screen position
  pDoc->m_trackerObject.m_rect = rectTemp;
  InvalidateRect(rectTemp); // clear former focus   
  m_nCurrentFunction = OBJECT_SELECTED;
	ReleaseDC(pDC);
}

void CSQIObjectView::OnArrangeBreaksymbol()
{
  // TODO: Add your command handler code here      
  CDC * pdc = GetDC();
  CRect rectTemp;
  CPoint ptOffset = GetDeviceScrollPosition();
  
  GetDocument()->SetModifiedFlag( TRUE ); // document's content is changed
  rectTemp = m_pCObjectCurrent->GetSize();
  rectTemp -= ptOffset;       // change to screen position
  
  BreakSymbol( (CObjectSymbol *)m_pCObjectCurrent );
  InvalidateRect(rectTemp); // clear former focus
  ReleaseDC(pdc);
  m_pCObjectCurrent = nullptr;
  m_nCurrentFunction = MAKE_SYMBOL;
}

void CSQIObjectView::OnStyleCentered()
{
  // TODO: Add your command handler code here
  GetDocument()->SetModifiedFlag( TRUE ); // document's content is changed
  CenterAlign();
  Invalidate();
}

void CSQIObjectView::OnStyleLeft()
{
  // TODO: Add your command handler code here
  GetDocument()->SetModifiedFlag( TRUE ); // document's content is changed
  LeftAlign();
  Invalidate();
}

void CSQIObjectView::OnStyleRight()
{
  // TODO: Add your command handler code here
  GetDocument()->SetModifiedFlag( TRUE ); // document's content is changed
  RightAlign();
  Invalidate();
}

void CSQIObjectView::OnUpdateArrangeBreakSymbol(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch ( m_nCurrentFunction ) {
  case MAKE_SYMBOL :
    pCmdUI->Enable(FALSE);
    break;
  case OBJECT_PRE_SELECT :
    pCmdUI->Enable(FALSE);
    break;
  case OBJECT_SELECTED :
    if ( m_pCObjectCurrent->IsKindOf(RUNTIME_CLASS(CObjectSymbol)) ) {
      pCmdUI->Enable(TRUE);
    }
    else {
      pCmdUI->Enable(FALSE);
    }                    
    break;
  default : 
    pCmdUI->Enable(FALSE);
  } // switch 

}

void CSQIObjectView::OnUpdateArrangeMakeSymbol(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch ( m_nCurrentFunction ) {
  case MAKE_SYMBOL :
    pCmdUI->Enable(TRUE);
    break;
  case OBJECT_PRE_SELECT :
    pCmdUI->Enable(FALSE);
    break;
  case OBJECT_SELECTED :
    pCmdUI->Enable(FALSE);
    break;
  default : 
    pCmdUI->Enable(FALSE);
  } // switch
}

void CSQIObjectView::OnUpdateArrangeToback(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch ( m_nCurrentFunction ) {
  case OBJECT_SELECTED :
    pCmdUI->Enable(TRUE);
    break;
  default : 
    pCmdUI->Enable(FALSE);
  } // switch
}

void CSQIObjectView::OnUpdateArrangeTofront(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch ( m_nCurrentFunction ) {
  case OBJECT_SELECTED :
    pCmdUI->Enable(TRUE);
    break;
  default : 
    pCmdUI->Enable(FALSE);
  } // switch
}

void CSQIObjectView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch ( m_nCurrentFunction ) {
  case OBJECT_SELECTED :
    pCmdUI->Enable(TRUE);
    break;
  default :
    pCmdUI->Enable(FALSE);
    break;
  } // switch
}

void CSQIObjectView::OnUpdateEditDelete(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch ( m_nCurrentFunction ) {
  case OBJECT_SELECTED :
    pCmdUI->Enable(TRUE);
    break;
  default :
    pCmdUI->Enable(FALSE);
    break;
  } // switch
}

void CSQIObjectView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch ( m_nCurrentFunction ) {
  case OBJECT_SELECTED :
    pCmdUI->Enable(TRUE);
    break;
  default :
    pCmdUI->Enable(FALSE);
    break;
  } // switch
}

void CSQIObjectView::OnEditCopy()
{
  // TODO: Add your command handler code here
  char *strFileName = "VIEW.CLP";
  char * lpData;

  if ( (hData = GlobalAlloc(GMEM_MOVEABLE, 128)) == nullptr ) {
    ShowMessage(ID_ERROR_OUT_OF_MEMORY);
    return;
  }
  if ( (lpData = (char *)GlobalLock(hData)) == nullptr ) {
    ShowMessage(ID_ERROR_OUT_OF_MEMORY);
    return;
  }
  lstrcpy(lpData, strFileName);
  
  CFile cFile; 
  CFileException e;
  char buffer[512];
  
  if ( !cFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite, &e) ) {
    ShowMessage(ID_ERROR_OPEN_FILE, strFileName);
  }
  CArchive ar(&cFile, CArchive::store, 512, buffer); 
  ar << m_pCObjectCurrent;
  
  if ( OpenClipboard() ) {
    EmptyClipboard();
    SetClipboardData(m_uObjectFormat, hData);
    GlobalUnlock(hData);
    CloseClipboard();
  }
  hData = nullptr;
}

void CSQIObjectView::OnEditCut()
{
  // TODO: Add your command handler code here
  char *strFileName = "VIEW.CLP";
  char * lpData;

  if ( (hData = GlobalAlloc(GMEM_MOVEABLE, 128)) == nullptr ) {
    ShowMessage(ID_ERROR_OUT_OF_MEMORY);
    return;
  }
  if ( (lpData = (char *)GlobalLock(hData)) == nullptr ) {
    ShowMessage(ID_ERROR_OUT_OF_MEMORY);
    return;
  }
  lstrcpy(lpData, strFileName);
  
  CFile cFile; 
  CFileException e;
  char buffer[512];
  
  if ( !cFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite, &e) ) {
    ShowMessage(ID_ERROR_OPEN_FILE, strFileName);
  }
  CArchive ar(&cFile, CArchive::store, 512, buffer); 
  ar << m_pCObjectCurrent;
  DeleteObject( m_pCObjectCurrent );
  m_pCObjectCurrent = nullptr;
  m_nCurrentFunction = OBJECT_PRE_SELECT;
	GetDocument()->m_trackerObject.m_rect.SetRectEmpty();

  if ( OpenClipboard() ) {
    EmptyClipboard();
    SetClipboardData(m_uObjectFormat, hData);
    GlobalUnlock(hData);
    CloseClipboard();
  }
  hData = nullptr;
  Invalidate();
}

void CSQIObjectView::OnEditPaste()
{
  // TODO: Add your command handler code here
  CFile cFile; 
  char buffer[512];
  char * strFileName = nullptr;

  if ( OpenClipboard() ) {
    if ( (hData = GetClipboardData(m_uObjectFormat)) == nullptr ) {
      CloseClipboard();
      return;
    }
    if ( (strFileName = (char *)GlobalLock(hData)) == nullptr ) {
      ShowMessage(ID_ERROR_OUT_OF_MEMORY);
      CloseClipboard();
      return;
    }
  }
  
  if ( !cFile.Open(strFileName, CFile::modeRead) ) {
    ShowMessage(ID_ERROR_OPEN_FILE, strFileName);
  }
  CArchive ar(&cFile, CArchive::load, 512, buffer); 
  ar >> m_pCObjectCurrent;
	ClearAllSelect();
  CreateUniName( m_pCObjectCurrent );
  m_pCObjectListCurrent->push_back(m_pCObjectCurrent);
	m_pCObjectCurrent->SetSelect( TRUE );
	if ( m_pCObjectComponentUpper != nullptr ) {
		if ( m_pCObjectComponentUpper->IsKindOf(RUNTIME_CLASS(CObjectSymbol)) ) {
			m_pCObjectCurrent->SetSymbolThatHaveMe( (CObjectSymbol *)m_pCObjectComponentUpper );
		}
	}
	GetDocument()->m_trackerObject.m_rect = m_pCObjectCurrent->GetSize() - GetDeviceScrollPosition();
  m_nCurrentFunction = OBJECT_SELECTED;
  GlobalUnlock(hData);
  CloseClipboard();
  GetDocument()->SetModifiedFlag( TRUE ); // document's content is changed
  Invalidate();
}

void CSQIObjectView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if ( OpenClipboard() ) {
    if ( IsClipboardFormatAvailable(m_uObjectFormat) ) {
      pCmdUI->Enable(TRUE);
    }
    else pCmdUI->Enable(FALSE);
  }
  else pCmdUI->Enable(FALSE);
  CloseClipboard();
}

void CSQIObjectView::OnUpdateStyleCentered(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch ( m_nCurrentFunction ) {
  case MAKE_SYMBOL :
    pCmdUI->Enable(TRUE);
    break;
  default :
    pCmdUI->Enable(FALSE);
    break;
  } // switch
}

void CSQIObjectView::OnUpdateStyleLeft(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch ( m_nCurrentFunction ) {
  case MAKE_SYMBOL :
    pCmdUI->Enable(TRUE);
    break;
  default :
    pCmdUI->Enable(FALSE);
    break;
  } // switch
}

void CSQIObjectView::OnUpdateStyleRight(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch ( m_nCurrentFunction ) {
  case MAKE_SYMBOL :
    pCmdUI->Enable(TRUE);
    break;
  default :
    pCmdUI->Enable(FALSE);
    break;
  } // switch
}

void CSQIObjectView::OnSize(UINT nType, int cx, int cy)
{
  CScrollView::OnSize(nType, cx, cy);
  
  // TODO: Add your message handler code here
  SetScrollSizes(MM_TEXT, GetDocSize());
}

int CSQIObjectView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CScrollView::OnCreate(lpCreateStruct) == -1)
    return -1;
  
  // TODO: Add your specialized creation code here
  m_pCObjectListCurrent = GetDocument()->GetObjectList();
  return 0;
}

void CSQIObjectView::OnUpdateViewViewin(CCmdUI* pCmdUI) 
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

void CSQIObjectView::OnUpdateViewViewout(CCmdUI* pCmdUI) 
{
  // TODO: Add your command update UI handler code here
  if ( m_uIntoObjectComponent ) {
    pCmdUI->Enable(TRUE);
  }
  else {
    pCmdUI->Enable(FALSE);
  }
}

void CSQIObjectView::OnViewViewin() 
{
  // TODO: Add your command handler code here
  ViewIn((CObjectComponentBase *)m_pCObjectCurrent);
  m_uIntoObjectComponent++;   // goto inner
}

void CSQIObjectView::OnViewViewout() 
{
  // TODO: Add your command handler code here
  if ( m_uIntoObjectComponent > 0 ) {
    ViewOut();
    m_uIntoObjectComponent--;
  }
}

void CSQIObjectView::OnUpdateArrangeBreakdynlink(CCmdUI* pCmdUI) 
{
  // TODO: Add your command update UI handler code here
  switch ( m_nCurrentFunction ) {
  case OBJECT_SELECTED :
    if ( m_pCObjectCurrent->GetDynLinkList()->size() > 0 ) {  // have dynamic link ?
      pCmdUI->Enable(TRUE);
    }
    else {
      pCmdUI->Enable(FALSE);
    }
    break;
  default :
    pCmdUI->Enable(FALSE);
    break;
  } // switch
}

void CSQIObjectView::OnArrangeBreakdynlink() 
{
  // TODO: Add your command handler code here
  if ( m_pCObjectCurrent->IsDeleteDynLink() ) {
		GetDocument()->SetModifiedFlag( TRUE );
	}
  Invalidate();
}

void CSQIObjectView::OnViewProperty() 
{
	// TODO: Add your command handler code here
	if ( m_pCObjectCurrent->SetProperty() ) {
    GetDocument()->SetModifiedFlag( TRUE );
  }
	Invalidate();
}

void CSQIObjectView::OnUpdateViewProperty(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_pCObjectCurrent != nullptr );
}

//////////////////////////////////////////////////////////////////////////
//
// OnSetCursor()
//
// 当移动光标时,如果鼠标在已选中对象中,则显示m_trackerObject的光标,否则显示正常的光标(箭头).
//
//////////////////////////////////////////////////////////////////////////
BOOL CSQIObjectView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	CSQIFileDoc* pDoc = GetDocument();
	if (pWnd == this && pDoc->m_trackerObject.SetCursor(this, nHitTest)) // 显示m_trackerObject的光标.
		return TRUE;
	
	if ((m_pCObjectMouseMove != nullptr) && (m_pCObjectCurrent == nullptr)) {
		if (m_pCObjectMouseMove->CanViewIn()) {
			SetCursor(m_hCursorComponent);
		}
	}
	else SetCursor(m_hCursorNormal);	// 显示箭头.

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CSQIObjectView::OnArrangeMakedynlink() 
{
	// TODO: Add your command handler code here
  CSQIFileDoc * pDoc = GetDocument();
	CDynamicLinkDlg CDynLinkDlg;

  ASSERT( m_pCObjectCurrent != nullptr );
  m_pCObjectCurrent->SelectParameter(tMODIFIABLE | tDOUBLE | tBOOL | tWORD | tSTRING);
  CDynLinkDlg.SetLink(pDoc->GetUnitDictionaryList(tMODIFIABLE | tDOUBLE | tBOOL | tWORD | tSTRING, m_pCObjectCurrent),
											m_pCObjectCurrent,
                      m_pCObjectCurrent->GetDynLinkList(),
                      m_pCObjectCurrent->GetParaNameAddress());
  switch ( CDynLinkDlg.DoModal() ) {
		case IDOK:
      //m_pCObjectCurrent->SetParameterSelected(); // 设置输入型单元的参数选择标志
			pDoc->SetModifiedFlag( TRUE );
	}
	m_pCObjectCurrent->CheckSelf();
}

void CSQIObjectView::OnUpdateArrangeMakedynlink(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
  switch ( m_nCurrentFunction ) {
  case OBJECT_SELECTED :
    pCmdUI->Enable(TRUE);
    break;
  default :
    pCmdUI->Enable(FALSE);
    break;
  } // switch
}
