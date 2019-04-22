#include "stdafx.h"

#include "CObjSymbol.h"
#include "cObjOval.h"

IMPLEMENT_SERIAL(CObjectOval, CObjRectBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

ParaName CObjectOval::sm_ptrParaName[] =
{ {"Visibility", tINPUT | tMODIFIABLE | tBOOL, 0},
	{"Height", tINPUT | tMODIFIABLE | tWORD | tDOUBLE,  1},
	{"Width",  tINPUT | tMODIFIABLE | tWORD | tDOUBLE,  2},
	{"BackColor",tINPUT | tMODIFIABLE | tWORD,  3},
	{"ForeColor",tINPUT | tMODIFIABLE | tWORD,  4},
	{"Title", tINPUT | tMODIFIABLE | tDOUBLE | tWORD | tBOOL | tSTRING,  5},
	{""     , 0, 6},
};

INT32 CObjectOval::sm_aulSuitable[] = { -1, -1, -1, -1, -1, -1, -1 };

const ULONG CObjectOval::sm_ulDoubleEnd = 0;
const ULONG CObjectOval::sm_ulBoolEnd = 1;
const ULONG CObjectOval::sm_ulWordEnd = 4;
const ULONG CObjectOval::sm_ulStringEnd = 5;


CObjectOval::CObjectOval(CString s, CRect r) : CObjRectBase(s, r) {
	m_fCreateMemoryDC = false;
	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectOval::CObjectOval( void ) : CObjRectBase( ) {
  m_fCreateMemoryDC = false;
	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}
                                   
CObjectOval::~CObjectOval() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
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

ParaName* CObjectOval::GetParaNameAddress(void) {
	return(sm_ptrParaName);
}

CString CObjectOval::GetParaName(ULONG index) {
	ASSERT(index <= CObjectOval::sm_ulStringEnd);
	return(CObjectOval::sm_ptrParaName[index].Name);
}

ULONG CObjectOval::GetDynLinkType(ULONG ulIndex) {
	return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

void CObjectOval::SelectParameter(ULONG ulType) {
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

INT32 CObjectOval::GetIndex(ULONG ulIndex) {
	return(CObjectOval::sm_aulSuitable[ulIndex]);
}

