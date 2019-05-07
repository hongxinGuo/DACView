// cFillBar.cpp implement CObjectFillBar class

#include "stdafx.h"

#include "globedef.h"

#include "CUnitBase.h"
												 
#include "cObjFillBar.h"
#include "DlgObjBar.h"

const int c_iSliderWidth = 30;

IMPLEMENT_SERIAL(CObjectFillBar, CObjRectBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

ParaName CObjectFillBar::sm_ptrParaName[] = 
    { {"Bar", tINPUT | tMODIFIABLE | tDOUBLE | tWORD,  0}, 
      {""     , 0, 1},
    }; 
    
INT32 CObjectFillBar::sm_aulSuitable[] = { -1 };

const ULONG CObjectFillBar::sm_ulDoubleEnd = 1;
const ULONG CObjectFillBar::sm_ulWordEnd   = 1;
const ULONG CObjectFillBar::sm_ulStringEnd = 1;                

CObjectFillBar::CObjectFillBar(const CString& s, CRect r) : CObjRectBase(s, r) {
	m_fVertical = TRUE;
	m_fMoveMark = FALSE;
	m_fShowRange = FALSE;
	m_fBeginTrack = FALSE;
	m_eLoLimit = 0.0;
	m_eMarkPosition = 0.0;
	m_eHiLimit = 1.0;
	m_fCreateMemoryDC = FALSE;
	m_fPressed = false;
	m_clrBar = RGB(255, 0, 0);

	if ( m_rectArea.Width() < c_iSliderWidth * 2 ) m_rectArea.right = m_rectArea.left + c_iSliderWidth*2;
	if ( m_rectArea.Height() < c_iSliderWidth * 2 ) m_rectArea.bottom = m_rectArea.top + c_iSliderWidth*2;

	m_rectFillBar.left = m_rectArea.left + 30;
	m_rectFillBar.right = m_rectArea.right - 10;
	m_rectFillBar.top = m_rectArea.top + 20;
	m_rectFillBar.bottom = m_rectArea.top + 20;

  sm_aulSuitable[0] = -1;

  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectFillBar::CObjectFillBar( void ) : CObjRectBase( ) {
	m_fVertical = TRUE;
	m_fMoveMark = FALSE;
	m_fShowRange = FALSE;
	m_fBeginTrack = FALSE;

  m_eLoLimit = 0.0;
	m_eMarkPosition = 0.0;
	m_eHiLimit = 1.0;
	m_fCreateMemoryDC = FALSE;
	m_fPressed = false;

  sm_aulSuitable[0] = -1;

  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectFillBar::~CObjectFillBar() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}

/////////////////////////////////////////////////////////////////////////////
// CObjectFillBar diagnostics

#ifdef _DEBUG
void CObjectFillBar::AssertValid() const
{
  CObjRectBase::AssertValid();
}

void CObjectFillBar::Dump(CDumpContext& dc) const
{
  CObjRectBase::Dump(dc);
  dc << "\nm_eLoLimit = " << m_eLoLimit;
  dc << "\nm_eHiLimit = " << m_eHiLimit;
  if ( m_fVertical ) dc << "\nVertical"; 
  else dc << "\nHorizonal";
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectRect member function

void CObjectFillBar::Serialize( CArchive& ar ) {
  CObjRectBase::Serialize( ar );
  LONG a, b, c;							 
  if( ar.IsStoring() ) {
		ar << m_eLoLimit << m_eHiLimit << (ULONG)m_fVertical 
			 << (ULONG)m_fMoveMark << (ULONG)m_fShowRange << m_clrBar << m_rectFillBar;
  }
  else {
    ar >> m_eLoLimit >> m_eHiLimit >> a 
				>> b >> c >> m_clrBar >> m_rectFillBar;
		m_fVertical = (bool)a;
		m_fMoveMark = (bool)b;
		m_fMoveMark = (bool)c;

		if ( (m_eMarkPosition < m_eLoLimit) || (m_eMarkPosition > m_eHiLimit) ) {
			m_eMarkPosition = (m_eHiLimit + m_eLoLimit)/2;
		}
		if ( m_rectArea.Width() < c_iSliderWidth*2 ) m_rectArea.right = m_rectArea.left + c_iSliderWidth*2;
		if ( m_rectArea.Height() < c_iSliderWidth*2 ) m_rectArea.bottom = m_rectArea.top + c_iSliderWidth*2;
		if ( m_fVertical ) {
			m_rectFillBar.left = m_rectArea.left + 30;
			m_rectFillBar.right = m_rectArea.right - 10;
			m_rectFillBar.top = m_rectArea.top + 20;
			m_rectFillBar.bottom = m_rectArea.top + 20;
		}
		else {
			m_rectFillBar.left = m_rectArea.left + 20;
			m_rectFillBar.right = m_rectArea.right - 20;
			m_rectFillBar.top = m_rectArea.top + 10;
			m_rectFillBar.bottom = m_rectArea.top + 30;
		}
  }
} 

const CString& CObjectFillBar::GetClassName( void ) {
  static CString s_strClassName = "FillBar";

  return(s_strClassName);
}

bool CObjectFillBar::ExectiveDynLink( ) {
	CUnitBase * pcunit;
	ULONG ulSourceIndex, ulDestIndex;
		
	for ( const auto pcobjDynLink : m_listDynLink) {
		pcunit = pcobjDynLink->GetUnit();                    
		ulSourceIndex = pcobjDynLink->GetUnitIndex();
		ulDestIndex = pcobjDynLink->GetObjectIndex();		
		switch ( pcobjDynLink->GetLinkMethod() ) {
    case IDD_OUTPUT_FILLBAR :
		  SetDouble(ulDestIndex, pcunit->GetDouble(ulSourceIndex));
      break;
    default :
      ASSERT( 0 );		// error
    }
	}	// for 
  return( TRUE );
}

void CObjectFillBar::ToShowStatic( CDC * const pdc, CPoint  ) {
	CPen p1(PS_SOLID, 1, RGB(255, 255, 255)), p2(PS_SOLID, 1, RGB(64, 64, 64)), cpMark, * pcp;
	CPen p3(PS_SOLID, 1, RGB(0, 0, 0)), * pp;
	CBrush cbf, cbb, cbMark;
	CRect rectScroll;
  CRect rectArea = m_rectArea;
  CPoint pt = GetOffset();

  rectArea += pt;
  cbb.CreateSolidBrush( RGB(192, 192, 192) );
	cbMark.CreateSolidBrush( m_clrBar );
	cpMark.CreatePen(PS_SOLID, 3, m_clrBar);
  					
	if ( pdc->RectVisible(rectArea) ) {
    pdc->FillRect(rectArea, &cbb);
    pp = pdc->SelectObject(&p3);
    pdc->MoveTo(rectArea.right-1, rectArea.top);
    pdc->LineTo(rectArea.right-1, rectArea.bottom-1);
    pdc->LineTo(rectArea.left, rectArea.bottom-1);
    pdc->SelectObject(&p2);
    pdc->MoveTo(rectArea.right-2, rectArea.top+1);
    pdc->LineTo(rectArea.right-2, rectArea.bottom-2);
    pdc->LineTo(rectArea.left+1, rectArea.bottom-2);
    pdc->SelectObject(&p1);
    pdc->LineTo(rectArea.left+1, rectArea.top+1);
    pdc->LineTo(rectArea.right-2, rectArea.top+1);
    pdc->SelectObject( pp );
		if ( m_fVertical ) {
			rectScroll.left = rectArea.left + 2;
			rectScroll.right = rectArea.right - 2;
			rectScroll.top = rectArea.top + 15;
			rectScroll.bottom = rectArea.bottom - 15;
			if ( m_fShowRange ) {
				rectScroll.left = rectArea.left + rectArea.Width()/2;
				if ( m_fMoveMark ) {
					rectScroll.bottom = rectScroll.bottom - (long)(rectScroll.Height() * (m_eMarkPosition - m_eLoLimit) /(m_eHiLimit - m_eLoLimit));
					pdc->MoveTo( rectScroll.left, rectScroll.bottom );
					pcp = pdc->SelectObject( &cpMark );
					pdc->LineTo( rectScroll.right, rectScroll.bottom );
					pdc->SelectObject( pcp );
				}
				else {
					rectScroll.top = rectScroll.bottom - (long)(rectScroll.Height() * m_eMarkPosition /(m_eHiLimit - m_eLoLimit));						
					pdc->FillRect( rectScroll, &cbMark );
				}
			}
			else {	// not show range
				if ( m_fMoveMark ) {
					rectScroll.bottom = rectScroll.bottom - (long)(rectScroll.Height() * (m_eMarkPosition - m_eLoLimit) /(m_eHiLimit - m_eLoLimit));
					pdc->MoveTo( rectScroll.left, rectScroll.bottom );
					pcp = pdc->SelectObject( &cpMark );
					pdc->LineTo( rectScroll.right, rectScroll.bottom );
					pdc->SelectObject( pcp );
				}
				else { // fill bar
					rectScroll.top = rectScroll.bottom - (long)(rectScroll.Height() * (m_eMarkPosition - m_eLoLimit) /(m_eHiLimit - m_eLoLimit));						
					pdc->FillRect( rectScroll, &cbMark );
				}
			}
		}
		else {	// Horizonal
			rectScroll.left = rectArea.left + 15;
			rectScroll.right = rectArea.right - 15;
			rectScroll.top = rectArea.top + 2;
			rectScroll.bottom = rectArea.bottom - 2;
			if ( m_fShowRange ) {
				rectScroll.top = rectArea.top + rectArea.Height()/2;
				if ( m_fMoveMark ) {
					rectScroll.left = rectScroll.left + (long)(rectScroll.Width() * (m_eMarkPosition - m_eLoLimit) /(m_eHiLimit - m_eLoLimit));
					pdc->MoveTo( rectScroll.left, rectScroll.top );
					pcp = pdc->SelectObject( &cpMark );
					pdc->LineTo( rectScroll.left, rectScroll.bottom );
					pdc->SelectObject( pcp );
				}
				else {	// fill bar
					rectScroll = rectScroll;
					rectScroll.right = rectScroll.left + (long)(rectScroll.Width() * (m_eMarkPosition - m_eLoLimit) /(m_eHiLimit - m_eLoLimit));
					pdc->FillRect( rectScroll, &cbMark );
				}
			}
			else { // not show range 
				if ( m_fMoveMark ) {
					rectScroll.left = rectScroll.left + (long)(rectScroll.Width() * (m_eMarkPosition - m_eLoLimit) /(m_eHiLimit - m_eLoLimit));
					pdc->MoveTo( rectScroll.left, rectScroll.top );
					pcp = pdc->SelectObject( &cpMark );
					pdc->LineTo( rectScroll.left, rectScroll.bottom );
					pdc->SelectObject( pcp );
				}
				else {	// fill bar
					rectScroll.right = rectScroll.left + (long)(rectScroll.Width() * (m_eMarkPosition - m_eLoLimit) /(m_eHiLimit - m_eLoLimit));						
					pdc->FillRect( rectScroll, &cbMark );
				}
			}
		}		// if Vertical
	}	// if rectVisible
	if ( m_fSelected ) SetFocus( pdc );
}  

void CObjectFillBar::ToShowDynamic( CDC * const pdc ) {
  CBitmap * pOldBitmap;
	CPen p1(PS_SOLID, 1, RGB(255, 255, 255)), p2(PS_SOLID, 1, RGB(64, 64, 64)), cpMark, * pcp;
	CPen p3(PS_SOLID, 1, RGB(0, 0, 0)), * pp;
	CBrush cbf, cbb, cbMark;
	CRect rectScroll;
	CFont *pcf;
	char cTemp[10];
	int i;
  CRect rectArea = m_rectArea, rectLastTime = m_rectLastTime;

  m_rectLastTime = m_rectArea;

  cbb.CreateSolidBrush( RGB(192, 192, 192) );
	cbMark.CreateSolidBrush( m_clrBar );
	cpMark.CreatePen(PS_SOLID, 3, m_clrBar);
  					
  if ( !m_fCreateMemoryDC ) {
    m_MemoryDC.CreateCompatibleDC( pdc );
		m_Bitmap.CreateCompatibleBitmap( pdc, rectArea.Width(), rectArea.Height());
		rectScroll.left = rectScroll.top = 0;
    rectScroll.bottom = rectArea.Height();
    rectScroll.right = rectArea.Width();
    pOldBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
    m_MemoryDC.FillRect(rectScroll, &cbb);
    pp = m_MemoryDC.SelectObject(&p3);
    m_MemoryDC.MoveTo(rectScroll.right-1, rectScroll.top);
    m_MemoryDC.LineTo(rectScroll.right-1, rectScroll.bottom-1);
    m_MemoryDC.LineTo(rectScroll.left, rectScroll.bottom-1);
    m_MemoryDC.SelectObject(&p2);
    m_MemoryDC.MoveTo(rectScroll.right-2, rectScroll.top+1);
    m_MemoryDC.LineTo(rectScroll.right-2, rectScroll.bottom-2);
    m_MemoryDC.LineTo(rectScroll.left+1, rectScroll.bottom-2);
    m_MemoryDC.SelectObject(&p1);
    m_MemoryDC.LineTo(rectScroll.left+1, rectScroll.top+1);
    m_MemoryDC.LineTo(rectScroll.right-2, rectScroll.top+1);
    m_MemoryDC.SelectObject( pp );
    m_MemoryDC.SelectObject( pOldBitmap );

		cbf.CreateSolidBrush( RGB(128, 128, 128) );
    if ( m_fVertical ) {
			if ( m_fShowRange ) {
				pcf = m_MemoryDC.SelectObject( &gl_fontSmall );
				for ( i = 0; i < 3; i++ ) {
					_gcvt_s(cTemp, m_eLoLimit + i * (m_eHiLimit - m_eLoLimit)/3, 2);
					m_MemoryDC.TextOut( rectScroll.left + 4, 
															rectScroll.top + 15 + i * rectScroll.Height()/3, cTemp, 2);
				}
			}
			else {
			}
		}
		else {	// horizonal
			if ( m_fShowRange ) {
			}
			else {
			}
		}
    m_fCreateMemoryDC = TRUE;
  }   

	pOldBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
	pdc->BitBlt(rectArea.left, rectArea.top, 
                rectArea.Width(), rectArea.Height(), 
                &m_MemoryDC, 0, 0, SRCCOPY);
	m_MemoryDC.SelectObject( pOldBitmap );
	if ( m_fVertical ) {
		rectScroll.left = rectArea.left + 2;
		rectScroll.right = rectArea.right - 2;
		rectScroll.top = rectArea.top + 15;
		rectScroll.bottom = rectArea.bottom - 15;
		if ( m_fShowRange ) {
			rectScroll.left = rectArea.left + rectArea.Width()/2;
			if ( m_fMoveMark ) {
				rectScroll.right = rectArea.right - 4;
				rectScroll.bottom = rectScroll.bottom - (long)(rectScroll.Height() * (m_eMarkPosition - m_eLoLimit) /(m_eHiLimit - m_eLoLimit));
				pdc->MoveTo( rectScroll.left, rectScroll.bottom );
				pcp = pdc->SelectObject( &cpMark );
				pdc->LineTo( rectScroll.right, rectScroll.bottom );
				pdc->SelectObject( pcp );
			}
			else {
				rectScroll.top = rectScroll.bottom - (long)(rectScroll.Height() * m_eMarkPosition /(m_eHiLimit - m_eLoLimit));
				pdc->FillRect( rectScroll, &cbMark );
			}
		}
		else {	// not show range
			if ( m_fMoveMark ) {
				rectScroll.right = rectArea.right - 4;
				rectScroll.bottom = rectScroll.bottom - (long)(rectScroll.Height() * (m_eMarkPosition - m_eLoLimit) /(m_eHiLimit - m_eLoLimit));
				pdc->MoveTo( rectScroll.left, rectScroll.bottom );
				pcp = pdc->SelectObject( &cpMark );
				pdc->LineTo( rectScroll.right, rectScroll.bottom );
				pdc->SelectObject( pcp );
			}
			else { // fill bar
				rectScroll.top = rectScroll.bottom - (long)(rectScroll.Height() * (m_eMarkPosition - m_eLoLimit) /(m_eHiLimit - m_eLoLimit));
				pdc->FillRect( rectScroll, &cbMark );
			}
		}
	}
	else {	// Horizonal
		rectScroll.left = rectArea.left + 15;
		rectScroll.right = rectArea.right - 15;
		rectScroll.top = rectArea.top + 2;
		rectScroll.bottom = rectArea.bottom - 2;
		if ( m_fShowRange ) {
			rectScroll.top = rectArea.top + rectArea.Height()/2;
			if ( m_fMoveMark ) {
				rectScroll.bottom = rectArea.bottom - 4;
				rectScroll.left = rectScroll.left + (long)(rectScroll.Width() * (m_eMarkPosition - m_eLoLimit) /(m_eHiLimit - m_eLoLimit));
				pdc->MoveTo( rectScroll.left, rectScroll.top );
				pcp = pdc->SelectObject( &cpMark );
				pdc->LineTo( rectScroll.left, rectScroll.bottom );
				pdc->SelectObject( pcp );
			}
			else {	// fill bar
				rectScroll.right = rectScroll.right + (long)(rectScroll.Width() * (m_eMarkPosition - m_eLoLimit) /(m_eHiLimit - m_eLoLimit));						
				pdc->FillRect( rectScroll, &cbMark );
			}
		}
		else { // not show range 
			if ( m_fMoveMark ) {
				rectScroll.bottom = rectArea.bottom - 4;
				rectScroll.left = rectScroll.left + (long)(rectScroll.Width() * (m_eMarkPosition - m_eLoLimit) /(m_eHiLimit - m_eLoLimit));
				pdc->MoveTo( rectScroll.left, rectScroll.top );
				pcp = pdc->SelectObject( &cpMark );
				pdc->LineTo( rectScroll.left, rectScroll.bottom );
				pdc->SelectObject( pcp );
			}
			else {	// fill bar
				rectScroll.right = rectScroll.right + (long)(rectScroll.Width() * (m_eMarkPosition - m_eLoLimit) /(m_eHiLimit - m_eLoLimit));
				pdc->FillRect( rectScroll, &cbMark );
			}
		}
	} // if Horizonal
	m_fDrawAll = FALSE;
  m_fNeedUpdate = FALSE;
}  

bool CObjectFillBar::SetDouble( ULONG ulIndex, double eValue ) {
	ASSERT( ulIndex == 0 );
	static double eOld;
	int a;
	if ( (eValue >= m_eLoLimit) && (eValue <= m_eHiLimit) ) m_eMarkPosition = eValue;
	else {
		ShowMessage(ID_WARN_INPUT_VALUE_OUT_OF_RANGE, (LPCTSTR)m_strName);
	}
	if ( m_fVertical ) {
		a = (int)(m_rectArea.Height() * (m_eMarkPosition - eOld));
	}
	else {
		a = (int)(m_rectArea.Width() * (m_eMarkPosition - eOld));
	}
	if ( a != 0 ) {
		eOld = m_eMarkPosition;
		m_fNeedUpdate = TRUE;
	}
	return(TRUE);
}

ParaName* CObjectFillBar::GetParaNameAddress( void ) {
  return( sm_ptrParaName );
}     

CString CObjectFillBar::GetParaName( ULONG index ) {
	ASSERT( index <= CObjectFillBar::sm_ulStringEnd );
	return( CObjectFillBar::sm_ptrParaName[index].Name );
}

ULONG CObjectFillBar::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

void CObjectFillBar::SelectParameter(ULONG ulType) {
  int i = 0;
  int j = 0;
  
  for (int k = 0; k <= sm_ulStringEnd; k++) {
    sm_aulSuitable[k] = -1;
  }
  while ( sm_ptrParaName[i].ulType != 0 ) {
    if ( (sm_ptrParaName[i].ulType | ulType) == sm_ptrParaName[i].ulType ) {
			if ( ulType & tINPUT ) {
				if ( m_vfSelected[i] == FALSE ) {
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

bool CObjectFillBar::SetProperty( void ) {
  CDlgSetFillBarProperty CDlg;
  CDlg.SetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_clrBar, m_lScanRate, m_fMoveMark, 
  							m_fShowRange, m_eHiLimit, m_eLoLimit, m_fVertical );
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_clrBar, m_lScanRate, m_fMoveMark,
  	  					  m_fShowRange, m_eHiLimit, m_eLoLimit, m_fVertical );
    m_fCreateMemoryDC = FALSE;
    return( TRUE );
  }
  return( FALSE );
}

INT32 CObjectFillBar::GetIndex( ULONG ulIndex ) {
  ASSERT((ulIndex >= 0) && (ulIndex <= 1));
  return( CObjectFillBar::sm_aulSuitable[ulIndex] );
}

bool CObjectFillBar::CheckSelf( void ) {
	CString str;

	for(const auto pc : m_listDynLink) {
		if ( pc->GetLinkMethod() != IDD_OUTPUT_FILLBAR ) {
			ShowMessage(ID_WARN_LINK_METHOD_MISMATCH, (LPCTSTR)m_strName);
			pc->SetLinkMethod( IDD_OUTPUT_FILLBAR );
		}
	}
	return( TRUE );
}
