#include "stdafx.h"

#include "CObjSymbol.h"
#include "cObjOval.h"

IMPLEMENT_SERIAL(CObjectOval, CObjRectBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

CObjectOval::CObjectOval(CString s, CRect r) : CObjRectBase(s, r) {
  m_fCreateMemoryDC = FALSE;

}

CObjectOval::CObjectOval( void ) : CObjRectBase( ) {
  m_fCreateMemoryDC = FALSE;
}
                                   
CObjectOval::~CObjectOval() {
}

/////////////////////////////////////////////////////////////////////////////
// CObjectOval diagnostics

#ifdef _DEBUG
void CObjectOval::AssertValid() const
{
  CObjRectBase::AssertValid();
}

void CObjectOval::Dump(CDumpContext& dc) const
{
  CObjRectBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectRect member function

void CObjectOval::Serialize( CArchive& ar ) {
  CObjRectBase::Serialize( ar );
  if( ar.IsStoring() ) {
  // do nothing
  }
  else {
		// Create clip reign
    m_rgnClip.DeleteObject();
    m_rgnClip.CreateEllipticRgn(m_rectArea.left, m_rectArea.top, m_rectArea.right, m_rectArea.bottom);
  }
} 

const CString& CObjectOval::GetClassNameStr( void ) {
  static CString s_strClassName = "Oval";

  return(s_strClassName);
}

void CObjectOval::ToShowStatic( CDC * const pdc, CPoint  ) {
  CBrush cbf, cbb, cbWindow, * pcb;
  CPen * pcp;
  CPen cpb( PS_SOLID, 1, m_clrBkGrd );
  CRect rectArea = m_rectArea;
  CPoint pt = GetOffset();

  rectArea += pt;
  if ( pdc->RectVisible( rectArea ) ) {
    cbb.CreateSolidBrush( m_clrBkGrd );
    cbWindow.CreateSolidBrush( RGB(255, 255, 255) );
    pcb = pdc->SelectObject( &cbb );    
    pcp = pdc->SelectObject( &cpb );
    pdc->Ellipse(rectArea);             // Yes, to draw myself
    pdc->SelectObject( pcp );
    pdc->SelectObject( pcb );
  }
	if ( m_fSelected ) SetFocus( pdc );
}     

void CObjectOval::ToShowDynamic( CDC * const pdc ) {
  CBrush cbf, cbb, cbWindow, * pcb;
  CPen * pcp;
  CPen cpb( PS_SOLID, 1, m_clrBkGrd );
  COLORREF clrFore, clrBk;
  CRect rectArea = m_rectArea;

  m_rectLastTime = m_rectArea;
  cbb.CreateSolidBrush( m_clrBkGrd );
  cbWindow.CreateSolidBrush( RGB(255, 255, 255) );
  pcb = pdc->SelectObject( &cbb );     
  pcp = pdc->SelectObject( &cpb );
  pdc->Ellipse(rectArea);             // Yes, to draw myself
  if ( !m_strTitle.IsEmpty() ) {
    clrFore = pdc->GetTextColor();
    clrBk = pdc->GetBkColor();
    pdc->SetTextColor( m_clrForeGrd );
    pdc->SetBkColor( m_clrBkGrd );
    pdc->ExtTextOut( rectArea.left+10, rectArea.top + rectArea.Height()/2, 
                     ETO_CLIPPED, rectArea, m_strTitle, m_strTitle.GetLength(), nullptr );
    pdc->SetBkColor( clrBk );
    pdc->SetTextColor( clrFore );
  }
  pdc->SelectObject( pcp );
  pdc->SelectObject( pcb );
  m_fDrawAll = FALSE;
  m_fNeedUpdate = FALSE;
}

CRgn * CObjectOval::GetClipRgn( const CPoint& ptScrollPosition ) {
  CRect rect;
  CPoint pt = GetOffset();
  m_rgnClip.DeleteObject();
  rect = m_rectArea;
  rect += pt - ptScrollPosition;
  m_rgnClip.CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom);
  return( &m_rgnClip );
}

bool CObjectOval::IsRectShape( void ) {
  return( FALSE );
}
