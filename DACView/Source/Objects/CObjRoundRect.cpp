#include "stdafx.h"

#include "cobjRoundRect.h"
#include "DlgObjRoundRect.h"

#include "CObjSymbol.h"

IMPLEMENT_SERIAL(CObjectRoundRect, CObjRectBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

CObjectRoundRect::CObjectRoundRect(CString s, CRect r) : CObjRectBase(s, r) {
  m_ulRound = min(r.Height(), r.Width()) / 4;
  
  m_fCreateMemoryDC = FALSE;

}

CObjectRoundRect::CObjectRoundRect( void ) : CObjRectBase( ) {
	m_ulRound = 4;

  m_fCreateMemoryDC = FALSE;

}
  
CObjectRoundRect::~CObjectRoundRect() {
}

/////////////////////////////////////////////////////////////////////////////
// CObjectRoundRect diagnostics

#ifdef _DEBUG
void CObjectRoundRect::AssertValid() const
{
  CObjRectBase::AssertValid();
}

void CObjectRoundRect::Dump(CDumpContext& dc) const
{
  CObjRectBase::Dump(dc);
  dc << "\nm_ulRound = " << m_ulRound << "\n";
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectRect member function

void CObjectRoundRect::Serialize( CArchive& ar ) {
  CObjRectBase::Serialize( ar );
  if( ar.IsStoring() ) {
    ar << m_ulRound;
  }
  else {
    ar >> m_ulRound;
    m_rgnClip.DeleteObject();
    m_rgnClip.CreateRoundRectRgn(m_rectArea.left, m_rectArea.top, 
                                 m_rectArea.right, m_rectArea.bottom, m_ulRound, m_ulRound);
  
  }
}   

const CString& CObjectRoundRect::GetClassNameStr( void ) {
  static CString s_strClassName = "RoundRect";

  return(s_strClassName);
}

void CObjectRoundRect::ToShowStatic( CDC * const pdc, CPoint  ) {
  POINT ptTemp;
  CBrush cbWindow, cbb, cbf, * pcb;
  CPen * pcp;
  CPen cpb( PS_SOLID, 1, m_clrBkGrd );
  CRect rectArea = m_rectArea;
  CPoint pt = GetOffset();

  rectArea += pt;
  if ( pdc->RectVisible( rectArea ) ) {
    cbb.CreateSolidBrush( m_clrBkGrd );
    ptTemp.x = ptTemp.y = m_ulRound;
    pcb = pdc->SelectObject( &cbb );    
    pcp = pdc->SelectObject( &cpb );
    pdc->RoundRect(rectArea, ptTemp);           // Yes, to draw myself
    pdc->SelectObject( pcp );
    pdc->SelectObject( pcb );
  }
	if ( m_fSelected ) SetFocus( pdc );
}   

void CObjectRoundRect::ToShowDynamic( CDC * const pdc ) {
  POINT ptTemp;
  CBrush cbWindow, cbb, cbf, * pcb;
  COLORREF clrBk;
  CPen * pcp;
  CPen cpb( PS_SOLID, 1, m_clrBkGrd );
  COLORREF clrFore;
  CRect rectArea = m_rectArea, rectLastTime = m_rectLastTime;

  m_rectLastTime = m_rectArea;
  
  cbb.CreateSolidBrush( m_clrBkGrd );
  ptTemp.x = ptTemp.y = m_ulRound;
  pcb = pdc->SelectObject( &cbb );
  pcp = pdc->SelectObject( &cpb );
  pdc->RoundRect(rectArea, ptTemp);
  if ( !m_strTitle.IsEmpty() ) {
    clrFore = pdc->GetTextColor();
    clrBk = pdc->GetBkColor();
    pdc->SetTextColor( m_clrForeGrd );
    pdc->SetBkColor( m_clrBkGrd );
    pdc->ExtTextOut( rectArea.left+3, rectArea.top+rectArea.Height()/2, 
                     ETO_CLIPPED, rectArea, m_strTitle, m_strTitle.GetLength(), nullptr );
    pdc->SetBkColor( clrBk );
    pdc->SetTextColor( clrFore );
  }
  pdc->SelectObject( pcp );
  pdc->SelectObject( pcb );
  m_fDrawAll = FALSE;
  m_fNeedUpdate = FALSE;
}


CRgn * CObjectRoundRect::GetClipRgn( const CPoint& ptScrollPosition ) {
  CRect rect;
  CPoint pt = GetOffset();
  m_rgnClip.DeleteObject();
  rect = m_rectArea;
  rect += pt - ptScrollPosition;
  m_rgnClip.CreateRoundRectRgn(rect.left, rect.top, rect.right, rect.bottom, m_ulRound, m_ulRound);
  return( &m_rgnClip );
}

bool CObjectRoundRect::SetProperty( void ) {
  CDlgSetRoundRectProperty CDlg;
  CDlg.SetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_ulRound, m_lScanRate );
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_ulRound, m_lScanRate );
    return( TRUE );
  }
  return( FALSE );
}

bool CObjectRoundRect::IsRectShape( void ) {
  return( FALSE );
}
