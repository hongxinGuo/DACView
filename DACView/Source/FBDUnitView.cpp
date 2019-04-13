//////////////////////////////////////////////////////////////////////////
// runstrat.cpp : implementation file
//
// OnInitialUpdate()					:
// OnUpdate()									:
// OnDraw()										: Show unit on DC.
//
// FindUnit()									: find unit pointer by his name.
// GetUnitDocSize()						: Get this unit list's size
// IsInRect()									: Check pt is in a rect.
//
// OnTimer()									: Period called function, show units
// OnCreate()									: 
// OnDestroy()								:
// OnViewViewIn()
// OnUpdateViewViewIn()
// OnViewViewOut()
// OnUpdateViewViewOut()
// OnLButtonDown()
// OnlButtonUp()
// OnMouseMove()
// OnLButtonDblClk()
// OnUpdateSCClose()
// OnUpdateObjectBar()
// OnViewStatus()
// OnUpdateViewStatus()
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "dacview.h"
#include "globedef.h"

#include "FBDUnitView.h"
#include "FBDFileDoc.h"

#include "CObjectPrimitive.h"
#include "CUnitComponent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFBDUnitView

IMPLEMENT_DYNCREATE(CFBDUnitView, CScrollView)

CFBDUnitView::CFBDUnitView()
{
	CSize sizeTemp(2000, 1500);
	m_sizeUnitDoc = sizeTemp;
	
	m_uIntoComponentProcess = 0;
	m_pCUnitComponentCurrent = nullptr;
	m_pCUnitCurrent  = nullptr;
	m_ulUnitListCurrentCount = 0;
	m_uIdTimer = 0;
}

CFBDUnitView::~CFBDUnitView()
{
}


