#include "stdafx.h"

#include "cObjectRect.h"

#include "CObjSymbol.h"

IMPLEMENT_SERIAL(CObjectRect, CObjRectBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif  

ParaName CObjectRect::sm_ptrParaName[] =
{ {"Visibility", tINPUT | tMODIFIABLE | tBOOL, 0},
	{"Height", tINPUT | tMODIFIABLE | tWORD | tDOUBLE,  1},
	{"Width",  tINPUT | tMODIFIABLE | tWORD | tDOUBLE,  2},
	{"BackColor",tINPUT | tMODIFIABLE | tWORD,  3},
	{"ForeColor",tINPUT | tMODIFIABLE | tWORD,  4},
	{"Title", tINPUT | tMODIFIABLE | tDOUBLE | tWORD | tBOOL | tSTRING,  5},
	{""     , 0, 6},
};

INT32 CObjectRect::sm_aulSuitable[] = { -1, -1, -1, -1, -1, -1, -1 };

const ULONG CObjectRect::sm_ulDoubleEnd = 0;
const ULONG CObjectRect::sm_ulBoolEnd = 1;
const ULONG CObjectRect::sm_ulWordEnd = 4;
const ULONG CObjectRect::sm_ulStringEnd = 5;


CObjectRect::CObjectRect(CString s, CRect r) : CObjRectBase(s, r) {
  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}

}

CObjectRect::CObjectRect( void ) : CObjRectBase( ) {
  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
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
	CObjectDynLink * pc;
	CString str;

	for(const auto pc : m_listDynLink) {
	}
	return( TRUE );
}

ParaName* CObjectRect::GetParaNameAddress(void) {
	return(sm_ptrParaName);
}

CString CObjectRect::GetParaName(ULONG index) {
	ASSERT(index <= CObjectRect::sm_ulStringEnd);
	return(CObjectRect::sm_ptrParaName[index].Name);
}

ULONG CObjectRect::GetDynLinkType(ULONG ulIndex) {
	return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

void CObjectRect::SelectParameter(ULONG ulType) {
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

INT32 CObjectRect::GetIndex(ULONG ulIndex) {
	return(CObjectRect::sm_aulSuitable[ulIndex]);
}
