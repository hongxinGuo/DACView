#include "stdafx.h"

#include "cObjRect.h"

#include "CObjSymbol.h"

IMPLEMENT_SERIAL(CObjectRect, CObjRectBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

CObjectRect::CObjectRect(CString s, CRect r) : CObjRectBase(s, r) {
  m_fCreateMemoryDC = FALSE;
}

CObjectRect::CObjectRect( void ) : CObjRectBase( ) {
  m_fCreateMemoryDC = FALSE;
}
  
CObjectRect::~CObjectRect() {
}

/////////////////////////////////////////////////////////////////////////////
// CObjectRect diagnostics

#ifdef _DEBUG
void CObjectRect::AssertValid() const
{
  CObjRectBase::AssertValid();
}

void CObjectRect::Dump(CDumpContext& dc) const
{
  CObjRectBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectRect member function

void CObjectRect::Serialize( CArchive& ar ) {
  CObjRectBase::Serialize( ar );
  if( ar.IsStoring() ) {
  // do nothing
  }
  else {
  // do nothing
  }
} 

const CString& CObjectRect::GetClassNameStr( void ) {
  static CString s_strClassName = "Rect";

  return(s_strClassName);
}

void CObjectRect::ToShowStatic( CDC * const pdc, CPoint  ) {
  CBrush cbf, cbb;
  COLORREF clrFore;
  COLORREF clrBk;
  CPoint pt = GetOffset();
  CRect rectArea = m_rectArea;

  rectArea += pt;
  if ( pdc->RectVisible( rectArea ) ) { 
    cbb.CreateSolidBrush( m_clrBkGrd );
    pdc->FillRect(rectArea, &cbb);           // Yes, to draw myself
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
  }
	if ( m_fSelected ) SetFocus( pdc );
}  

void CObjectRect::ToShowDynamic( CDC * const pdc ) {
  CBrush cbf, cbb;
  COLORREF clrFore;
  COLORREF clrBk;
  
  CRect rectArea = m_rectArea;
  m_rectLastTime = m_rectArea;

  cbb.CreateSolidBrush( m_clrBkGrd );
  pdc->FillRect(rectArea, &cbb);           // Yes, to draw myself
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
  m_fDrawAll = FALSE;
  m_fNeedUpdate = FALSE;
}

bool CObjectRect::CheckSelf( void ) {
	POSITION poDL = m_listDynLink.GetHeadPosition();
	INT_PTR i, iCount = m_listDynLink.GetCount();
	CObjectDynLink * pc;
	CString str;

	for( i = 0; i < iCount; i++ ) {
		pc = m_listDynLink.GetNext( poDL );
	}
	return( TRUE );
}