BEGIN_MESSAGE_MAP(CFBDUnitView, CScrollView)
	//{{AFX_MSG_MAP(CFBDUnitView)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_VIEWIN, OnViewViewin)
	ON_COMMAND(ID_VIEW_VIEWOUT, OnViewViewout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWIN, OnUpdateViewViewin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWOUT, OnUpdateViewViewout)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_UPDATE_COMMAND_UI(AFX_IDS_SCCLOSE, OnUpdateSCClose)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_BRUSH, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS, OnUpdateViewStatus)
	ON_COMMAND(ID_VIEW_STATUS, OnViewStatus)
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
	ON_UPDATE_COMMAND_UI(ID_OBJECT_NUMBERINPUT, OnUpdateObjectBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFBDUnitView drawing 

////////////////////////////////////////////////////////////////////////
//
// OnInitialUpdate()
//
////////////////////////////////////////////////////////////////////////
void CFBDUnitView::OnInitialUpdate() {
  SetScrollSizes( MM_TEXT, GetUnitDocSize() );
}

////////////////////////////////////////////////////////////////////////
//
// OnUpdate()
//
////////////////////////////////////////////////////////////////////////
void CFBDUnitView::OnUpdate( )
{
   // ...
   // Implement a GetDocSize( ) member function in 
   // your document class; it returns a CSize.
   SetScrollSizes( MM_TEXT, GetUnitDocSize( ) );
   ResizeParentToFit( );   // Default bShrinkOnly argument
   // ...
}          

////////////////////////////////////////////////////////////////////////////////
//
// OnDraw()
//
// Parameter :
//		CDC * pDC : Current draw DC
//
// Description :
//		display m_CUnitList's all object to pdc, head is the first,
//	this is mean draw from back unit to front.
//
///////////////////////////////////////////////////////////////////////////
void CFBDUnitView::OnDraw(CDC* pDC)
{
	CFBDFileDoc * pDoc = GetDocument();

  // TODO: add draw code here 
	CRect rectClip, rectTemp;
	
	pDC->GetClipBox(&rectClip);
	SetStrategyClipRect(rectClip);
	ULONGLONG ulTick;
	ulTick = GetTickCount64();
  
  if ( pDoc->IsInterRunUnitList() ) { // is some one have do this function?
  	return; 	// some one is doing, so I can't do. 
	}
	else pDoc->SetInterRunUnitList( TRUE );	 // no one do this, so I can do it

  CUnitBase * pcunitTemp;
  POSITION poUnit = m_pCUnitListCurrent->GetHeadPosition();                          
	INT_PTR i, iTemp = m_pCUnitListCurrent->GetCount();
  
  for ( i = 0; i < iTemp; i ++ ) {                            
    pcunitTemp = m_pCUnitListCurrent->GetNext(poUnit);
		pcunitTemp->ToShow( pDC ); 
  }
	pDoc->SetInterRunUnitList( FALSE );		// I have done it. clear the flag.
  
  gl_ulSystemProcessTime += GetTickCount64() - ulTick;
} 

////////////////////////////////////////////////////////////////////////
//
// FindUnit()
//
// Parameter :
//		CString TagName : unit's name
//
// Return :
//		CUnitBase * : pointer of unit, nullptr if not found.
//
// Description :
//		find the unit's pointer according its name
//
////////////////////////////////////////////////////////////////////////
CUnitBase * CFBDUnitView::FindUnit( CString TagName ) {
  CUnitBase * pc;
  POSITION pos = m_pCUnitListCurrent->GetHeadPosition(); 
	INT_PTR iTemp = m_pCUnitListCurrent->GetCount();
  
  for ( int i = 0; i < iTemp; i ++ ) {                            
    pc = m_pCUnitListCurrent->GetNext(pos);
    if ( (pc->IsMe(TagName))  ) {
      return( pc );
    }
  }    
  return ( nullptr );
}

////////////////////////////////////////////////////////////////////////
//
// GetUnitDocSize()
//
////////////////////////////////////////////////////////////////////////
CSize CFBDUnitView::GetUnitDocSize( void ) {
  return ( m_sizeUnitDoc );
}

////////////////////////////////////////////////////////////////////////
//
// SetStrtegyClipRect()
// 
// Parameter :
//		CRect rectClip : current clip rect
//
// Return : no return
//
// Description :
//		Set current clip rect.
//
////////////////////////////////////////////////////////////////////////
void 	CFBDUnitView::SetStrategyClipRect( CRect rectClip ) {
	m_crectStrategyClip = rectClip;
}

///////////////////////////////////////////////////////////////////////
//
// IsInRect()
//
// Parameter :
//		POINT const pt				: current point that need to check.
//		CUnitBase *& pcobj		: point to which object have the pt.
//
// Return :
//		BOOL :  TRUE if pt in a rect, pcobj point to the object.
//						FALSE if pt in a blank area, poobj is nullptr.
//
// Description :
//		check whether 'pt' is in a 'rect' of m_pCUnitListCurrent->
//
///////////////////////////////////////////////////////////////////////  
BOOL CFBDUnitView::IsInRect( POINT const pt, CUnitBase* & pcobj ) {
  CUnitBase* pc;
  POSITION poUnit = m_pCUnitListCurrent->GetTailPosition(); // Tail position is the top most
	INT_PTR iTemp = m_pCUnitListCurrent->GetCount();
  
  for ( int i = 0; i < iTemp; i ++ ) {                            
    pc = m_pCUnitListCurrent->GetPrev(poUnit);
    if ( pc->InIt( pt ) ) {
      pcobj = pc;
      return ( TRUE );
    }
  }
  pcobj = nullptr;
  return ( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
// CFBDObjView diagnostics

#ifdef _DEBUG
void CFBDUnitView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFBDUnitView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFBDFileDoc* CFBDUnitView::GetDocument( void ) {  // no debug version is inline
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFBDFileDoc)));
	return (CFBDFileDoc*) m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFBDUnitView message handlers

////////////////////////////////////////////////////////////////////////
//
// OnTimer()
// 64位函数需要把UINT改为UINT_PTR,只有OnTimer函数需要这么改。
//
////////////////////////////////////////////////////////////////////////
void CFBDUnitView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
  CFBDFileDoc * pDoc = GetDocument();
  CDC * pdc;
	CUnitBase * pcunit;
  POSITION pos;                          
	INT_PTR iCount = m_pCUnitListCurrent->GetCount();
  CRect rectTemp;
	BOOL fEnd = FALSE;

	ULONGLONG ulTick;
	ulTick = GetTickCount64();
  
  pos = m_pCUnitListCurrent->GetHeadPosition();
	for ( ULONG i = 0; i < m_ulUnitListCurrentCount; i++ ) {
		pcunit = m_pCUnitListCurrent->GetNext( pos );
	}
  while ( (!fEnd) && (m_ulUnitListCurrentCount++ < iCount) ) {
    pcunit = m_pCUnitListCurrent->GetNext(pos);
    if ( pcunit->IsNeedUpdate() ) {
			pdc = GetDC();
			OnPrepareDC( pdc );
			pcunit->ToShow( pdc );
			ReleaseDC( pdc );
			pcunit->SetUpdateFlag(FALSE);
			fEnd = TRUE;
    }
  } 
  
  if ( m_ulUnitListCurrentCount >= iCount ) {
  	m_ulUnitListCurrentCount = 0;
  } 
  if ( pDoc->IsShowStatus() ) {
    pDoc->ShowStatus();
  }
  
  gl_ulSystemProcessTime += GetTickCount64() - ulTick;
  
	CScrollView::OnTimer(nIDEvent);
}

