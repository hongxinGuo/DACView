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

ParaName CObjectRoundRect::sm_ptrParaName[] =
{ {"Visibility", tINPUT | tMODIFIABLE | tBOOL, 0},
	{"Height", tINPUT | tMODIFIABLE | tWORD | tDOUBLE,  1},
	{"Width",  tINPUT | tMODIFIABLE | tWORD | tDOUBLE,  2},
	{"BackColor",tINPUT | tMODIFIABLE | tWORD,  3},
	{"ForeColor",tINPUT | tMODIFIABLE | tWORD,  4},
	{"Title", tINPUT | tMODIFIABLE | tDOUBLE | tWORD | tBOOL | tSTRING,  5},
	{""     , 0, 6},
};

INT32 CObjectRoundRect::sm_aulSuitable[] = { -1, -1, -1, -1, -1, -1, -1 };

const ULONG CObjectRoundRect::sm_ulDoubleEnd = 0;
const ULONG CObjectRoundRect::sm_ulBoolEnd = 1;
const ULONG CObjectRoundRect::sm_ulWordEnd = 4;
const ULONG CObjectRoundRect::sm_ulStringEnd = 5;


CObjectRoundRect::CObjectRoundRect(CString s, CRect r) : CObjRectBase(s, r) {
  m_ulRound = min(r.Height(), r.Width()) / 4;
  
  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectRoundRect::CObjectRoundRect( void ) : CObjRectBase( ) {
	m_ulRound = 4;

  m_fCreateMemoryDC = FALSE;
	
	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}
  
CObjectRoundRect::~CObjectRoundRect() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
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

const CString& CObjectRoundRect::GetClassName( void ) {
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

ParaName* CObjectRoundRect::GetParaNameAddress(void) {
	return(sm_ptrParaName);
}

CString CObjectRoundRect::GetParaName(ULONG index) {
	ASSERT(index <= CObjectRoundRect::sm_ulStringEnd);
	return(CObjectRoundRect::sm_ptrParaName[index].Name);
}

ULONG CObjectRoundRect::GetDynLinkType(ULONG ulIndex) {
	return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

void CObjectRoundRect::SelectParameter(ULONG ulType) {
	int i = 0;
	int j = 0;

	for (int k = 0; k <= sm_ulStringEnd; k++) {
		sm_aulSuitable[k] = -1;
	}
	while (sm_ptrParaName[i].ulType != 0) {
		if ((sm_ptrParaName[i].ulType | ulType) == sm_ptrParaName[i].ulType) {
			if (ulType & tINPUT) {
				if (m_vfSelected[i] == FALSE) {
					sm_aulSuitable[j++] = sm_ptrParaName[i].ulIndex;
				}
			}
			else {
				sm_aulSuitable[j++] = sm_ptrParaName[i].ulIndex;
			}
		}
		i++;
	}
}

INT32 CObjectRoundRect::GetIndex(ULONG ulIndex) {
	return(CObjectRoundRect::sm_aulSuitable[ulIndex]);
}

