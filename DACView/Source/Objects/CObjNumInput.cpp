#include "stdafx.h"

#include "CUnitBase.h"

#include "DlgObjNumInput.h"

#include "cObjNumInput.h"

IMPLEMENT_SERIAL(CObjectNumberInput, CObjRectBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

ParaName CObjectNumberInput::sm_ptrParaName[] = 
    { {"Value", tOUTPUT | tMODIFIABLE | tDOUBLE,  0}, 
      {""     , 0, 1},
    }; 
    
INT32 CObjectNumberInput::sm_aulSuitable[] = { -1, -1 };

const ULONG CObjectNumberInput::sm_ulDoubleEnd = 0;
const ULONG CObjectNumberInput::sm_ulWordEnd   = 1;
const ULONG CObjectNumberInput::sm_ulStringEnd = 1;                

CObjectNumberInput::CObjectNumberInput(const CString& s, CRect r) : CObjRectBase(s, r) {
  m_strFontName = """Times New Roman""";
  m_fBold = FALSE;
  m_fItalic = FALSE;
  m_fUnderline = FALSE;
  m_fStrikeOut = FALSE;
  m_lHeight = -20;
  m_eValue = 0.0;
	m_eStep = 1.0;
  m_clrForeGrd = RGB(0, 0, 0);
  m_clrBkGrd = RGB(255, 255, 255);

  m_ulCurrentInsertPosition = 0;
  m_ulTotalChar = 0;
  m_pchChar[0] = 0x000;
  m_fCreateMemoryDC = FALSE;
	
	int iWeight;
	if ( m_fBold ) iWeight = 700;
	else iWeight = 400;
	ASSERT( m_font.CreateFont(m_lHeight, 0, 0, 0, iWeight, 
    (BYTE)m_fItalic, (BYTE)m_fUnderline, (BYTE)m_fStrikeOut,
    OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY, (FIXED_PITCH | FF_MODERN),
    (LPCTSTR)m_strFontName) );

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectNumberInput::CObjectNumberInput( void ) : CObjRectBase( ) {
	m_strFontName = """Times New Roman""";
  m_fBold = FALSE;
  m_fItalic = FALSE;
  m_fUnderline = FALSE;
  m_fStrikeOut = FALSE;
  m_lHeight = -20;

	m_eValue = 0.0;
	m_eStep = 1.0;
  
  m_ulCurrentInsertPosition = 0;
  m_ulTotalChar = 0;
  m_pchChar[0] = 0x000;
  m_fCreateMemoryDC = FALSE;

	int iWeight;
	if ( m_fBold ) iWeight = 700;
	else iWeight = 400;
	ASSERT( m_font.CreateFont(m_lHeight, 0, 0, 0, iWeight, 
    (BYTE)m_fItalic, (BYTE)m_fUnderline, (BYTE)m_fStrikeOut,
    OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY, (FIXED_PITCH | FF_MODERN),
    (LPCTSTR)m_strFontName) );

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectNumberInput::~CObjectNumberInput() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}

/////////////////////////////////////////////////////////////////////////////
// CObjectNumberInput diagnostics

#ifdef _DEBUG
void CObjectNumberInput::AssertValid() const
{
  CObjRectBase::AssertValid();
}

void CObjectNumberInput::Dump(CDumpContext& dc) const
{
  CObjRectBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectRect member function

void CObjectNumberInput::Serialize( CArchive& ar ) {
  CObjRectBase::Serialize( ar );
  ULONG a, b, c, d;

  if( ar.IsStoring() ) {
    ar << m_strFontName << m_lHeight 
       << (ULONG)m_fBold << (ULONG)m_fItalic 
       << (ULONG)m_fUnderline << (ULONG)m_fStrikeOut
       << m_eLowLimit << m_eHighLimit << m_eStep ;
  }
  else {
    ar >> m_strFontName >> m_lHeight 
      >> a >> b >> c >> d 
      >> m_eLowLimit >> m_eHighLimit >> m_eStep;
    m_fBold = (bool)a;
    m_fItalic = (bool)b;
    m_fUnderline = (bool)c;
    m_fStrikeOut = (bool)d;
  }
} 

const CString& CObjectNumberInput::GetClassNameStr( void ) {
  static CString s_strClassName = "NumberInput";

  return(s_strClassName);
}

bool CObjectNumberInput::ExectiveDynLink( void ) {
  // Dynamic Link Button
  CUnitBase * pcunit;
  ULONG ulDestIndex;
    
  for (const auto pcobjDynLink : m_listDynLink) {
    pcunit = pcobjDynLink->GetUnit();                    
    ulDestIndex = pcobjDynLink->GetUnitIndex();   
    switch ( pcobjDynLink->GetLinkMethod() ) {
    case IDD_INPUT_DOUBLE :
      pcunit->SetDouble(ulDestIndex, m_eValue);
      break;
    default : // don't permit other dynamic link, report error
      ASSERT( 0 );
      break;
    } // switch Link Method
  } // for
  return( TRUE ); 
}
        
bool CObjectNumberInput::InIt(POINT const pt, int iStatus) {
  CRect rectArea = m_rectArea;
  CPoint ptOffsetBase = GetOffset();

  rectArea += ptOffsetBase;
  bool fInIt;
  fInIt = rectArea.PtInRect(pt);
  switch ( iStatus ) {
  case 1 :  // mouse down
  case 0 :  // mouse move  
  case -1 : // don't change
    break;
  case 2 :  // mouse up
    if ( fInIt ) m_fNeedShowCaret = TRUE;
    else m_fNeedShowCaret = FALSE;
    break;
  default :
    ;
  } // switch   
  return( fInIt );
}

//////////////////////////////////////////////////////////////////////////////////
//
// adjust coret position
//
// Return :	always TRUE
// 
// Parameter :
//			CDC * pdc		: pointer to DC
//			CPoint& pt	: the position of caret, modified in function
//
// Description :
//	 Calculate the right position of caret, and set the variable pt.
//
////////////////////////////////////////////////////////////////////////////////////// 
bool CObjectNumberInput::AdjustCaretPos( CDC * pdc, CPoint& pt ) {
  char pch[64];
  memset( pch, 0x000, 64);
  int i = 0, length = m_rectArea.left + 4;
  CSize size1, size2;
  CFont *pfontOld;

  pfontOld = pdc->SelectObject( &m_font );
  
  size2.cx = 0;
  while ( (length < pt.x) && (m_pchChar[i] != 0x000) ) {
    pch[i] = m_pchChar[i];
    size1 = pdc->GetTextExtent( pch, i+1 );
    length = m_rectArea.left + 4 + size1.cx;
    if ( pt.x < length ) {
      if ( (length - (size1.cx - size2.cx)/2) > pt.x ) {
        length = m_rectArea.left + 4 + size2.cx;
        break;
      }
      else {
        i++;
        break;
      }
    }
    size2.cx = size1.cx;
    i++;
  }
  pt.y = m_rectArea.top + 4 + m_rectArea.Height()/2 - abs(m_lHeight)/2;
  pt.x = length;
  m_ulCurrentInsertPosition = i;  // set current insert position
  pdc->SelectObject( pfontOld );

  return( TRUE );
}

bool CObjectNumberInput::ProcessChar( UINT nChar, CPoint& pt ) {
  ULONG i = m_ulTotalChar;
  bool fDo;
  switch ( nChar ) {
  case 0x00D :  // CR
    m_eValue = atof( m_pchChar );
    m_ulTotalChar = m_ulCurrentInsertPosition = 0;
    m_pchChar[0] = 0x000;
    fDo = TRUE;
    break;
  case 0x008 :  // Back space
    if ( m_ulTotalChar == 0 ) return( TRUE );
    if ( m_ulCurrentInsertPosition == 0 ) return( TRUE );
    i = --m_ulCurrentInsertPosition;
    do {
      m_pchChar[i] = m_pchChar[i+1];
    } while ( i++ < m_ulTotalChar );
    m_ulTotalChar--;
    fDo = TRUE;
    break;
  case 0x000 :
    return( TRUE );
  case '0' :
  case '1' :
  case '2' :
  case '3' :
  case '4' :
  case '5' :
  case '6' :
  case '7' :
  case '8' :
  case '9' :
  case '.' :
    do {
      m_pchChar[i+1] = m_pchChar[i];
    } while ( i-- > m_ulCurrentInsertPosition );
    m_pchChar[m_ulCurrentInsertPosition++] = (char)nChar;
    m_pchChar[++m_ulTotalChar] = 0x000;
    fDo = TRUE;
    break;
  default :
  return( FALSE );
  }
  if ( fDo ) {
    CSize size;
    CFont *pfontOld;
    CClientDC dc(nullptr);
    int iWeight;

    if ( m_fBold ) iWeight = 700;
    else iWeight = 400;
  
    // BLOCK: compute the width of the date string
    pfontOld = dc.SelectObject( &m_font );
  
    size = dc.GetTextExtent( m_pchChar, m_ulCurrentInsertPosition );
    dc.SelectObject( pfontOld );
    pt.x = m_rectArea.left + 4 + size.cx;
    pt.y = m_rectArea.top + m_rectArea.Height()/2;
    return( TRUE );
  }
  return( FALSE );
}

void CObjectNumberInput::ToShow( CDC * const pdc ) {
  CPen p1(PS_SOLID, 1, RGB(255, 255, 255));
  CPen p2(PS_SOLID, 1, RGB(64, 64, 64)), p3(PS_SOLID, 1, RGB(0, 0, 0)), * pp;
  CRect rectArea = m_rectArea;
  CBrush cbb;

  cbb.CreateSolidBrush( m_clrBkGrd );
  pdc->FillRect(rectArea, &cbb);
  int left = rectArea.left, right = rectArea.right - 1;
  int top = rectArea.top, bottom = rectArea.bottom - 1;
  pp = (CPen *)pdc->SelectObject(&p3);
  pdc->MoveTo(left, top);
  pdc->LineTo(right, top);
  pdc->LineTo(right, bottom);
  pdc->LineTo(left, bottom);
  pdc->LineTo(left, top);
	pdc->SelectObject(pp);
}

void CObjectNumberInput::ToShowStatic( CDC * const pdc, CPoint  ) {
  CPen p1(PS_SOLID, 1, RGB(255, 255, 255));
  CPen p2(PS_SOLID, 1, RGB(64, 64, 64)), p3(PS_SOLID, 1, RGB(0, 0, 0)), * pp;
  CRect rectArea = m_rectArea;
  CPoint pt = GetOffset();

  rectArea += pt;
  if ( pdc->RectVisible(rectArea) ) {   // if I need to redraw ?
    CBrush cbb;
    cbb.CreateSolidBrush( m_clrBkGrd );
    pdc->FillRect(rectArea, &cbb);
    int left = rectArea.left, right = rectArea.right - 1;
    int top = rectArea.top, bottom = rectArea.bottom - 1;
    pp = (CPen *)pdc->SelectObject(&p3);
    pdc->MoveTo(left, top);
    pdc->LineTo(right, top);
    pdc->LineTo(right, bottom);
    pdc->LineTo(left, bottom);
    pdc->LineTo(left, top);
    pdc->SelectObject(pp);
  }
	if ( m_fSelected ) SetFocus( pdc );
}  

void CObjectNumberInput::ToShowDynamic( CDC * const pdc ) {
  CRect rectArea = m_rectArea;

  CBrush cbb;
  CFont *pfontOld;
  UINT uTextAlign;
  COLORREF colorOld;
  CRect rectClip;

  m_rectLastTime = m_rectArea;
  
  rectClip.left = rectArea.left + 4;
  rectClip.right = rectArea.right - 4;
  rectClip.top = rectArea.top + rectArea.Height()/2 - abs(m_lHeight)/2 - 1;
  rectClip.bottom = rectArea.bottom - rectArea.Height()/2 + abs(m_lHeight)/2 + 1;

	ToShow( pdc );
  uTextAlign = pdc->SetTextAlign( TA_LEFT | TA_TOP );
  colorOld = pdc->SetTextColor( m_clrForeGrd );
  pfontOld = pdc->SelectObject(&m_font);
  pdc->ExtTextOut( rectClip.left, rectClip.top, ETO_CLIPPED, 
     							 rectClip,  m_pchChar, strlen(m_pchChar), nullptr);
  pdc->SelectObject(pfontOld);
  pdc->SetTextColor( colorOld );
	pdc->SetTextAlign( uTextAlign );

	m_fDrawAll = FALSE;
  m_fNeedUpdate = FALSE;
}

ParaName* CObjectNumberInput::GetParaNameAddress( void ) {
  return( sm_ptrParaName );
}  

CString CObjectNumberInput::GetParaName( ULONG index ) {
  ASSERT( index <= CObjectNumberInput::sm_ulStringEnd );
  return( CObjectNumberInput::sm_ptrParaName[index].Name );
}

ULONG CObjectNumberInput::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

void CObjectNumberInput::SelectParameter(ULONG ulType) {
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

INT32 CObjectNumberInput::GetIndex( ULONG ulIndex ) {
  return( CObjectNumberInput::sm_aulSuitable[ulIndex] );
}

bool CObjectNumberInput::SetProperty( void ) {
  CDlgSetNumberInputProperty CDlg;
  CDlg.SetData( m_strFontName, m_lHeight,
    m_fBold, m_fItalic, m_fUnderline, m_fStrikeOut,
    m_clrForeGrd, m_clrBkGrd, m_strName, m_lScanRate  );
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData( m_strFontName, m_lHeight, 
                  m_fBold, m_fItalic, m_fUnderline, m_fStrikeOut,
                  m_clrForeGrd, m_clrBkGrd, m_strName, m_lScanRate  );
		m_font.DeleteObject();
		int iWeight;
		if ( m_fBold ) iWeight = 700;
		else iWeight = 400;
    if ( abs(m_lHeight) > (m_rectArea.Height() - 8) ) {
      if ( m_lHeight > 0 ) m_lHeight = m_rectArea.Height() - 8;
      else m_lHeight = -(m_rectArea.Height() - 8);
    }
		m_font.CreateFont(m_lHeight, 0, 0, 0, iWeight, 
      (BYTE)m_fItalic, (BYTE)m_fUnderline, (BYTE)m_fStrikeOut,
      OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
      DEFAULT_QUALITY, (FIXED_PITCH | FF_MODERN),
      (LPCTSTR)m_strFontName);
    return( TRUE );
  }
  return( FALSE );
}

bool CObjectNumberInput::CheckSelf( void ) {
	CString str;

	for( const auto pc : m_listDynLink ) {
	}
	return( TRUE );
}

bool CObjectNumberInput::CanInSymbol( void ) {
  return( FALSE );
}