////////////////////////////////////////////////////////////////////////
//
// OnCreate()
//
////////////////////////////////////////////////////////////////////////
int CFBDUnitView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_pCUnitListCurrent = GetDocument()->GetUnitList();		// initialize current unitList

	CRect rectTemp(0, 0, 800, 500);
	CUnitBase * pcunitTemp;
  POSITION pos = m_pCUnitListCurrent->GetHeadPosition();                          
	INT_PTR i, iTemp = m_pCUnitListCurrent->GetCount();
  
  for ( i = 0; i < iTemp; i ++ ) { 
    pcunitTemp = m_pCUnitListCurrent->GetNext(pos);
    rectTemp |= pcunitTemp->GetSize();
  }                                 
	m_sizeUnitDoc = CSize(rectTemp.right,rectTemp.bottom); 

	m_uIdTimer = SetTimer(1, 1000, nullptr);
	if ( m_uIdTimer == 0 ) {
    CString str;
    str = GetDocument()->GetTitle();
	  ShowMessage(ID_ERROR_SET_TIMER, (LPCTSTR)str);
  }
	return 0;
}

////////////////////////////////////////////////////////////////////////
//
// OnDestroy()
//
////////////////////////////////////////////////////////////////////////
void CFBDUnitView::OnDestroy()
{
	CScrollView::OnDestroy();
	
	// TODO: Add your message handler code here
	if ( m_uIdTimer ) {
		if ( !KillTimer(m_uIdTimer) ) {
      CString str;
      GetWindowText( str );
		  ShowMessage(ID_ERROR_KILL_TIMER, (LPCTSTR)str);
    }
	}

}

////////////////////////////////////////////////////////////////////////
//
// OnViewViewIn()
//
////////////////////////////////////////////////////////////////////////
void CFBDUnitView::OnViewViewin()
{
	// TODO: Add your command handler code here
	CPoint pt(0,0);
	
	ASSERT( m_pCUnitCurrent->CanViewIn() );
	((CUnitComponent *)m_pCUnitCurrent)->SetComponentUpper( m_pCUnitComponentCurrent );
	m_pCUnitComponentCurrent = (CUnitComponent *)m_pCUnitCurrent;
	m_pCUnitComponentCurrent->SetUpperUnitList( m_pCUnitListCurrent );
	m_pCUnitComponentCurrent->SetUpperScrollPosition(GetScrollPosition());
	m_pCUnitListCurrent = m_pCUnitComponentCurrent->GetUnitList();        
	ScrollToPosition( pt );
	m_uIntoComponentProcess++;
	m_pCUnitCurrent = nullptr;
	m_ulUnitListCurrentCount = 0;
	Invalidate();
}

