#include "stdafx.h"

#include "cObjText.h"
#include "DlgObjText.h"

IMPLEMENT_SERIAL(CObjectText, CObjRectBase, 1 | VERSIONABLE_SCHEMA);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

ParaName CObjectText::sm_ptrParaName[] =
{ {"Content", tINPUT | tMODIFIABLE | tSTRING,  0},
	{""     , 0, 1},
};

INT32 CObjectText::sm_aulSuitable[] = { -1, -1 };

const ULONG CObjectText::sm_ulDoubleEnd = 0;
const ULONG CObjectText::sm_ulWordEnd = 0;
const ULONG CObjectText::sm_ulStringEnd = 1;


CObjectText::CObjectText( CString s, CRect r ) : CObjRectBase( s, r ) {
	m_strShowName = s;
  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectText::CObjectText( void ) : CObjRectBase( ) {
  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}
													  
CObjectText::~CObjectText( ) {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);

}  

/////////////////////////////////////////////////////////////////////////////
// CObjectText diagnostics

#ifdef _DEBUG
void CObjectText::AssertValid() const
{
	CObjRectBase::AssertValid();
}

void CObjectText::Dump(CDumpContext& dc) const
{
	CObjRectBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectText member function

void CObjectText::Serialize( CArchive& ar ) {
	CObjRectBase::Serialize( ar );
  if( ar.IsStoring() ) {
  	ar << m_strShowName;
  }
  else {
  	ar >> m_strShowName;
  }
}

const CString& CObjectText::GetClassName( void ) {
  static CString s_strClassName = "Text";

  return(s_strClassName);
}

void CObjectText::ToShowStatic( CDC * const pdc, CPoint  ) { 
  COLORREF clrFore, clrBk;
	UINT uTextAlign;
  CRect rectArea = m_rectArea;
  CPoint pt = GetOffset();

  rectArea += pt;
  if ( pdc->RectVisible(rectArea) ) {		// need to redraw ?
		clrFore = pdc->SetTextColor( m_clrForeGrd );
		clrBk = pdc->SetBkColor( m_clrBkGrd );
  	uTextAlign = pdc->SetTextAlign( TA_CENTER | TA_BASELINE );
    pdc->ExtTextOut( rectArea.left + rectArea.Width()/2, rectArea.top + m_rectArea.Height()/2+5, ETO_CLIPPED, 
     								 rectArea,  m_strShowName, m_strShowName.GetLength(), nullptr);
		pdc->SetTextAlign( uTextAlign );
		pdc->SetTextColor( clrFore );
		pdc->SetBkColor( clrBk );
  }
	if ( m_fSelected ) SetFocus( pdc );
}  

void CObjectText::ToShowDynamic( CDC * const pdc ) { 
  COLORREF clrFore, clrBk;
	UINT uTextAlign;
  CBrush cbf;
  CRect rectUpdate;
  CPoint pt = GetOffset();
  CRect rectArea = m_rectArea;

  m_rectLastTime = m_rectArea;
  rectArea -= rectArea.TopLeft();
  cbf.CreateSolidBrush( RGB(255, 255, 255) );
  pdc->FillRect( rectArea, &cbf );
	clrFore = pdc->SetTextColor( m_clrForeGrd );
	clrBk = pdc->SetBkColor( m_clrBkGrd );
  uTextAlign = pdc->SetTextAlign( TA_CENTER | TA_BASELINE );
  pdc->ExtTextOut( rectArea.left + rectArea.Width()/2, rectArea.top + m_rectArea.Height()/2+5, ETO_CLIPPED, 
     							 rectArea,  m_strShowName, m_strShowName.GetLength(), nullptr);
	pdc->SetTextAlign( uTextAlign );
	pdc->SetTextColor( clrFore );
	pdc->SetBkColor( clrBk );
	m_fDrawAll = FALSE;
  m_fNeedUpdate = FALSE;
}  

void CObjectText::SetShowName( CString const strShowName ) {
	m_strShowName = strShowName;					   
}

CString CObjectText::GetShowName( void ) const {
	return( m_strShowName ); 
}

bool CObjectText::SetProperty( void ) {
	CDlgSetTextProperty CDlg;
	CDlg.SetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_strShowName, m_lScanRate );
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_strShowName, m_lScanRate );
    return( TRUE );
  }
  return( FALSE );
}

  
