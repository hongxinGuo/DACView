#include "stdafx.h"

#include "globedef.h"

#include "CUnitBase.h"

#include "cObjButton.h"
#include "DlgObjButton.h"

IMPLEMENT_SERIAL(CObjectButton, CObjRectBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

ParaName CObjectButton::sm_ptrParaName[] = 
    { {"Pressed", tOUTPUT | tMODIFIABLE | tBOOL,  0}, 
      {""     , 0, 1},
    }; 
    
INT32 CObjectButton::sm_aulSuitable[] = { -1, -1 };

const ULONG CObjectButton::sm_ulDoubleEnd = 0;
const ULONG CObjectButton::sm_ulWordEnd   = 1;
const ULONG CObjectButton::sm_ulStringEnd = 1;                

CObjectButton::CObjectButton(const CString& s, CRect r) : CObjRectBase(s, r) {
  m_fTrackMouseMove = false;
  m_fPressed = false;
  m_fBeginTrack = false;
  m_clrOn = RGB(255, 0, 0);
  m_clrOff = RGB(0, 0, 255);
  m_strOn = "ON";
  m_strOff = "OFF";

  m_fCreateMemoryDC = false;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectButton::CObjectButton( void ) : CObjRectBase( ) {
  m_fTrackMouseMove = false;
  m_fPressed = false;
  m_fBeginTrack = false;

  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectButton::~CObjectButton() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);

}

/////////////////////////////////////////////////////////////////////////////
// CObjectButton diagnostics

#ifdef _DEBUG
void CObjectButton::AssertValid() const
{
  CObjRectBase::AssertValid();
}

void CObjectButton::Dump(CDumpContext& dc) const
{
  CObjRectBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectRect member function

void CObjectButton::Serialize( CArchive& ar ) {
  CObjRectBase::Serialize( ar );
  if( ar.IsStoring() ) {
    ar << m_clrOn << m_clrOff << m_strOn << m_strOff;
  }
  else {
    ar >> m_clrOn >> m_clrOff >> m_strOn >> m_strOff;
  }
} 

const CString& CObjectButton::GetClassNameStr( void ) {
  static CString s_strClassName = "Button";
  return(s_strClassName);
}

bool CObjectButton::ExectiveDynLink( void ) {
  // Dynamic Link Button
  CUnitBase * pcunit;
  ULONG ulDestIndex;
    
  for ( const auto pcobjDynLink : m_listDynLink ) {
    pcunit = pcobjDynLink->GetUnit();                    
    ulDestIndex = pcobjDynLink->GetUnitIndex();   
    switch ( pcobjDynLink->GetLinkMethod() ) {
    case IDD_INPUT_BOOL :
      pcunit->SetBool(ulDestIndex, m_fPressed);
      break;
    default :
      ASSERT( 0 );  // error link method
      break;
    } // switch Link Method
  } // for
  return( TRUE ); 
}
        
bool CObjectButton::InIt( CDC * const, POINT const pt, int iStatus ) {
  bool fInIt = FALSE, fDo = FALSE;
  CPoint ptOffsetBase = GetOffset(), pt0(0, 0);
  CRect rectArea = m_rectArea;

  rectArea += ptOffsetBase;
  fInIt = rectArea.PtInRect(pt);
  switch ( iStatus ) {
  case 1 :  // mouse down
    if ( fInIt ) {
      m_fTrackMouseMove = TRUE;
      m_fBeginTrack = TRUE;
    }
    break;
  case 2 :  // mouse up
    if ( m_fBeginTrack ) {
      if ( fInIt ) {
        m_fTrackMouseMove = FALSE;
        if ( m_fPressed ) {
          m_fPressed = FALSE;
        }
        else {
          m_fPressed = TRUE;
        }
        fDo = TRUE;
      }
      m_fBeginTrack = FALSE;
    }
    break;
  case 0 :  // mouse move
    if ( m_fBeginTrack ) {
      if ( fInIt ) {
        m_fTrackMouseMove  = TRUE;
      }
      else {
        m_fTrackMouseMove = FALSE;
      }
    }
    break;
  case -1 : // don't change
    break;
  default :
    ;
  } // switch 
  
  // send data to linked unit
  if ( fDo ) ExectiveDynLink();   
  
  return( fInIt );
}
													  
void CObjectButton::ToShowStatic( CDC * const pdc, CPoint  ) {
  CPen p1(PS_SOLID, 1, RGB(255, 255, 255));
  CPen p2(PS_SOLID, 1, RGB(64, 64, 64)), p3(PS_SOLID, 1, RGB(0, 0, 0)), * pp;
  COLORREF clrFore, clrBk;
	UINT uTextAlign;
  CRect rectArea = m_rectArea;
  CPoint pt = GetOffset();

  rectArea += pt;
  if ( pdc->RectVisible(rectArea) ) {   // if I need to redraw ?
    CBrush cbb;
    // draw background
    cbb.CreateSolidBrush( RGB(192, 192, 192) );
    pdc->FillRect(rectArea, &cbb);
    int left = rectArea.left, right = rectArea.right - 1;
    int top = rectArea.top, bottom = rectArea.bottom - 1;
    // draw border
    pp = (CPen *)pdc->SelectObject(&p3);
    pdc->MoveTo(left, top);
    pdc->LineTo(right, top);
    pdc->LineTo(right, bottom);
    pdc->LineTo(left, bottom);
    pdc->LineTo(left, top);
    // draw inner border and show text
    clrFore = pdc->GetTextColor();
		clrBk = pdc->GetBkColor();
    pdc->SetTextColor(m_clrOff);
		uTextAlign = pdc->SetTextAlign( TA_CENTER | TA_BASELINE | TA_UPDATECP );
    if ( (m_fTrackMouseMove == FALSE) && (m_fPressed == FALSE) ) { // off status
      pdc->MoveTo(right-1, top+1);
      pdc->LineTo(right-1, bottom-1);
      pdc->LineTo(left+1, bottom-1);
      pdc->SelectObject(&p2);
      pdc->MoveTo(right-2, top+2);
      pdc->LineTo(right-2, bottom-2);
      pdc->LineTo(left+2, bottom-2);
      pdc->SelectObject(&p1);
      pdc->LineTo(left+2, top+2);
      pdc->LineTo(right-2, top+2);
      pdc->SetTextColor(m_clrOff);
			pdc->SetBkColor(m_clrBkGrd);
			uTextAlign = pdc->SetTextAlign( TA_CENTER | TA_BASELINE );
    	pdc->ExtTextOut( rectArea.left + rectArea.Width()/2, rectArea.top + m_rectArea.Height()/2, ETO_CLIPPED, 
      								 rectArea,  m_strOff, m_strOff.GetLength(), nullptr);
			pdc->SetTextAlign( uTextAlign );
			pdc->SetBkColor( clrBk );
			pdc->SetTextColor( clrFore );
    }
    else {    // on status
      pdc->MoveTo(left+1, bottom-1);
      pdc->LineTo(left+1, top+1);
      pdc->LineTo(right-1, top+1);
      pdc->SelectObject(&p1);
      pdc->MoveTo(right-2, top+2);
      pdc->LineTo(right-2, bottom-2);
      pdc->LineTo(left+2, bottom-2);
      pdc->SelectObject(&p2);
      pdc->LineTo(left+2, top+2);
      pdc->LineTo(right-2, top+2);
      pdc->SetTextColor(m_clrOn);
			pdc->SetBkColor( m_clrBkGrd );
			uTextAlign = pdc->SetTextAlign( TA_CENTER | TA_BASELINE );
    	pdc->ExtTextOut( rectArea.left + rectArea.Width()/2, rectArea.top + m_rectArea.Height()/2, ETO_CLIPPED,
      								 rectArea,  m_strOn, m_strOn.GetLength(), nullptr);
			pdc->SetTextAlign( uTextAlign );
			pdc->SetBkColor( clrBk );
			pdc->SetTextColor( clrFore );
    }
		pdc->SelectObject(pp);
  }
	if ( m_fSelected ) SetFocus( pdc );
}  
													  
void CObjectButton::ToShowDynamic( CDC * const pdc ) {
  CPen p1(PS_SOLID, 1, RGB(255, 255, 255));
  CPen p2(PS_SOLID, 1, RGB(64, 64, 64)), p3(PS_SOLID, 1, RGB(0, 0, 0)), * pp;
  COLORREF clrFore, clrBk;
	UINT uTextAlign;
  CPoint pt = GetOffset();
  CRect rectArea = m_rectArea, rectLastTime = m_rectLastTime;

  m_rectLastTime = m_rectArea;
  rectArea += pt;
  rectLastTime += pt;
  if ( pdc->RectVisible(rectArea) || pdc->RectVisible(rectLastTime) ) { // if I need to redraw ?
    CBrush cbb;
    cbb.CreateSolidBrush( RGB(192, 192, 192) );
    pdc->FillRect(rectArea, &cbb);
    int left = rectArea.left, right = rectArea.right - 1;
    int top = rectArea.top, bottom = rectArea.bottom - 1;
    pp = (CPen *)pdc->SelectObject(&p3);
    pdc->MoveTo(left, top);
    pdc->LineTo(right, top);
    pdc->LineTo(right, bottom);
    pdc->LineTo(left, bottom);
    pdc->LineTo(left, top);
    clrFore = pdc->GetTextColor();
		clrBk = pdc->GetBkColor();
    pdc->SetTextColor(m_clrOff);
		uTextAlign = pdc->SetTextAlign( TA_CENTER | TA_BASELINE | TA_UPDATECP );
    if ( (m_fTrackMouseMove == FALSE) && (m_fPressed == FALSE) ) { // off status
      pdc->MoveTo(right-1, top+1);
      pdc->LineTo(right-1, bottom-1);
      pdc->LineTo(left+1, bottom-1);
      pdc->SelectObject(&p2);
      pdc->MoveTo(right-2, top+2);
      pdc->LineTo(right-2, bottom-2);
      pdc->LineTo(left+2, bottom-2);
      pdc->SelectObject(&p1);
      pdc->LineTo(left+2, top+2);
      pdc->LineTo(right-2, top+2);
      pdc->SetTextColor(m_clrOff);
			pdc->SetBkColor(m_clrBkGrd);
			uTextAlign = pdc->SetTextAlign( TA_CENTER | TA_BASELINE );
    	pdc->ExtTextOut( rectArea.left + rectArea.Width()/2, rectArea.top + m_rectArea.Height()/2, ETO_CLIPPED, 
      								 rectArea,  m_strOff, m_strOff.GetLength(), nullptr);
			pdc->SetTextAlign( uTextAlign );
			pdc->SetBkColor( clrBk );
			pdc->SetTextColor( clrFore );
    }
    else {    // on status
      pdc->MoveTo(left+1, bottom-1);
      pdc->LineTo(left+1, top+1);
      pdc->LineTo(right-1, top+1);
      pdc->SelectObject(&p1);
      pdc->MoveTo(right-2, top+2);
      pdc->LineTo(right-2, bottom-2);
      pdc->LineTo(left+2, bottom-2);
      pdc->SelectObject(&p2);
      pdc->LineTo(left+2, top+2);
      pdc->LineTo(right-2, top+2);
      pdc->SetTextColor(m_clrOn);
			pdc->SetBkColor( m_clrBkGrd );
			uTextAlign = pdc->SetTextAlign( TA_CENTER | TA_BASELINE );
    	pdc->ExtTextOut( rectArea.left + rectArea.Width()/2, rectArea.top + m_rectArea.Height()/2, ETO_CLIPPED,
      								 rectArea,  m_strOn, m_strOn.GetLength(), nullptr);
			pdc->SetTextAlign( uTextAlign );
			pdc->SetBkColor( clrBk );
			pdc->SetTextColor( clrFore );
    }
		pdc->SelectObject(pp);
  }
	m_fDrawAll = FALSE;
  m_fNeedUpdate = FALSE;  // have updated
}

ParaName* CObjectButton::GetParaNameAddress( void ) {
  return( sm_ptrParaName );
}  

CString CObjectButton::GetParaName( ULONG index ) {
  ASSERT( index <= CObjectButton::sm_ulStringEnd );
  return( CObjectButton::sm_ptrParaName[index].Name );
}

ULONG CObjectButton::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

void CObjectButton::SelectParameter(ULONG ulType) {
  int i = 0;
  int j = 0;
 
  for (int k = 0; k <= sm_ulStringEnd; k++) {
    sm_aulSuitable[k] = -1;
  }
  while ( sm_ptrParaName[i].ulType != 0 ) {
    if ( (sm_ptrParaName[i].ulType | ulType) == sm_ptrParaName[i].ulType ) {
      if ( ulType & tINPUT ) {
        if ( m_vfSelected[i] == false ) {
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

INT32 CObjectButton::GetIndex( ULONG ulIndex ) {
  return( CObjectButton::sm_aulSuitable[ulIndex] );
}

bool CObjectButton::SetProperty( void ) {
	CDlgSetButtonProperty CDlg;
  CDlg.SetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_strOn, m_strOff, m_lScanRate );
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_strOn, m_strOff, m_lScanRate );
    m_fCreateMemoryDC = FALSE;
    return( TRUE );
  }
  return( FALSE );
}

bool CObjectButton::CheckSelf( void ) {
	CString str;

	for( const auto pc : m_listDynLink ) {
		if ( pc->GetLinkMethod() != IDD_INPUT_BOOL ) {
			ShowMessage(ID_WARN_LINK_METHOD_MISMATCH, (LPCTSTR)m_strName);
			pc->SetLinkMethod( IDD_INPUT_BOOL );
		}
	}
	return( TRUE );
}

bool CObjectButton::CanInSymbol( void ) {
  return( FALSE );
}

