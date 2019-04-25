#include "stdafx.h"
#include "math.h"

#include "globedef.h"

#include "cUnitBase.h"

#include "cObjGauge.h"
#include "DlgObjGauge.h"

IMPLEMENT_SERIAL(CObjectGauge, CObjRectBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

ParaName CObjectGauge::sm_ptrParaName[] = 
    { {"Value", tINPUT | tMODIFIABLE | tDOUBLE,  0}, 
      {"BackColor",tINPUT | tMODIFIABLE | tWORD,  1}, 
      {"ForeColor",tINPUT | tMODIFIABLE | tWORD,  2}, 
      {""     , 0, 3},
    }; 
    
INT32 CObjectGauge::sm_aulSuitable[] = { -1, -1, -1, -1 };

const ULONG CObjectGauge::sm_ulDoubleEnd = 0;
const ULONG CObjectGauge::sm_ulWordEnd   = 2;
const ULONG CObjectGauge::sm_ulStringEnd = 2; 

CObjectGauge::CObjectGauge(const CString& s, CRect r) : CObjRectBase(s, r) {
  ASSERT( r.Width() > 6 );
  
  m_eUpperLimit = 2;
  m_eLowLimit = m_eValue = 0;
  m_ulAngle = 180;
  m_fDrawAll = FALSE;
  m_fCreateMemoryDC = FALSE;
  m_ulGaugeType = TY_GAUGE;

  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd+1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectGauge::CObjectGauge( void ) : CObjRectBase( ) {

  m_eUpperLimit = 2;
  m_eLowLimit = m_eValue = 0;
  m_ulAngle = 180;
  m_fDrawAll = FALSE;
  m_fCreateMemoryDC = FALSE;
  m_ulGaugeType = TY_GAUGE;
  
  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectGauge::~CObjectGauge() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}

/////////////////////////////////////////////////////////////////////////////
// CObjectGauge diagnostics

#ifdef _DEBUG
void CObjectGauge::AssertValid() const
{
  CObjRectBase::AssertValid();
}

void CObjectGauge::Dump(CDumpContext& dc) const
{
  CObjRectBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectRect member function

void CObjectGauge::Serialize( CArchive& ar ) {
  CObjRectBase::Serialize( ar );

  ULONG a;

  if( ar.IsStoring() ) {
    ar << m_ulGaugeType << (ULONG)m_fSaveToFile << m_ulAngle;
  }
  else {
    ar >> m_ulGaugeType >> a >> m_ulAngle;
    m_fSaveToFile = (ULONG)a;
  }
} 

const CString& CObjectGauge::GetClassNameStr( void ) {
  static CString s_strClassName = "Gauge";

  return(s_strClassName);
}

bool CObjectGauge::ExectiveDynLink( ) {
  CUnitBase * pcunit;
  ULONG ulSourceIndex, ulDestIndex;
  
  for ( const auto pcobjDynLink : m_listDynLink ) {
    pcunit = pcobjDynLink->GetUnit();                    
    ulSourceIndex = pcobjDynLink->GetUnitIndex();
    ulDestIndex = pcobjDynLink->GetObjectIndex();   
    switch ( pcobjDynLink->GetLinkMethod() ) {
    case IDD_OUTPUT_GAUGE :
      ASSERT( pcobjDynLink->GetUnitDynLinkType() == tDOUBLE );
      SetDouble(ulDestIndex, pcunit->GetDouble(ulSourceIndex));
      SetUpdateFlag(TRUE); 
      break;
    default :		// don't permit other link method, report error
      ASSERT( 0 );
    }
  } // for 
  return( TRUE );
}

void CObjectGauge::ToShowStatic( CDC * const pdc, CPoint   ) {
  CPen *penOld, penBorder(PS_SOLID, 1, RGB(0, 0, 0)), p1(PS_SOLID, 1, RGB(255, 255, 255));
  CPen p2(PS_SOLID, 1, RGB(64, 64, 64)), p3(PS_SOLID, 1, RGB(192, 192, 192));
  CRect rectScroll, rectClip, rectFore, rectBk;
  CPoint ptStart, ptEnd;
  CBrush *pcb, cbb, cb1, cb2;
  double b;
  CRect rectArea = m_rectArea;
  CPoint pt = GetOffset();

  rectArea += pt;      
  cbb.CreateSolidBrush( RGB(192, 192, 192) );

  penOld = pdc->SelectObject(&penBorder);
  pcb = pdc->SelectObject( &cbb );
  pdc->MoveTo(rectArea.right-1, rectArea.top);
  pdc->LineTo(rectArea.right-1, rectArea.bottom-1);
  pdc->LineTo(rectArea.left, rectArea.bottom-1);
  pdc->SelectObject( &p3 );
  pdc->LineTo(rectArea.left, rectArea.top);
  pdc->LineTo(rectArea.right - 1, rectArea.top);
  pdc->SelectObject(&p2);
  pdc->MoveTo(rectArea.right-2, rectArea.top+1);
  pdc->LineTo(rectArea.right-2, rectArea.bottom-2);
  pdc->LineTo(rectArea.left+1, rectArea.bottom-2);
  pdc->SelectObject(&p1);
  pdc->LineTo(rectArea.left+1, rectArea.top+1);
  pdc->LineTo(rectArea.right-2, rectArea.top+1);

  switch ( m_ulGaugeType ) {
  case TY_GAUGE :
    if ( pdc->RectVisible(rectArea) ) {   // need to redraw ?
      rectFore.left = rectArea.left + 2;
      rectFore.right = rectArea.right - 2;
      rectFore.bottom = rectArea.bottom - 2;
      rectFore.top = rectArea.top + 2;
      pdc->FillRect( rectFore, &cbb );
      ptStart.x = rectFore.right;
      ptStart.y = (rectFore.bottom - rectFore.top) / 2 + rectFore.top;
      ptEnd.y = (long)((rectFore.Height() / 2 + rectFore.top) * (1 - sin(m_ulAngle * 3.14159265359 / 180)));
      ptEnd.x  = (long)(rectFore.left + rectFore.Width() * (1 + cos(m_ulAngle * 3.14159265359 / 180)) / 2);
      pdc->Arc( rectFore, ptStart, ptEnd );
      cb1.CreateSolidBrush( m_clrForeGrd );
      pdc->SelectObject( &cb1 );
      b = m_ulAngle * (m_eValue - m_eLowLimit) / (m_eUpperLimit - m_eLowLimit);
      ptEnd.y = (long)((rectFore.Height() / 2 + rectFore.top) * (1 - sin(b * 3.14159265359 / 180)));
      ptEnd.x = (long)(rectFore.left + rectFore.Width() * (1 + cos(b * 3.14159265359 / 180)) / 2);
      pdc->Pie(rectFore, ptStart, ptEnd );
    }
    break;
  case TY_FILLVERTICAL :
    rectFore.left = rectBk.left = rectArea.left + 2;
    rectFore.right = rectBk.right = rectArea.right - 2;
    rectFore.bottom = rectArea.bottom - 2;
    rectBk.top = rectArea.top + 2;
    rectFore.top = rectFore.bottom - 
                  (long)((rectFore.bottom - rectBk.top) * (m_eValue - m_eLowLimit) / ( m_eUpperLimit - m_eLowLimit));
    rectBk.bottom = rectFore.top;
    cb1.CreateSolidBrush( m_clrForeGrd );
    cb2.CreateSolidBrush( m_clrBkGrd );
    pdc->FillRect( rectFore, &cb1 );
    pdc->FillRect( rectBk, & cb2 );
    break;

  case TY_FILLHORIZON :
    rectFore.top = rectBk.top = rectArea.top + 2;
    rectFore.bottom = rectBk.bottom = rectArea.bottom - 2;
    rectFore.left = rectArea.left + 2;
    rectBk.right = rectArea.right - 2;
    rectFore.right = rectFore.left + 
                  (long)((rectBk.right - rectFore.left) * (m_eValue - m_eLowLimit) / ( m_eUpperLimit - m_eLowLimit));
    rectBk.left = rectFore.right;
    cb1.CreateSolidBrush( m_clrForeGrd );
    cb2.CreateSolidBrush( m_clrBkGrd );
    pdc->FillRect( rectFore, &cb1 );
    pdc->FillRect( rectBk, & cb2 );
    break;
  } // switch
  pdc->SelectObject( pcb );
  pdc->SelectObject( penOld );
	if ( m_fSelected ) SetFocus( pdc );
}

void CObjectGauge::ToShow( CDC * const pdc ) {
  CPen *penOld, penBorder(PS_SOLID, 1, RGB(0, 0, 0)), p1(PS_SOLID, 1, RGB(255, 255, 255));
  CPen p2(PS_SOLID, 1, RGB(64, 64, 64)), p3(PS_SOLID, 1, RGB(192, 192, 192));
  CRect rectScroll, rectClip, rectFore, rectBk;
  CPoint ptStart, ptEnd;
  CBrush *pcb, cbb, cb1, cb2;
  double b;
  CRect rectArea = m_rectArea, rectLastTime;

	rectArea.bottom -= rectArea.top;
	rectArea.right -= rectArea.left;
	rectArea.left = rectArea.top = 0;
	cbb.CreateSolidBrush( RGB(192, 192, 192) );
	penOld = pdc->SelectObject(&penBorder);
	pcb = pdc->SelectObject( &cbb );
	pdc->MoveTo(rectArea.right-1, rectArea.top);
	pdc->LineTo(rectArea.right-1, rectArea.bottom-1);
	pdc->LineTo(rectArea.left, rectArea.bottom-1);
	pdc->SelectObject( &p3 );
	pdc->LineTo(rectArea.left, rectArea.top);
	pdc->LineTo(rectArea.right - 1, rectArea.top);
	pdc->SelectObject(&p2);
	pdc->MoveTo(rectArea.right-2, rectArea.top+1);
	pdc->LineTo(rectArea.right-2, rectArea.bottom-2);
	pdc->LineTo(rectArea.left+1, rectArea.bottom-2);
	pdc->SelectObject(&p1);
	pdc->LineTo(rectArea.left+1, rectArea.top+1);
	pdc->LineTo(rectArea.right-2, rectArea.top+1);

  switch ( m_ulGaugeType ) {
  case TY_GAUGE :
    if ( pdc->RectVisible(rectArea) ) {   // need to redraw ?
      rectFore.left = rectArea.left + 2;
      rectFore.right = rectArea.right - 2;
      rectFore.bottom = rectArea.bottom - 2;
      rectFore.top = rectArea.top + 2;
      pdc->FillRect( rectFore, &cbb );
      ptStart.x = rectFore.right;
      ptStart.y = (rectFore.bottom - rectFore.top) / 2 + rectFore.top;
      ptEnd.y = (long)((rectFore.Height() / 2 + rectFore.top) * (1 - sin(m_ulAngle * 3.14159265359 / 180)));
      ptEnd.x  = (long)(rectFore.left + rectFore.Width() * (1 + cos(m_ulAngle * 3.14159265359 / 180)) / 2);
      pdc->Arc( rectFore, ptStart, ptEnd );
      cb1.CreateSolidBrush( m_clrForeGrd );
      pdc->SelectObject( &cb1 );
      b = m_ulAngle * (m_eValue - m_eLowLimit) / (m_eUpperLimit - m_eLowLimit);
      ptEnd.y = (long)((rectFore.Height() / 2 + rectFore.top) * (1 - sin(b * 3.14159265359 / 180)));
      ptEnd.x = (long)(rectFore.left + rectFore.Width() * (1 + cos(b * 3.14159265359 / 180)) / 2);
      pdc->Pie(rectFore, ptStart, ptEnd );
    }
    break;
  case TY_FILLVERTICAL :
    rectFore.left = rectBk.left = rectArea.left + 2;
    rectFore.right = rectBk.right = rectArea.right - 2;
    rectFore.bottom = rectArea.bottom - 2;
    rectBk.top = rectArea.top + 2;
    rectFore.top = rectFore.bottom - 
                  (long)((rectFore.bottom - rectBk.top) * (m_eValue - m_eLowLimit) / ( m_eUpperLimit - m_eLowLimit));
    rectBk.bottom = rectFore.top;
    cb1.CreateSolidBrush( m_clrForeGrd );
    cb2.CreateSolidBrush( m_clrBkGrd );
    pdc->FillRect( rectFore, &cb1 );
    pdc->FillRect( rectBk, & cb2 );
    break;

  case TY_FILLHORIZON :
    rectFore.top = rectBk.top = rectArea.top + 2;
    rectFore.bottom = rectBk.bottom = rectArea.bottom - 2;
    rectFore.left = rectArea.left + 2;
    rectBk.right = rectArea.right - 2;
    rectFore.right = rectFore.left + 
                  (long)((rectBk.right - rectFore.left) * (m_eValue - m_eLowLimit) / ( m_eUpperLimit - m_eLowLimit));
    rectBk.left = rectFore.right;
    cb1.CreateSolidBrush( m_clrForeGrd );
    cb2.CreateSolidBrush( m_clrBkGrd );
    pdc->FillRect( rectFore, &cb1 );
    pdc->FillRect( rectBk, & cb2 );
    break;
  } // switch
  pdc->SelectObject( pcb );
  pdc->SelectObject( penOld );
}

void CObjectGauge::ToShowDynamic( CDC * const pdc ) {
  CRect rectArea = m_rectArea, rectLastTime = m_rectLastTime;

  m_rectLastTime = m_rectArea;
	CBitmap * pOldBitmap;

	if ( !m_fCreateMemoryDC ) {
		m_MemoryDC.CreateCompatibleDC( pdc );
    m_Bitmap.CreateCompatibleBitmap( pdc, rectArea.Width(), rectArea.Height());
    pOldBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
		ToShow( &m_MemoryDC );
		m_MemoryDC.SelectObject( pOldBitmap );
		m_fCreateMemoryDC = TRUE;
	}
	// need to redraw graph
	if ( ( rectArea.Height() != rectLastTime.Height() )
		|| ( rectArea.Width() != rectLastTime.Width() ) ) {
		m_Bitmap.DeleteObject();
    m_Bitmap.CreateCompatibleBitmap( pdc, rectArea.Width(), rectArea.Height());
		pOldBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
		ToShow( &m_MemoryDC );
		m_MemoryDC.SelectObject( pOldBitmap );
	}
	// show it on upper layer's screen
	pOldBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
	pdc->BitBlt(rectArea.left, rectArea.top, 
			        rectArea.Width(), rectArea.Height(), 
				      &m_MemoryDC, 0, 0, SRCCOPY);
	m_MemoryDC.SelectObject( pOldBitmap );
	   
	m_fDrawAll = FALSE;
  m_fNeedUpdate = FALSE;
}  

ParaName* CObjectGauge::GetParaNameAddress( void ) {
  return( sm_ptrParaName );
}     

CString CObjectGauge::GetParaName( ULONG index ) {
  ASSERT( index <= CObjectGauge::sm_ulStringEnd );
  return( CObjectGauge::sm_ptrParaName[index].Name );
}

ULONG CObjectGauge::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

void CObjectGauge::SelectParameter(ULONG ulType) {
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

INT32 CObjectGauge::GetIndex( ULONG ulIndex ) {
  return( CObjectGauge::sm_aulSuitable[ulIndex] );
}

bool CObjectGauge::SetDouble(ULONG ulIndex, double eValue) {
  ASSERT( ulIndex <= 3 );

  ASSERT(m_vfSelected[ulIndex] == TRUE);
  if ( m_eUpperLimit < eValue ) { 
    m_eUpperLimit = eValue;
    m_fDrawAll = TRUE;
  }
  else if ( m_eLowLimit > eValue ) {
    m_eLowLimit = eValue; 
    m_fDrawAll = TRUE;
  }

  m_eValue = eValue;

  return(TRUE);
}

bool CObjectGauge::SetColor(ULONG index, LONG lValue) {
  ASSERT((index >= 1) && (index <= 2));

  switch ( index ) {
  case 2 :
    if ( (lValue < 16) && ( lValue >= 0) ) {
      m_clrForeGrd = rgbArray[lValue];
    }
    else m_clrForeGrd = lValue;
    m_fChangeSize = TRUE;
    break;
  case 1 :
    if ( (lValue < 16) && ( lValue >= 0) ) {
      m_clrBkGrd = rgbArray[lValue];
    }
    else m_clrBkGrd = lValue;
    break;
  default :
    return( FALSE );
  }

  return( TRUE );
} 

bool CObjectGauge::SetProperty( void ) {
  CDlgSetGaugeProperty CDlg;
  CDlg.SetData( m_fSaveToFile, m_ulAngle, m_ulGaugeType, m_clrForeGrd,
                m_clrBkGrd, m_strName, m_lScanRate );
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData( m_fSaveToFile, m_ulAngle, m_ulGaugeType, m_clrForeGrd,
                  m_clrBkGrd, m_strName, m_lScanRate );
    return( TRUE );
  }
  return( FALSE );
}

bool CObjectGauge::CheckSelf( void ) {
	CString str;

	for( const auto pc : m_listDynLink ) {
		if ( pc->GetLinkMethod() != IDD_OUTPUT_GAUGE ) {
			ShowMessage(ID_WARN_LINK_METHOD_MISMATCH, (LPCTSTR)m_strName);
			pc->SetLinkMethod( IDD_OUTPUT_GAUGE );
		}
	}
	return( TRUE );
}
