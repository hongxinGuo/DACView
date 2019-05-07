#include "stdafx.h"

#include "Globedef.h"

#include "cObjline.h"
#include "DlgObjLine.h"

IMPLEMENT_SERIAL(CObjectLine, CObjectBase, 1 | VERSIONABLE_SCHEMA);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

CObjectLine::CObjectLine( const CString& s, const CPoint& ptStart, const CPoint& ptEnd) 
  :CObjectBase( s ) { 
  m_ptEnd = ptEnd;
  m_ptStart = ptStart;
  m_ulWidth = 1;
  m_fCreateMemoryDC = FALSE;

}  
                         
CObjectLine::CObjectLine( void ) : CObjectBase( ) {
  m_ulWidth = 1;
  m_fCreateMemoryDC = FALSE;

}

CObjectLine::~CObjectLine( ) {
} 

/////////////////////////////////////////////////////////////////////////////
// CObjectLine diagnostics

#ifdef _DEBUG
void CObjectLine::AssertValid() const
{
  CObjectBase::AssertValid();
}

void CObjectLine::Dump(CDumpContext& dc) const
{
  CObjectBase::Dump(dc);
  dc << "\nm_ulWitth = " << m_ulWidth;
  dc << "\nm_ptStart = " << m_ptStart;
  dc << "\nm_ptEnd = " << m_ptEnd;
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectLine membem_rect function

const CRect& CObjectLine::GetSize( void ) {
  if ( m_ptStart.x < m_ptEnd.x ) {
    m_rectArea.left = m_ptStart.x;
    m_rectArea.right = m_ptEnd.x;
  }
  else if ( m_ptStart.x > m_ptEnd.x ) {
    m_rectArea.left = m_ptEnd.x;
    m_rectArea.right = m_ptStart.x;
  }
  else {
    m_rectArea.left = m_ptStart.x;
    m_rectArea.right = m_ptStart.x + 1;
  }
  if ( m_ptStart.y < m_ptEnd.y ) {
    m_rectArea.top = m_ptStart.y;
    m_rectArea.bottom = m_ptEnd.y;
  }
  else if ( m_ptStart.y > m_ptEnd.y ) {
    m_rectArea.top = m_ptEnd.y;
    m_rectArea.bottom = m_ptStart.y;
  }
  else {
    m_rectArea.top = m_ptStart.y;
    m_rectArea.bottom = m_ptStart.y + 1;
  }
  return(m_rectArea);
}

const CRect& CObjectLine::GetLastSize( void ) {
  if ( m_ptStart.x < m_ptEnd.x ) {
    m_rectAreaLast.left = m_ptStart.x;
    m_rectAreaLast.right = m_ptEnd.x;
  }
  else if ( m_ptStart.x > m_ptEnd.x ) {
    m_rectAreaLast.left = m_ptEnd.x;
    m_rectAreaLast.right = m_ptStart.x;
  }
  else { // length = 1
    m_rectAreaLast.left = m_ptStart.x;
    m_rectAreaLast.right = m_ptStart.x + 1;
  }
  if ( m_ptStart.y < m_ptEnd.y ) {
    m_rectAreaLast.top = m_ptStart.y;
    m_rectAreaLast.bottom = m_ptEnd.y;
  }
  else if ( m_ptStart.y > m_ptEnd.y ) {
    m_rectAreaLast.top = m_ptEnd.y;
    m_rectAreaLast.bottom = m_ptStart.y;
  }
  else {    // lenth = 1
    m_rectAreaLast.top = m_ptStart.y;
    m_rectAreaLast.bottom = m_ptStart.y + 1;
  }
  return(m_rectAreaLast);
}

CRgn* CObjectLine::GetClipRgn( const CPoint& ) {
  return( &m_rgnClip );
}

void CObjectLine::Serialize( CArchive& ar ) {
  CObjectBase::Serialize( ar );
  if( ar.IsStoring() ) {
    ar << m_ulWidth << m_ptStart << m_ptEnd;
  }
  else {
     ar >> m_ulWidth >> m_ptStart >> m_ptEnd;
  }
}

bool CObjectLine::InIt( POINT const pt, int ) { 
  CRect rectTemp;
  double a, b, d;
  CRect rect;
  CPoint ptTemp;

  if ( m_ptEnd.y == m_ptStart.y ) {
    if ( abs( pt.y - m_ptStart.y ) > 3 ) return( FALSE );
    if ( m_ptStart.x < m_ptEnd.x ) {
      if ( (m_ptStart.x <= pt.x) && (m_ptEnd.x >= pt.x) ) return( TRUE );
    }
    else {
      if ( (m_ptStart.x >= pt.x) && (m_ptEnd.x <= pt.x) ) return( TRUE );
    }
    return( FALSE );
  }
  if ( m_ptEnd.x == m_ptStart.x ) {
    if ( abs( pt.x - m_ptStart.x ) > 3 ) return( FALSE );
    if ( m_ptEnd.y > m_ptStart.y ) {
      if ( (m_ptStart.y <= pt.y) && (m_ptEnd.y >= pt.y) ) return( TRUE );
    }
    else {
      if ( (m_ptStart.y >= pt.y) && (m_ptEnd.y <= pt.y) ) return( TRUE );
    }
    return( FALSE );
  }

  if ( m_ptStart.x > m_ptEnd.x ) {
    rectTemp.left = m_ptEnd.x;
    rectTemp.right = m_ptStart.x;
  }
  else {
    rectTemp.left = m_ptStart.x;
    rectTemp.right = m_ptEnd.x;
  }
  if ( m_ptStart.y > m_ptEnd.y ) {
    rectTemp.top = m_ptEnd.y;
    rectTemp.bottom = m_ptStart.y;
  }
  else {
    rectTemp.top = m_ptStart.y;
    rectTemp.bottom = m_ptEnd.y;
  }

  if ( !rectTemp.PtInRect( pt ) ) return( FALSE );

  a = m_ptEnd.x - m_ptStart.x;
  b = m_ptEnd.y - m_ptStart.y;
  d   = a / b;
  rect.left = pt.x - m_ulWidth;
  rect.right = pt.x + m_ulWidth;
  rect.top = pt.y - m_ulWidth;
  rect.bottom = pt.y + m_ulWidth;
  if ( abs((int)d) > 0 ) {
    ptTemp.y = m_ptStart.y - ((m_ptStart.x-pt.x)*(m_ptStart.y-m_ptEnd.y))/(m_ptStart.x-m_ptEnd.x);
    ptTemp.x = pt.x;
    if ( rect.PtInRect(ptTemp) ) return( TRUE );
  }
  else {
    ptTemp.y = m_ptStart.x - ((m_ptStart.y-pt.y)*(m_ptStart.x-m_ptEnd.x))/(m_ptStart.y-m_ptEnd.y);
    ptTemp.y = pt.y;
    if ( rect.PtInRect(ptTemp) ) return( TRUE );
  }
  return(FALSE);
}

const CString& CObjectLine::GetClassName( void ) {
  static CString s_strClassName = "Line";

  return(s_strClassName);
}

bool CObjectLine::ExectiveDynLink( void ) {
  return( TRUE );
}

void CObjectLine::ToShowStatic( CDC * const pdc, CPoint  ) { 
  CPen cp, * cpOld;
  COLORREF clrBk = pdc->GetBkColor();
  CPoint pt = GetOffset();
  CSize sizeDiffer;
  CPoint ptStart = m_ptStart, ptEnd = m_ptEnd;

  ptStart += pt;
  ptEnd += pt;
  pdc->MoveTo(ptStart);
  cp.CreatePen( PS_SOLID, m_ulWidth, m_clrBkGrd );
  cpOld = pdc->SelectObject( &cp );
  pdc->SetBkColor( m_clrBkGrd );
  pdc->LineTo(ptEnd);     
  pdc->SetBkColor( clrBk );
  pdc->SelectObject( cpOld );

	if ( m_fSelected ) SetFocus( pdc );
}   

void CObjectLine::ToShowDynamic( CDC * const pdc ) { 
  CPen cp, * cpOld;
  COLORREF clrBk = pdc->GetBkColor();
  CPoint ptStart = m_ptStart, ptEnd = m_ptEnd;

  pdc->MoveTo(ptStart);
  cp.CreatePen( PS_SOLID, m_ulWidth, m_clrBkGrd );
  cpOld = pdc->SelectObject( &cp );
  pdc->SetBkColor( m_clrBkGrd );
  pdc->LineTo(ptEnd);     
  pdc->SetBkColor( clrBk );
  pdc->SelectObject( cpOld );
  m_fDrawAll = FALSE;
  m_fNeedUpdate = FALSE;
}

void CObjectLine::SetFocus( CDC * pdc) {
  int left, top, bottom,  right;
  CRect arrayRect[2];
                        
  left   = m_ptStart.x;
  top    = m_ptStart.y;
  bottom = m_ptEnd.y;
  right  = m_ptEnd.x;
                          
  arrayRect[0].bottom = top - 1;
  arrayRect[0].top    = top - 11;
  arrayRect[0].right  = left - 1;
  arrayRect[0].left   = left - 11;     
  
  arrayRect[1].bottom = bottom + 11;
  arrayRect[1].top    = bottom + 1;
  arrayRect[1].right  = right + 11;
  arrayRect[1].left   = right + 1; 
  
  pdc->InvertRect(arrayRect[0]);
  pdc->InvertRect(arrayRect[1]);
}  

bool CObjectLine::SetProperty( void ) {
  CDlgSetLineProperty CDlg;
  CDlg.SetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_lScanRate, m_ulWidth );
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_lScanRate, m_ulWidth );
    return( TRUE );
  }
  return( FALSE );
}

bool CObjectLine::CheckSelf( void ) {
  CString str;

  for( const auto pc : m_listDynLink ) {
  }
  return( TRUE );
}

bool CObjectLine::IsRectShape( void ) {
  return( FALSE );
}

  
