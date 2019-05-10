// cSlider.cpp implement CObjectSlider class

#include "stdafx.h"
                               
#include "CUnitBase.h"
#include "cObjSlider.h"
#include "DlgObjSlide.h"

const int c_iSliderWidth = 20;

IMPLEMENT_SERIAL(CObjectSlider, CObjRectBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

ParaName CObjectSlider::sm_ptrParaName[] = 
    { {"Slider", tOUTPUT | tMODIFIABLE | tDOUBLE,  0}, 
      {""     , 0, 1},
    }; 
    
INT32 CObjectSlider::sm_aulSuitable[] = { -1, -1 };

const ULONG CObjectSlider::sm_ulDoubleEnd = 1;
const ULONG CObjectSlider::sm_ulWordEnd   = 1;
const ULONG CObjectSlider::sm_ulStringEnd = 1;                

CObjectSlider::CObjectSlider(const CString& s, CRect r) : CObjRectBase(s, r) {
  m_fVertical = TRUE;
  m_fBeginTrack = FALSE;
  m_eLoLimit = 0.0;
  m_eSliderPosition = 0.0;
  m_eHiLimit = 1.0;
  m_fCreateMemoryDC = FALSE;

  if ( m_rectArea.Width() < c_iSliderWidth * 2 ) m_rectArea.right = m_rectArea.left + c_iSliderWidth*2;
  if ( m_rectArea.Height() < c_iSliderWidth * 2 ) m_rectArea.bottom = m_rectArea.top + c_iSliderWidth*2;

  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectSlider::CObjectSlider( void ) : CObjRectBase( ) {
  m_fBeginTrack = FALSE;
  m_eLoLimit = 0.0;
  m_eSliderPosition = 0.0;
  m_eHiLimit = 1.0;
  m_fCreateMemoryDC = FALSE;

  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectSlider::~CObjectSlider() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}

/////////////////////////////////////////////////////////////////////////////
// CObjectSlider diagnostics

#ifdef _DEBUG
void CObjectSlider::AssertValid() const
{
  CObjRectBase::AssertValid();
}

void CObjectSlider::Dump(CDumpContext& dc) const
{
  CObjRectBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectRect member function

void CObjectSlider::Serialize( CArchive& ar ) {
  CObjRectBase::Serialize( ar );
  LONG a;
  if( ar.IsStoring() ) {
    ar << m_eLoLimit << m_eHiLimit << (ULONG)m_fVertical;
  }
  else {
    ar >> m_eLoLimit >> m_eHiLimit >> a;
    m_fVertical = (bool)a;
    if ( m_rectArea.Width() < c_iSliderWidth*2 ) m_rectArea.right = m_rectArea.left + c_iSliderWidth*2;
    if ( m_rectArea.Height() < c_iSliderWidth*2 ) m_rectArea.bottom = m_rectArea.top + c_iSliderWidth*2;
    if ( m_fVertical ) {
      m_rectSlider.left = m_rectArea.left + 1;
      m_rectSlider.right = m_rectArea.right - 1;
      m_rectSlider.bottom = m_rectArea.bottom - c_iSliderWidth*3/2;
      m_rectSlider.top = m_rectSlider.bottom - c_iSliderWidth;
    }
    else {
      m_rectSlider.left = m_rectArea.left + c_iSliderWidth*3/2;
      m_rectSlider.right = m_rectSlider.left + c_iSliderWidth;
      m_rectSlider.bottom = m_rectArea.bottom - 1;
      m_rectSlider.top = m_rectArea.top + 1;
    }
  }
} 

const CString& CObjectSlider::GetClassNameStr( void ) {
  static CString s_strClassName = "Slider";

  return(s_strClassName);
}

bool CObjectSlider::ExectiveDynLink( ) {
  CUnitBasePtr pcunit;
  ULONG ulSourceIndex, ulDestIndex;
    
  for (const auto pcobjDynLink : m_listDynLink) {
    pcunit = pcobjDynLink->GetUnit();                    
    ulSourceIndex = pcobjDynLink->GetUnitIndex();
    ulDestIndex = pcobjDynLink->GetObjectIndex();   
    switch ( pcobjDynLink->GetLinkMethod() ) {
    case IDD_INPUT_DOUBLE :
      pcunit->SetDouble(ulSourceIndex, m_eSliderPosition);
      break;
    default :		// don't permit other link method, report error
      ASSERT( 0 );
    } // 
  } // for   
  return( TRUE );
}

bool CObjectSlider::InIt(POINT const pt, int iStatus) {
  CRect rectArea = m_rectArea, rectSlider = m_rectSlider;
  CPoint ptOffsetBase = GetOffset(), pt0(0,0);

	ASSERT( ptOffsetBase == pt0 );
  rectArea += ptOffsetBase;
  rectSlider += ptOffsetBase;
  bool fDo = FALSE;
  bool fInRect = rectArea.PtInRect(pt);
  bool fInSlider = rectSlider.PtInRect(pt);
  
  switch ( iStatus ) {
  case 1 :  // mouse down
    if ( fInSlider ) {
      m_fTrackMouseMove = TRUE;
      m_fBeginTrack = TRUE;
    }
    break;
  case 2 :  // mouse up
    if ( m_fBeginTrack ) {
      m_fTrackMouseMove = FALSE;
      m_fBeginTrack = FALSE;
    }
    else if ( fInRect ) {
      if ( m_fVertical ) {
        m_eSliderPosition = (m_eHiLimit-m_eLoLimit)*(rectArea.bottom - 2*c_iSliderWidth -pt.y)/(rectArea.Height()-c_iSliderWidth*4);
        if ( m_eSliderPosition > m_eHiLimit ) m_eSliderPosition = m_eHiLimit;
        if ( m_eSliderPosition < m_eLoLimit ) m_eSliderPosition = m_eLoLimit;
        m_rectSlider.bottom = rectArea.bottom - c_iSliderWidth*3/2 - (ULONG)((m_rectArea.Height() - c_iSliderWidth*4)
                * (m_eSliderPosition - m_eLoLimit)/(m_eHiLimit - m_eLoLimit));
        m_rectSlider.top = m_rectSlider.bottom - c_iSliderWidth;
        fDo = TRUE;
      }
      else {
        m_eSliderPosition = (m_eHiLimit-m_eLoLimit)*(pt.x - rectArea.left - 2*c_iSliderWidth)/(rectArea.Width()-c_iSliderWidth*4);
        if ( m_eSliderPosition > m_eHiLimit ) m_eSliderPosition = m_eHiLimit;
        if ( m_eSliderPosition < m_eLoLimit ) m_eSliderPosition = m_eLoLimit;
        m_rectSlider.left = m_rectArea.left + c_iSliderWidth*3/2 + (ULONG)((m_rectArea.Width() - c_iSliderWidth*4)
                * (m_eSliderPosition - m_eLoLimit)/(m_eHiLimit - m_eLoLimit));
        m_rectSlider.right = m_rectSlider.left + c_iSliderWidth;
        fDo = TRUE;
      }
    }
    else {
    }
    break;
  case 0 :  // mouse move
    if ( m_fBeginTrack ) {
      if ( fInRect ) {
        m_fTrackMouseMove  = TRUE;
        if ( m_fVertical ) {
          m_eSliderPosition = (m_eHiLimit-m_eLoLimit)*(rectArea.bottom - 2*c_iSliderWidth - pt.y)/(rectArea.Height()-c_iSliderWidth*4);
          if ( m_eSliderPosition > m_eHiLimit ) m_eSliderPosition = m_eHiLimit;
          if ( m_eSliderPosition < m_eLoLimit ) m_eSliderPosition = m_eLoLimit;
          m_rectSlider.bottom = m_rectArea.bottom - c_iSliderWidth*3/2 - (ULONG)((m_rectArea.Height() - c_iSliderWidth*4)
                  * (m_eSliderPosition - m_eLoLimit)/(m_eHiLimit - m_eLoLimit));
          m_rectSlider.top = m_rectSlider.bottom - c_iSliderWidth;fDo = TRUE;
        }
        else {
          m_eSliderPosition = (m_eHiLimit-m_eLoLimit)*(pt.x - m_rectArea.left - 2*c_iSliderWidth)/(m_rectArea.Width()-c_iSliderWidth*4);
          if ( m_eSliderPosition > m_eHiLimit ) m_eSliderPosition = m_eHiLimit;
          if ( m_eSliderPosition < m_eLoLimit ) m_eSliderPosition = m_eLoLimit;
          m_rectSlider.left = rectArea.left + c_iSliderWidth*3/2 + (ULONG)((rectArea.Width() - c_iSliderWidth*4)
                  * (m_eSliderPosition - m_eLoLimit)/(m_eHiLimit - m_eLoLimit));
          fDo = TRUE;
        }
      }
      else {
        m_fTrackMouseMove = FALSE;
      }
    }
    break;
  case -1 : // don't change
    ;
  default :
    ;
  } // switch 
  
	// set value to linked unit
  if ( fDo ) ExectiveDynLink();
        
  return( fInRect );
}


void CObjectSlider::ToShowStatic( CDC * const pdc, CPoint  ) {
  CPen p1(PS_SOLID, 1, RGB(255, 255, 255)), p2(PS_SOLID, 1, RGB(64, 64, 64));
  CPen p3(PS_SOLID, 1, RGB(0, 0, 0)), * pp;
  CBrush cbf, cbb;
  CRect rectScroll;
  CRect rectSlider;
  CRect rectArea = m_rectArea;
  CPoint pt = GetOffset();

  rectArea += pt;
  cbb.CreateSolidBrush( RGB(192, 192, 192) );

  if ( pdc->RectVisible(rectArea) ) {
    if ( m_fVertical ) {
      rectSlider.left = rectArea.left + 1;
      rectSlider.right = rectArea.right - 1;
      rectSlider.bottom = rectArea.bottom - c_iSliderWidth*3/2;
      rectSlider.top = rectSlider.bottom - c_iSliderWidth;
  
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
      pdc->SelectObject(&p3);
      pdc->MoveTo(rectSlider.right, rectSlider.top);
      pdc->LineTo(rectSlider.right, rectSlider.bottom);
      pdc->LineTo(rectSlider.left, rectSlider.bottom);
      pdc->SelectObject(&p2);
      pdc->MoveTo(rectSlider.right-1, rectSlider.top+1);
      pdc->LineTo(rectSlider.right-1, rectSlider.bottom-1);
      pdc->LineTo(rectSlider.left+1, rectSlider.bottom-1);
      pdc->SelectObject(&p1);
      pdc->LineTo(rectSlider.left+1, rectSlider.top+1);
      pdc->LineTo(rectSlider.right-1, rectSlider.top+1);
      pdc->SelectObject(pp);
    }
    else {
      rectSlider.top = rectArea.top + 1;
      rectSlider.bottom = rectArea.bottom - 1;
      rectSlider.left = rectArea.left + c_iSliderWidth*3/2;
      rectSlider.right = rectSlider.left + c_iSliderWidth;
  
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
      pdc->SelectObject(&p3);
      pdc->MoveTo(rectSlider.right-1, rectSlider.top);
      pdc->LineTo(rectSlider.right-1, rectSlider.bottom-1);
      pdc->LineTo(rectSlider.left, rectSlider.bottom-1);
      pdc->SelectObject(&p1);
      pdc->LineTo(rectSlider.left, rectSlider.top);
      pdc->LineTo(rectSlider.right-1, rectSlider.top);
      pdc->MoveTo( rectSlider.left + c_iSliderWidth/2, rectSlider.top );
      pdc->LineTo( rectSlider.left + c_iSliderWidth/2, rectSlider.bottom-1 );
      pdc->SelectObject(pp);
    }
  }
	if ( m_fSelected ) SetFocus( pdc );
}  

void CObjectSlider::ToShowDynamic( CDC * const pdc  ) {
  CBitmap * pOldBitmap;
  CPen p1(PS_SOLID, 1, RGB(255, 255, 255)), p2(PS_SOLID, 1, RGB(64, 64, 64));
  CPen p3(PS_SOLID, 1, RGB(0, 0, 0)), * pp;
  CBrush cbf, cbb;
  CRect rectScroll;
  CRect rectSlider;
  CPoint pt = GetOffset();
  CRect rectArea = m_rectArea;


  rectArea += pt;
  cbb.CreateSolidBrush( RGB(192, 192, 192) );

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

    m_SliderDC.CreateCompatibleDC( pdc );
    cbf.CreateSolidBrush( RGB(128, 128, 128) );
    if ( m_fVertical ) {
      m_SliderBitmap.CreateCompatibleBitmap( pdc, rectArea.Width()-2, c_iSliderWidth );
      pOldBitmap = m_SliderDC.SelectObject( &m_SliderBitmap );
      rectSlider.left = 0;
      rectSlider.right = rectArea.Width() - 2;
      rectSlider.top = 0;
      rectSlider.bottom = c_iSliderWidth;
      m_SliderDC.FillRect(rectSlider, &cbf);
      pp = m_SliderDC.SelectObject(&p3);
      m_SliderDC.MoveTo(rectSlider.right-1, rectSlider.top);
      m_SliderDC.LineTo(rectSlider.right-1, rectSlider.bottom-1);
      m_SliderDC.LineTo(rectSlider.left, rectSlider.bottom-1);
      m_SliderDC.SelectObject(&p1);
      m_SliderDC.LineTo(rectSlider.left, rectSlider.top);
      m_SliderDC.LineTo(rectSlider.right, rectSlider.top);
      m_SliderDC.MoveTo( rectSlider.left, c_iSliderWidth/2 );
      m_SliderDC.LineTo( rectSlider.right-1, c_iSliderWidth/2 );
      m_SliderDC.SelectObject(pp);
      m_SliderDC.SelectObject( pOldBitmap );
    }
    else {
      m_SliderBitmap.CreateCompatibleBitmap( pdc, c_iSliderWidth, rectArea.Height()-2 );
      pOldBitmap = m_SliderDC.SelectObject( &m_SliderBitmap );
      rectSlider.left = 0;
      rectSlider.right = c_iSliderWidth;
      rectSlider.top = 0;
      rectSlider.bottom = rectArea.Height()-2;
      m_SliderDC.FillRect(rectSlider, &cbf);
      pp = m_SliderDC.SelectObject(&p3);
      m_SliderDC.MoveTo(rectSlider.right-1, rectSlider.top);
      m_SliderDC.LineTo(rectSlider.right-1, rectSlider.bottom-1);
      m_SliderDC.LineTo(rectSlider.left, rectSlider.bottom-1);
      m_SliderDC.SelectObject(&p1);
      m_SliderDC.LineTo(rectSlider.left, rectSlider.top);
      m_SliderDC.LineTo(rectSlider.right-1, rectSlider.top);
      m_SliderDC.MoveTo( c_iSliderWidth/2, rectSlider.top );
      m_SliderDC.LineTo( c_iSliderWidth/2, rectSlider.bottom-1 );
      m_SliderDC.SelectObject(pp);
      m_SliderDC.SelectObject( pOldBitmap );
    }
    m_fCreateMemoryDC = TRUE;
  }   

  rectSlider = m_rectSlider;
  rectSlider += pt;
  if ( pdc->RectVisible(rectArea) ) {
    pOldBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
    pdc->BitBlt(rectArea.left, rectArea.top, 
                  rectArea.Width(), rectArea.Height(), 
                  &m_MemoryDC, 0, 0, SRCCOPY);
    m_MemoryDC.SelectObject( pOldBitmap );
  
    if ( m_fVertical ) {
      pOldBitmap = m_SliderDC.SelectObject( &m_SliderBitmap );
      pdc->BitBlt(rectArea.left+1, rectSlider.top, rectArea.Width() - 2, c_iSliderWidth,
                  &m_SliderDC, 0, 0, SRCCOPY);
      m_SliderDC.SelectObject( pOldBitmap );
    }
    else {  // Horizonal
      pOldBitmap = m_SliderDC.SelectObject( &m_SliderBitmap );
      pdc->BitBlt(rectSlider.left, rectArea.top+1, c_iSliderWidth, rectArea.Height()-2,
                  &m_SliderDC, 0, 0, SRCCOPY);
      m_SliderDC.SelectObject( pOldBitmap );
    }
  }
  m_fDrawAll = FALSE;
  m_fNeedUpdate = FALSE;
}  

double CObjectSlider::GetDouble( ULONG ulIndex ) {
  ASSERT( ulIndex == 0 );
  return( m_eSliderPosition );
}

ParaName* CObjectSlider::GetParaNameAddress( void ) {
  return( sm_ptrParaName );
}     

CString CObjectSlider::GetParaName( ULONG index ) {
  ASSERT( index <= CObjectSlider::sm_ulStringEnd );
  return( CObjectSlider::sm_ptrParaName[index].Name );
}

ULONG CObjectSlider::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

void CObjectSlider::SelectParameter(ULONG ulType) {
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

bool CObjectSlider::SetProperty( void ) {
  CDlgSetSliderProperty CDlg;
  CDlg.SetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_lScanRate, m_eHiLimit, m_eLoLimit, m_fVertical );
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_lScanRate, m_eHiLimit, m_eLoLimit, m_fVertical  );
    return( TRUE );
  }
  return( FALSE );
}

INT32 CObjectSlider::GetIndex( ULONG ulIndex ) {
  return( CObjectSlider::sm_aulSuitable[ulIndex] );
}

bool CObjectSlider::CanInSymbol( void ) {
  return( FALSE );
}
