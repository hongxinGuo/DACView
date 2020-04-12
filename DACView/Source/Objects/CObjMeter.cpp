#include "stdafx.h"

#include "math.h"

#include "CUnitBase.h"

#include "cObjMeter.h"
#include "DlgObjMeter.h"

#include "globedef.h"

IMPLEMENT_SERIAL(CObjectMeter, CObjRectBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE 
static char THIS_FILE[] = __FILE__;
#endif   

ParaName CObjectMeter::sm_ptrParaName[] = 
    { {"Value", tINPUT | tMODIFIABLE | tDOUBLE | tWORD,  0}, 
      {""     , 0, 1},
    }; 
    
INT32 CObjectMeter::sm_aulSuitable[] = { -1, -1 };

const ULONG CObjectMeter::sm_ulDoubleEnd = 0;
const ULONG CObjectMeter::sm_ulWordEnd   = 1;
const ULONG CObjectMeter::sm_ulStringEnd = 1;                

CObjectMeter::CObjectMeter(const CString& s, CRect r) : CObjRectBase(s, r) {
  m_fTrackMouseMove = FALSE;
  m_fBeginTrack = FALSE;
  m_eLowLimit = 0.0;
  m_eHighLimit = 1.0;
  m_eValue = 0.0;
	m_ulNumberStep = 10;
  m_fCreateMemoryDC = FALSE;
	m_clrNormal = RGB(0, 255, 0);
	m_clrWarn = RGB(255, 255, 0);
	m_clrError = RGB(255, 0, 0);
	m_clrNoddle = RGB(255, 255, 0);

  if ( m_rectArea.Width() < 60 ) m_rectArea.right = m_rectArea.left + 60;
  if ( m_rectArea.Height() < 60 )  m_rectArea.bottom = m_rectArea.top + 60;

  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectMeter::CObjectMeter( void ) : CObjRectBase( ) {
  m_fTrackMouseMove = FALSE;
  m_fBeginTrack = FALSE;
  m_eValue = 0.0;
	m_ulNumberStep = 10;
  m_fCreateMemoryDC = FALSE;
	m_clrNormal = RGB(0, 255, 0);
	m_clrWarn = RGB(255, 255, 0);
	m_clrError = RGB(255, 0, 0);
	m_clrNoddle = RGB(255, 255, 0);

  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectMeter::~CObjectMeter() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}

/////////////////////////////////////////////////////////////////////////////
// CObjectMeter diagnostics

#ifdef _DEBUG
void CObjectMeter::AssertValid() const
{
  CObjRectBase::AssertValid();
}

void CObjectMeter::Dump(CDumpContext& dc) const
{
  CObjRectBase::Dump(dc);
  dc << "\nm_eLoLimit = " << m_eLowLimit;
  dc << "\nm_eHiLimit = " << m_eHighLimit;
  dc << "\nm_eNormalLimit = " << m_eNormalLimit;
  dc << "\nm_eWarnLimit = " << m_eWarningLimit;
  dc << "\nm_eErrorLimit = " << m_eErrorLimit;
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectRect member function

void CObjectMeter::Serialize( CArchive& ar ) {
  CObjRectBase::Serialize( ar );
  if( ar.IsStoring() ) {
    ar << m_eLowLimit << m_eHighLimit << m_eNormalLimit << m_eWarningLimit << m_eErrorLimit;
		ar << m_clrNormal << m_clrWarn << m_clrError << m_clrNoddle;
  }
  else {
    ar >> m_eLowLimit >> m_eHighLimit >> m_eNormalLimit >> m_eWarningLimit >> m_eErrorLimit;
		ar >> m_clrNormal >> m_clrWarn >> m_clrError >> m_clrNoddle;
  }
}

const CString& CObjectMeter::GetClassNameStr( void ) {
  static CString s_strClassName = "Meter";

  return(s_strClassName);
}

bool CObjectMeter::ExectiveDynLink( void ) {
  // Dynamic Link Button
  CUnitBase * pcunit;
  ULONG ulDestIndex, ulSourceIndex;
    
  for (const auto pcobjDynLink : m_listDynLink ) {
    pcunit = pcobjDynLink->GetUnit();                    
    ulDestIndex = pcobjDynLink->GetObjectIndex();
    ulSourceIndex = pcobjDynLink->GetUnitIndex();   
    switch ( pcobjDynLink->GetLinkMethod() ) {
    case IDD_OUTPUT_METER :
      SetDouble(ulDestIndex, pcunit->GetDouble(ulSourceIndex));
      break;
    default :
			ASSERT( FALSE );		// error link method
      break;
    } // switch Link Method
  } // for 
  return( TRUE );
}

bool CObjectMeter::InIt(POINT const pt, int) {
  CRect rectArea = m_rectArea;
  CPoint ptOffsetBase = GetOffset();

  rectArea += ptOffsetBase;
  return(rectArea.PtInRect(pt));
}

void CObjectMeter::ToShowStatic( CDC * const pdc, CPoint  ) {
  CPen *penOld, penBorder(PS_SOLID, 1, RGB(0, 0, 0)), p1(PS_SOLID, 1, RGB(255, 255, 255));
  CPen cp1, p2(PS_SOLID, 1, RGB(64, 64, 64));
  CRect rectScroll, rectClip;
  CBrush * pcb, cbb, cbWhite, cbNormal, cbWarn, cbError;
	CPoint ptCenter, ptStart, ptEnd, pt1, pt2;
  int a, b, c, d, e, f;
	ULONG i;
	CFont * pcf;
	char cTemp[20];
	CSize sizeText;
	int iDataLength = abs((int)(log10(m_eHighLimit))) > abs((int)(log10(m_eLowLimit))) ? 
										abs((int)(log10(m_eHighLimit))) + 2 : abs((int)(log10(m_eLowLimit))) + 2;
  CRect rectArea = m_rectArea;
  CPoint pt = GetOffset();

  rectArea += pt;				                         
  cbb.CreateSolidBrush( RGB(192, 192, 192) );
  cbWhite.CreateSolidBrush( RGB(255, 255, 255) );
	cbNormal.CreateSolidBrush( m_clrNormal );
	cbWarn.CreateSolidBrush( m_clrWarn );
	cbError.CreateSolidBrush( m_clrError );
	cp1.CreatePen( PS_SOLID, 3, m_clrNoddle );
  
  if ( pdc->RectVisible(rectArea) ) {   // need to redraw ?
    cbb.CreateSolidBrush( RGB(192, 192, 192) );
    pdc->FillRect(rectArea, &cbb);
    int left = rectArea.left, right = rectArea.right - 1;
    int top = rectArea.top, bottom = rectArea.bottom - 1;
    penOld = (CPen *)pdc->SelectObject(&penBorder);
    pdc->MoveTo(left, top);
    pdc->LineTo(right, top);
    pdc->LineTo(right, bottom);
    pdc->LineTo(left, bottom);
    pdc->LineTo(left, top);
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
    
		m_ulCircleSize = min( rectArea.Width(), rectArea.Height() );
  	rectScroll.left = rectArea.left + rectArea.Width()/2 - m_ulCircleSize/2 + 20;
    rectScroll.right = rectArea.left + rectArea.Width()/2 + m_ulCircleSize/2 - 20;
    rectScroll.top = rectArea.top + rectArea.Height()/2 - m_ulCircleSize/2 + 20;
    rectScroll.bottom = rectArea.top + rectArea.Height()/2 + m_ulCircleSize/2 - 20;
    pcb = pdc->SelectObject( &cbError );
    ptEnd.x = rectScroll.right;
    ptEnd.y = rectScroll.top;
    ptStart.x = rectScroll.right;
    ptStart.y = rectScroll.bottom;
    pdc->Pie( rectScroll, ptStart, ptEnd );
    pcb = pdc->SelectObject( &cbWarn );
    ptStart = ptEnd;
    ptEnd.x = rectScroll.left;
    ptEnd.y = rectScroll.top;
    pdc->Pie( rectScroll, ptStart, ptEnd );
    pcb = pdc->SelectObject( &cbNormal );
    ptStart = ptEnd;
    ptEnd.x = rectScroll.left;
    ptEnd.y = rectScroll.bottom;
    pdc->Pie( rectScroll, ptStart, ptEnd );
    pdc->SelectObject( &cbb );
    rectScroll.left += 5;
    rectScroll.right -= 5;
    rectScroll.top += 5;
    rectScroll.bottom -= 5;
    ptEnd.x = rectScroll.left;
    ptEnd.y = rectScroll.bottom;
    ptStart.x = rectScroll.right;
    ptStart.y = rectScroll.bottom;
    pdc->Pie( rectScroll, ptStart, ptEnd );
		
		rectScroll.left += 5;
    rectScroll.right -= 5;
    rectScroll.top += 5;
    rectScroll.bottom -= 5;
		ptEnd.x = rectScroll.left;
    ptEnd.y = rectScroll.bottom;
    ptStart.x = rectScroll.right;
    ptStart.y = rectScroll.bottom;
    pdc->Arc(rectScroll, ptStart, ptEnd);
  	
  	// draw mark
		pcf = pdc->SelectObject( &gl_fontSmall );
		pdc->SetBkColor( RGB(192, 192, 192) );
		for ( i = 0; i <= m_ulNumberStep; i ++ ) {
			a = (rectScroll.left + rectScroll.Width()/2) +
				 	(int)(rectScroll.Width()/2 * sin(3.14*7/4 - i * 3.14*3/2/m_ulNumberStep));
			b = (rectScroll.top + rectScroll.Height()/2) +
					(int)(rectScroll.Height()/2	* cos(3.14*7/4 - i * 3.14*3/2/m_ulNumberStep));
			rectScroll.left += 10;
   		rectScroll.right -= 10;
    	rectScroll.top += 10;
    	rectScroll.bottom -= 10;
    	c = (rectScroll.left + rectScroll.Width()/2) +
    			(int)(rectScroll.Width()/2 * sin(3.14*7/4 - i * 3.14*3/2/m_ulNumberStep));
			d = (rectScroll.top + rectScroll.Height()/2) +
					(int)(rectScroll.Height()/2 * cos(3.14*7/4 - i * 3.14*3/2/m_ulNumberStep));
			rectScroll.left += 5;
   		rectScroll.right -= 5;
    	rectScroll.top += 5;
    	rectScroll.bottom -= 5;
    	e = (rectScroll.left + rectScroll.Width()/2) +
    			(int)(rectScroll.Width()/2 * sin(3.14*7/4 - i * 3.14*3/2/m_ulNumberStep));
			f = (rectScroll.top + rectScroll.Height()/2) +
					(int)(rectScroll.Height()/2 * cos(3.14*7/4 - i * 3.14*3/2/m_ulNumberStep));
			pdc->MoveTo(a, b);
			pdc->LineTo(c, d);
			_gcvt_s(cTemp, m_eLowLimit + i * (m_eHighLimit - m_eLowLimit)/m_ulNumberStep, iDataLength);
			sizeText = pdc->GetTextExtent( (LPCTSTR)cTemp, strlen(cTemp) );
			pdc->TextOut( e - sizeText.cx/2, f, (LPCTSTR)cTemp, iDataLength + 1 );
			rectScroll.left -= 15;
   		rectScroll.right += 15;
    	rectScroll.top -= 15;
    	rectScroll.bottom += 15;
		}
		pdc->SelectObject( pcf );
		pdc->SelectObject(penOld);
  }

	if ( m_fSelected ) SetFocus( pdc );
}  

void CObjectMeter::ToShowDynamic( CDC * const pdc ) {
  CPen *penOld, penBorder(PS_SOLID, 1, RGB(0, 0, 0)), p1(PS_SOLID, 1, RGB(255, 255, 255));
  CPen *pcp, cp1, p2(PS_SOLID, 1, RGB(64, 64, 64));
  CRect rectScroll, rectClip;
  CBrush * pcb, cbb, cbWhite, cbNormal, cbWarn, cbError;
	CPoint ptCenter, ptStart, ptEnd, pt1, pt2;
  int a, b, c, d, e, f;
	ULONG i;
	CFont * pcf;
  CBitmap * pOldBitmap;
	char cTemp[20];
	CSize sizeText;
	int iDataLength = abs((int)(log10(m_eHighLimit))) > abs((int)(log10(m_eLowLimit))) ? 
										abs((int)(log10(m_eHighLimit))) + 2 : abs((int)(log10(m_eLowLimit))) + 2;
  CRect rectArea = m_rectArea;

  m_rectLastTime = m_rectArea;

  cbb.CreateSolidBrush( RGB(192, 192, 192) );
  cbWhite.CreateSolidBrush( RGB(255, 255, 255) );
	cbNormal.CreateSolidBrush( m_clrNormal );
	cbWarn.CreateSolidBrush( m_clrWarn );
	cbError.CreateSolidBrush( m_clrError );
	cp1.CreatePen( PS_SOLID, 3, m_clrNoddle );
  
  if ( !m_fCreateMemoryDC ) {
    ptCenter.x = (rectArea.left + rectArea.right) / 2;
    ptCenter.y = (rectArea.top + rectArea.bottom) / 2;
    m_ulCircleSize = min( rectArea.Width(), rectArea.Height() );
    m_MemoryDC.CreateCompatibleDC( pdc );
    m_Bitmap.CreateCompatibleBitmap( pdc, rectArea.Width(), rectArea.Height());
    rectScroll.left = rectScroll.top = 0;
    rectScroll.bottom = rectArea.Height();
    rectScroll.right = rectArea.Width();
    pOldBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
    m_MemoryDC.FillRect(rectScroll, &cbb);
    // draw border
    penOld = m_MemoryDC.SelectObject(&penBorder);
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

		// draw color circle
    rectScroll.left = rectArea.Width()/2 - m_ulCircleSize/2 + 20;
    rectScroll.right = rectArea.Width()/2 + m_ulCircleSize/2 - 20;
    rectScroll.top = rectArea.Height()/2 - m_ulCircleSize/2 + 20;
    rectScroll.bottom = rectArea.Height()/2 + m_ulCircleSize/2 - 20;
    pcb = m_MemoryDC.SelectObject( &cbError );
    ptEnd.x = rectScroll.right;
    ptEnd.y = rectScroll.top;
    ptStart.x = rectScroll.right;
    ptStart.y = rectScroll.bottom;
    m_MemoryDC.Pie( rectScroll, ptStart, ptEnd );
    pcb = m_MemoryDC.SelectObject( &cbWarn );
    ptStart = ptEnd;
    ptEnd.x = rectScroll.left;
    ptEnd.y = rectScroll.top;
    m_MemoryDC.Pie( rectScroll, ptStart, ptEnd );
    pcb = m_MemoryDC.SelectObject( &cbNormal );
    ptStart = ptEnd;
    ptEnd.x = rectScroll.left;
    ptEnd.y = rectScroll.bottom;
    m_MemoryDC.Pie( rectScroll, ptStart, ptEnd );
    m_MemoryDC.SelectObject( &cbb );
    // clear inner circle 
    rectScroll.left += 5;
    rectScroll.right -= 5;
    rectScroll.top += 5;
    rectScroll.bottom -= 5;
    ptEnd.x = rectScroll.left;
    ptEnd.y = rectScroll.bottom;
    ptStart.x = rectScroll.right;
    ptStart.y = rectScroll.bottom;
    m_MemoryDC.Pie( rectScroll, ptStart, ptEnd );

    rectScroll.left += 5;
    rectScroll.right -= 5;
    rectScroll.top += 5;
    rectScroll.bottom -= 5;
    ptEnd.x = rectScroll.left;
    ptEnd.y = rectScroll.bottom;
    ptStart.x = rectScroll.right;
    ptStart.y = rectScroll.bottom;
    m_MemoryDC.Arc(rectScroll, ptStart, ptEnd);

    // draw mark
		pcf = m_MemoryDC.SelectObject( &gl_fontSmall );
		m_MemoryDC.SetBkColor( RGB(192, 192, 192) );
		for ( i = 0; i <= m_ulNumberStep; i ++ ) {
			a = (rectScroll.left + rectScroll.Width()/2) +
				 	(int)(rectScroll.Width()/2 * sin(3.14*7/4 - i * 3.14*3/2/m_ulNumberStep));
			b = (rectScroll.top + rectScroll.Height()/2) +
					(int)(rectScroll.Height()/2	* cos(3.14*7/4 - i * 3.14*3/2/m_ulNumberStep));
			rectScroll.left += 10;
   		rectScroll.right -= 10;
    	rectScroll.top += 10;
    	rectScroll.bottom -= 10;
    	c = (rectScroll.left + rectScroll.Width()/2) +
    			(int)(rectScroll.Width()/2 * sin(3.14*7/4 - i * 3.14*3/2/m_ulNumberStep));
			d = (rectScroll.top + rectScroll.Height()/2) +
					(int)(rectScroll.Height()/2 * cos(3.14*7/4 - i * 3.14*3/2/m_ulNumberStep));
			rectScroll.left += 5;
   		rectScroll.right -= 5;
    	rectScroll.top += 5;
    	rectScroll.bottom -= 5;
    	e = (rectScroll.left + rectScroll.Width()/2) +
    			(int)(rectScroll.Width()/2 * sin(3.14*7/4 - i * 3.14*3/2/m_ulNumberStep));
			f = (rectScroll.top + rectScroll.Height()/2) +
					(int)(rectScroll.Height()/2 * cos(3.14*7/4 - i * 3.14*3/2/m_ulNumberStep));
			m_MemoryDC.MoveTo(a, b);
			m_MemoryDC.LineTo(c, d);
			_gcvt_s(cTemp, m_eLowLimit + i * (m_eHighLimit - m_eLowLimit)/m_ulNumberStep, iDataLength);
			sizeText = m_MemoryDC.GetTextExtent( cTemp, strlen(cTemp) );
			m_MemoryDC.TextOut( e-sizeText.cx/2, f, cTemp, iDataLength + 1 );
			rectScroll.left -= 15;
   		rectScroll.right += 15;
    	rectScroll.top -= 15;
    	rectScroll.bottom += 15;
		}
		m_MemoryDC.SelectObject( pcf );
    m_MemoryDC.SelectObject( pOldBitmap );
    m_fCreateMemoryDC = TRUE;
  }   
  
  pOldBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
  pdc->BitBlt(rectArea.left, rectArea.top, 
              rectArea.Width(), rectArea.Height(), 
              &m_MemoryDC, 0, 0, SRCCOPY);
  m_MemoryDC.SelectObject( pOldBitmap );

  double a1;
  a1 = 3.1415926 *5/4 - 3.1415926 * 3 / 2 * (m_eValue - m_eLowLimit)/(m_eHighLimit - m_eLowLimit);
  d = (int)((m_ulCircleSize/2 - 30) * sin(a1));
  e = (int)((m_ulCircleSize/2 - 30) * cos(a1));
  ptStart.x = rectArea.left + rectArea.Width()/2;
  ptStart.y = rectArea.top + rectArea.Height()/2;
  ptEnd.x = ptStart.x + e;
  ptEnd.y = ptStart.y - d;
  pdc->MoveTo( ptStart.x, ptStart.y );
	pcp = pdc->SelectObject( &cp1 );
  pdc->LineTo( ptEnd.x, ptEnd.y );
	pdc->SelectObject( pcp );
  m_fDrawAll = FALSE;
  m_fNeedUpdate = FALSE;
}

ParaName* CObjectMeter::GetParaNameAddress( void ) {
  return( sm_ptrParaName );
}  

CString CObjectMeter::GetParaName( ULONG index ) {
  ASSERT( index <= CObjectMeter::sm_ulStringEnd );
  return( CObjectMeter::sm_ptrParaName[index].Name );
}

ULONG CObjectMeter::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

void CObjectMeter::SelectParameter(ULONG ulType) {
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

INT32 CObjectMeter::GetIndex( ULONG ulIndex ) {
  return( CObjectMeter::sm_aulSuitable[ulIndex] );
}

bool CObjectMeter::SetDouble( ULONG ulIndex, double eValue ) {
	ASSERT( ulIndex == 0 );
	static double eOld, e;
	e = (eOld - eValue)/(m_eHighLimit - m_eLowLimit);
	if ( (eValue <= m_eHighLimit) && (eValue >= m_eLowLimit) ) {
		if ( (e > 0.001) || (e < -0.001) ) {
			m_fNeedUpdate = TRUE;
			eOld = eValue;
		}
		m_eValue = eValue;
	}
	else {
		ShowMessage(ID_WARN_INPUT_VALUE_OUT_OF_RANGE, (LPCTSTR)m_strName);
	}
	return(TRUE);
}

bool CObjectMeter::SetProperty( void ) {
	CDlgSetMeterProperty CDlg;
  CDlg.SetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_lScanRate, 
  							m_clrNormal, m_clrWarn, m_clrError, m_clrNoddle, m_eHighLimit, m_eLowLimit );
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_lScanRate, 
  	  					  m_clrNormal, m_clrWarn, m_clrError, m_clrNoddle, m_eHighLimit, m_eLowLimit );
    m_fCreateMemoryDC = FALSE;
    return( TRUE );
  }
  return( FALSE );
}													 
  
bool CObjectMeter::CheckSelf( void ) {
	CString str;

	for(const auto pc : m_listDynLink) {
		if ( pc->GetLinkMethod() != IDD_OUTPUT_METER ) {
			ShowMessage(ID_WARN_LINK_METHOD_MISMATCH, (LPCTSTR)m_strName);
			pc->SetLinkMethod( IDD_OUTPUT_METER );
		}
	}
	return( TRUE );
}