void CFBDUnitView::OnUpdateViewViewin(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	
	if ( m_pCUnitCurrent != nullptr ) {
		if ( m_pCUnitCurrent->CanViewIn() ) {
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

void CFBDUnitView::OnViewViewout()
{
	// TODO: Add your command handler code here
	m_pCUnitListCurrent = m_pCUnitComponentCurrent->GetUpperUnitList();
	ScrollToPosition( m_pCUnitComponentCurrent->GetUpperScrollPosition() );
	m_pCUnitCurrent = m_pCUnitComponentCurrent;
	m_pCUnitComponentCurrent = m_pCUnitComponentCurrent->GetComponentUpper();
	m_uIntoComponentProcess--;
	m_ulUnitListCurrentCount = 0;
	
	Invalidate();
}

void CFBDUnitView::OnUpdateViewViewout(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	
	if ( m_uIntoComponentProcess ) {
		pCmdUI->Enable(TRUE);
	}
	else {
		pCmdUI->Enable(FALSE);
	}
}

void CFBDUnitView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CFBDUnitView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptDevice;
  CDC * pdc = GetDC();
  OnPrepareDC( pdc );
  CPoint m_ptOffset = GetDeviceScrollPosition();
  
  ptDevice = point.x + m_ptOffset.x;
  ptDevice.y = point.y + m_ptOffset.y;
  if ( m_pCUnitCurrent != nullptr ) {
    m_pCUnitCurrent->SetSelect( FALSE );
    Invalidate();
  }
	if ( IsInRect(ptDevice, m_pCUnitCurrent) ) {
    m_pCUnitCurrent->SetSelect( TRUE );
    Invalidate();
  }
	else {
		m_pCUnitCurrent = nullptr;
	}
  ReleaseDC ( pdc );

	CScrollView::OnLButtonUp(nFlags, point);
}

void CFBDUnitView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
  CPoint ptDevice, ptOffset = GetDeviceScrollPosition();
  CString str = " ";
	CUnitBase * pUnit = nullptr;

  ptDevice = ptOffset + point;
  if ( m_pCUnitCurrent == nullptr ) {
    if ( IsInRect( ptDevice, pUnit ) ) {
      // show current unit's name on status bar
      str = pUnit->GetClassNameStr();
      str += "->";
      str += pUnit->GetName();
	  }
    SetStatusBarMessage( str );
  }

	CScrollView::OnMouseMove(nFlags, point);
}

void CFBDUnitView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptDevice, ptOffset = GetDeviceScrollPosition();
  
	ptDevice = point + ptOffset;
  if ( IsInRect(ptDevice, m_pCUnitCurrent) ) {  // select object to process  
    // change object's property 
    ASSERT( m_pCUnitCurrent != nullptr );
    m_pCUnitCurrent->SetProperty();
		Invalidate();
  }
  else { 
    // do nothing
  }

	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CFBDUnitView::OnUpdateSCClose(CCmdUI* pCmdUI) {
	pCmdUI->Enable( FALSE );
}	

void CFBDUnitView::OnUpdateObjectBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(FALSE);
}

void CFBDUnitView::OnViewStatus() 
{
	// TODO: Add your command handler code here
	GetDocument()->ToggleViewStatus( m_pCUnitCurrent );
}

void CFBDUnitView::OnUpdateViewStatus(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if ( m_pCUnitCurrent != nullptr ) {
		pCmdUI->Enable(TRUE);
	}
	else {
		pCmdUI->Enable(FALSE);
	}
}
