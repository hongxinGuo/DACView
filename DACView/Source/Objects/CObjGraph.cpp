#include "stdafx.h"

#include "globedef.h"

#include "CUnitBase.h"

#include "cObjGraph.h"
#include "DlgObjGraph.h"

IMPLEMENT_SERIAL(CObjectGraph, CObjRectBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// graph have two pens for volatile use, and two pens for double variable
ParaName CObjectGraph::sm_ptrParaName[] = 
    { {"Pen0", tINPUT | tMODIFIABLE | tDOUBLE | tWORD | tBOOL,  0}, 
      {"Pen1", tINPUT | tMODIFIABLE | tDOUBLE | tWORD | tBOOL,  1},
      {"Pen2", tINPUT | tMODIFIABLE | tDOUBLE,  2}, 
      {"Pen3", tINPUT | tMODIFIABLE | tDOUBLE,  3}, 
      {""     , 0, 4},
    }; 
    
INT32 CObjectGraph::sm_aulSuitable[] = { -1, -1, -1, -1, -1};

const ULONG CObjectGraph::sm_ulDoubleEnd = 3;
const ULONG CObjectGraph::sm_ulWordEnd   = 3;
const ULONG CObjectGraph::sm_ulStringEnd = 3; 

CObjectGraph::CObjectGraph(const CString& s, CRect r) : CObjRectBase(s, r) {
  if (  r.Width() < 60 ) {
    m_rectArea.right = m_rectArea.left + 60;
  }
  

  m_ulDataLength = r.Width() - 4;
  m_penColor[0] = RGB(255, 0, 0);
  m_penColor[1] = RGB(0, 255, 0);
  m_penColor[2] = RGB(0, 0, 255);
  m_penColor[3] = RGB(128, 128, 128);
  m_ulInputDataType = tDOUBLE;
          
  for ( int i = 0; i < 4; i++ ) {
    m_ppenGraph[i] = make_unique<CPen>(PS_SOLID, 1, m_penColor[i]);
    m_eptrData[i].resize(m_ulDataLength+1);
    for ( ULONG j = 0; j < m_ulDataLength; j ++ ) {
      m_eptrData[i][j] = 0.0;   // reset buffer
    }
    m_ptrBegin[i] = m_eptrData[i].begin();
  }
  m_eUpperLimit = 2;
  m_eLowLimit = 0;
  m_ulScrollLength = 0;
  m_fDrawAll = FALSE;
  m_fCreateMemoryDC = FALSE;

  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectGraph::CObjectGraph( void ) : CObjRectBase( ) {                             

  m_ulInputDataType = tDOUBLE;
  m_ulDataLength = 0;
  for ( int i = 0; i < 4; i++ ) {
		m_ppenGraph[i] = nullptr;
    m_ptrBegin[i] = m_eptrData[i].begin();
    m_pScrollLength[i] = 0;
  }
  m_eUpperLimit = 2;
  m_eLowLimit = 0;
  m_ulScrollLength = 0;
  m_fDrawAll = FALSE;
  m_fCreateMemoryDC = FALSE;

  m_fCreateMemoryDC = FALSE;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CObjectGraph::~CObjectGraph() {
  m_MemoryDC.DeleteDC();
	
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}

/////////////////////////////////////////////////////////////////////////////
// CObjectGraph diagnostics

#ifdef _DEBUG
void CObjectGraph::AssertValid() const
{
  CObjRectBase::AssertValid();
}

void CObjectGraph::Dump(CDumpContext& dc) const
{
  CObjRectBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectRect member function

/*
  ar << m_ulInputDateType << m_ulDataLength << m_penColor(0->3).
*/
void CObjectGraph::Serialize( CArchive& ar ) {
  CObjRectBase::Serialize( ar );

  int i;

  if( ar.IsStoring() ) {
    ar << m_ulInputDataType << m_ulDataLength;
    for ( i = 0; i < 4; i++ ) {
      ar << m_penColor[i];
    }  
  }
  else {
    ar >> m_ulInputDataType >> m_ulDataLength;
    ASSERT( m_rectArea.Width() >= 60 );
    for ( i = 0; i < 4; i++ ) {
      ar >> m_penColor[i];
    }
     
    for ( i = 0; i < 4; i++ ) {
      if ( m_ulDataLength < (m_rectArea.Width() - 4) ) m_ulDataLength = m_rectArea.Width() - 4;
      m_eptrData[i].resize(m_ulDataLength+1);
      m_ptrBegin[i] = m_eptrData[i].begin();
      for ( ULONG j = 0; j <= m_ulDataLength; j ++ ) {
        m_eptrData[i][j] = 0.0;   // reset buffer
      }
      m_ppenGraph[i] = make_shared<CPen>(PS_SOLID, 1, m_penColor[i]);
    }
  }
} 

const CString& CObjectGraph::GetClassNameStr( void ) {
  static CString s_strClassName = "Graph";

  return(s_strClassName);
}

bool CObjectGraph::ExectiveDynLink( ) {
  CUnitBasePtr pcunit;
  ULONG ulSourceIndex, ulDestIndex;
  double eTemp;
  LONG lTemp;
  bool fTemp;

  for ( const auto pcobjDynLink : m_listDynLink ) {
    pcunit = pcobjDynLink->GetUnit();                    
    ulSourceIndex = pcobjDynLink->GetUnitIndex();
    ulDestIndex = pcobjDynLink->GetObjectIndex();   
    if ( pcobjDynLink->GetLinkMethod() == IDD_OUTPUT_GRAPH ) {
      switch ( pcobjDynLink->GetUnitDynLinkType() ) {
      case tBOOL :
        fTemp = pcunit->GetBool(ulSourceIndex);
				if ( fTemp ) {
					SetDouble(ulDestIndex, (m_eUpperLimit - m_eLowLimit)*3/4);
				}
				else {
					SetDouble(ulDestIndex, (m_eUpperLimit - m_eLowLimit)/4);
				}
        break;
      case tWORD :
        lTemp = pcunit->GetInteger(ulSourceIndex);
        SetDouble(ulDestIndex, (double)lTemp);
        break;
      case tDOUBLE :
        eTemp = pcunit->GetDouble(ulSourceIndex);
        SetDouble(ulDestIndex, eTemp);
        break;
      default : // something was error
        ASSERT( 0 );
      } //  switch
    } // if 
		else ASSERT( FALSE );	// error link mehod
  } // for 
  return( TRUE );
}

void CObjectGraph::ToShowStatic( CDC * const pdc, CPoint  ) {
  double eRangeRate;
  double eRange;
  CPen *penOld, penBorder(PS_SOLID, 1, RGB(0, 0, 0)), p1(PS_SOLID, 1, RGB(255, 255, 255));
  CPen p2(PS_SOLID, 1, RGB(64, 64, 64));
  CRect rectGraph, rectScroll;
  CBrush cbb, cbb1;
  CPoint pt = GetOffset();
  CRect rectArea = m_rectArea;

  rectArea += pt;

  cbb.CreateSolidBrush( RGB(192, 192, 192) );
	cbb1.CreateSolidBrush( RGB(255, 255, 255) );

  if ( pdc->RectVisible(rectArea) ) {   // need to redraw ?
		rectScroll.left = rectArea.left + 4;
		rectScroll.right = rectArea.right - 4;
		rectScroll.top = rectArea.top + 4;
		rectScroll.bottom = rectArea.bottom - 4;
    pdc->FillRect(rectArea, &cbb);
		pdc->FillRect(rectScroll, &cbb1);
  
    penOld = pdc->SelectObject(&penBorder);
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

		pdc->SelectObject(&penBorder);
		pdc->MoveTo(rectScroll.left-1, rectScroll.bottom);
  	pdc->LineTo(rectScroll.left-1, rectScroll.top-1);
  	pdc->LineTo(rectScroll.right, rectScroll.top-1);
	
    eRange = m_eUpperLimit - m_eLowLimit;
    eRangeRate = eRange/(rectScroll.Height()+1);
    pdc->SelectObject(penOld);
  }
	if ( m_fSelected ) SetFocus( pdc );
}  

void CObjectGraph::ToShowDynamic( CDC * const pdc ) {
  double eRangeRate;
  double eRange;
  vector<double>::iterator pData;
  CPen *penOld, penBorder(PS_SOLID, 1, RGB(0, 0, 0)), p1(PS_SOLID, 1, RGB(255, 255, 255));
  CPen p2(PS_SOLID, 1, RGB(64, 64, 64));
  CRect rectGraph, rectScroll;
  CBrush cbb, cbb1;
  vector<double>::iterator pe[4];
  int iCount1, iCount2, lScrollLength;
	CBitmap * pBitmap;
  CRect rectArea = m_rectArea, rectLastTime = m_rectLastTime;

  m_rectLastTime = m_rectArea;

  cbb.CreateSolidBrush( RGB(192, 192, 192) );
	cbb1.CreateSolidBrush( RGB(255, 255, 255) );

  if ( !m_fCreateMemoryDC ) {   
    m_MemoryDC.CreateCompatibleDC( pdc );
    m_Bitmap.CreateCompatibleBitmap( pdc, rectArea.Width(), rectArea.Height());
    rectGraph.left = rectGraph.top = 0;
    rectGraph.bottom = rectArea.Height();
    rectGraph.right = rectArea.Width();
		rectScroll.top = rectScroll.left = 4;
		rectScroll.bottom = rectArea.Height() - 4;
		rectScroll.right = rectArea.Width() - 4;
    pBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
    m_MemoryDC.FillRect(rectGraph, &cbb);
		m_MemoryDC.FillRect(rectScroll, &cbb1);
    penOld = m_MemoryDC.SelectObject(&penBorder);
    m_MemoryDC.MoveTo(rectGraph.right-1, rectGraph.top);
    m_MemoryDC.LineTo(rectGraph.right-1, rectGraph.bottom-1);
    m_MemoryDC.LineTo(rectGraph.left, rectGraph.bottom-1);
    m_MemoryDC.SelectObject(&p2);
    m_MemoryDC.MoveTo(rectGraph.right-2, rectGraph.top+1);
    m_MemoryDC.LineTo(rectGraph.right-2, rectGraph.bottom-2);
    m_MemoryDC.LineTo(rectGraph.left+1, rectGraph.bottom-2);
    m_MemoryDC.SelectObject(&p1);
    m_MemoryDC.LineTo(rectGraph.left+1, rectGraph.top+1);
    m_MemoryDC.LineTo(rectGraph.right-2, rectGraph.top+1);
		
		m_MemoryDC.SelectObject(&penBorder);
		m_MemoryDC.MoveTo(rectScroll.left-1, rectScroll.bottom);
    m_MemoryDC.LineTo(rectScroll.left-1, rectScroll.top-1);
    m_MemoryDC.LineTo(rectScroll.right, rectScroll.top-1);
		
		m_MemoryDC.SelectObject( penOld );
		m_MemoryDC.SelectObject( pBitmap );
    m_fCreateMemoryDC = true;
  }   
  
  pBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
  rectScroll.top = rectScroll.left = 4;
	rectScroll.bottom = rectArea.Height() - 4;
	rectScroll.right = rectArea.Width() - 4;
	
  /*  for another thread (ExectiveDynLink) will update m_ptrBegin and m_pScrollLength,
      so I must save those variables into other variables.
  */
  CSingleLock slLock( &m_csLock );
  slLock.Lock();
  for ( iCount1 = 0; iCount1 < 4; iCount1++ ) {
    pe[iCount1] = m_ptrBegin[iCount1];
    if ( m_pScrollLength[iCount1] > m_ulScrollLength ) {
      m_ulScrollLength = m_pScrollLength[iCount1];
    }
    m_pScrollLength[iCount1] = 0;
  }
  slLock.Unlock();
  
  if ( m_ulScrollLength < rectScroll.Width() ) {
    lScrollLength = m_ulScrollLength;
  }
  else {
    lScrollLength = rectScroll.Width();
  }
  m_ulScrollLength = 0;
  
	if ( m_fDrawAll == false ) {
    rectGraph = rectScroll;
    rectGraph.left = 4 + lScrollLength;
    m_MemoryDC.ScrollDC(-lScrollLength, 0, rectGraph, rectScroll, nullptr, nullptr);
    rectGraph.left = rectGraph.right - lScrollLength;
    m_MemoryDC.FillRect(rectGraph, &cbb1);
    eRange = m_eUpperLimit - m_eLowLimit;
    eRangeRate = eRange/rectScroll.Height();
		penOld = m_MemoryDC.SelectObject(m_ppenGraph[0].get());
    for ( iCount1 = 0; iCount1 < 4; iCount1 ++ ) {
      if ( m_vfSelected[iCount1] == false ) continue; // not selected, ignore this pen
      m_MemoryDC.SelectObject(m_ppenGraph[iCount1].get());
      pData = pe[iCount1];
      if ( pData == m_eptrData[iCount1].begin() ) {
        pData = m_eptrData[iCount1].end();
      }                                
      else pData--;
      m_MemoryDC.MoveTo(rectScroll.right-1, rectScroll.bottom - 1 - int((*pData - m_eLowLimit)/eRangeRate));
      for ( iCount2 = 1; iCount2 <= lScrollLength; iCount2 ++ ) {
        if ( pData == m_eptrData[iCount1].begin() ) {
          pData = m_eptrData[iCount1].end();
        }                                
        else pData--;
        m_MemoryDC.LineTo(rectScroll.right - iCount2 - 1, rectScroll.bottom - 1 - int((*pData - m_eLowLimit)/eRangeRate));
      }
    }
    pdc->BitBlt(rectArea.left, rectArea.top, 
                rectArea.Width(), rectArea.Height(), 
                &m_MemoryDC, 0, 0, SRCCOPY);
    m_MemoryDC.SelectObject(penOld);
  } 
  else {
    rectGraph.left = rectGraph.top = 0;
    rectGraph.bottom = rectArea.Height();
    rectGraph.right = rectArea.Width();
    m_MemoryDC.FillRect(rectGraph, &cbb);
		m_MemoryDC.FillRect(rectScroll, &cbb1);
    penOld = m_MemoryDC.SelectObject(&penBorder);
    m_MemoryDC.MoveTo(rectGraph.right-1, rectGraph.top);
    m_MemoryDC.LineTo(rectGraph.right-1, rectGraph.bottom-1);
    m_MemoryDC.LineTo(rectGraph.left, rectGraph.bottom-1);
    m_MemoryDC.SelectObject(&p2);
    m_MemoryDC.MoveTo(rectGraph.right-2, rectGraph.top+1);
    m_MemoryDC.LineTo(rectGraph.right-2, rectGraph.bottom-2);
    m_MemoryDC.LineTo(rectGraph.left+1, rectGraph.bottom-2);
    m_MemoryDC.SelectObject(&p1);
    m_MemoryDC.LineTo(rectGraph.left+1, rectGraph.top+1);
    m_MemoryDC.LineTo(rectGraph.right-2, rectGraph.top+1);

    m_MemoryDC.SelectObject(&penBorder);
		m_MemoryDC.MoveTo(rectScroll.left-1, rectScroll.bottom);
	  m_MemoryDC.LineTo(rectScroll.left-1, rectScroll.top-1);
	  m_MemoryDC.LineTo(rectScroll.right, rectScroll.top-1);

		eRange = m_eUpperLimit - m_eLowLimit;
    eRangeRate = eRange/rectScroll.Height();
    for ( iCount1 = 0; iCount1 < 4; iCount1 ++ ) {
      if ( m_vfSelected[iCount1] == false ) continue; // not selected, ignore this pen
      m_MemoryDC.SelectObject(m_ppenGraph[iCount1].get());
      pData = pe[iCount1];
      if ( pData == m_eptrData[iCount1].begin() ) {
        pData = m_eptrData[iCount1].end();
      }                                
      else pData--;
      m_MemoryDC.MoveTo(rectScroll.right-1, rectScroll.bottom - 1 - int((*pData - m_eLowLimit)/eRangeRate));
      for ( iCount2 = 1; iCount2 < rectScroll.Width(); iCount2 ++ ) {
        if ( pData == m_eptrData[iCount1].begin() ) {
          pData = m_eptrData[iCount1].end();
        }                                
        else pData--;
        m_MemoryDC.LineTo(rectScroll.right - 1 - iCount2, rectScroll.bottom - 1 - int((*pData - m_eLowLimit)/eRangeRate));
      }
    }
    m_MemoryDC.SelectObject( penOld );
    pdc->BitBlt(rectArea.left, rectArea.top, 
                rectArea.Width(), rectArea.Height(), 
                &m_MemoryDC, 0, 0, SRCCOPY);
  }
	m_MemoryDC.SelectObject( pBitmap );
  m_fDrawAll = false;
  m_fNeedUpdate = false;
}

ParaName* CObjectGraph::GetParaNameAddress( void ) {
  return( sm_ptrParaName );
}     

CString CObjectGraph::GetParaName( ULONG index ) {
  ASSERT( index <= CObjectGraph::sm_ulStringEnd );
  return( CObjectGraph::sm_ptrParaName[index].Name );
}

ULONG CObjectGraph::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

void CObjectGraph::SelectParameter(ULONG ulType) {
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

INT32 CObjectGraph::GetIndex( ULONG ulIndex ) {
  return( CObjectGraph::sm_aulSuitable[ulIndex] );
}

bool CObjectGraph::SetDouble(ULONG ulIndex, double eValue) {
  ASSERT( ulIndex <= 3 );

  ASSERT(m_vfSelected[ulIndex] == TRUE);
  *m_ptrBegin[ulIndex] = eValue;
  if ( m_eUpperLimit < eValue ) { 
    m_eUpperLimit = eValue;
    m_fDrawAll = TRUE;
  }
  else if ( m_eLowLimit > eValue ) {
    m_eLowLimit = eValue; 
    m_fDrawAll = TRUE;
  }
  m_fNeedUpdate = TRUE;

  CSingleLock slLock( &m_csLock );
  slLock.Lock();
  if ( m_ptrBegin[ulIndex] == m_eptrData[ulIndex].end() ) {
    m_ptrBegin[ulIndex] = m_eptrData[ulIndex].begin();
  }
  else {
    m_ptrBegin[ulIndex]++; 
  }
  m_pScrollLength[ulIndex]++;
  slLock.Unlock();

  return(TRUE);
} 

bool CObjectGraph::SetProperty( void ) {
  CDlgSetGraphProperty CDlg;

  CDlg.SetData( GetDynLinkList()->size(), m_ulDataLength, m_ulInputDataType, 
                m_clrForeGrd, m_clrBkGrd, m_strName, m_lScanRate  );
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData( m_ulDataLength, m_ulInputDataType, m_clrForeGrd, 
                  m_clrBkGrd, m_strName, m_lScanRate  );
    m_fCreateMemoryDC = FALSE;
    return( TRUE );
  }
  return( FALSE );
}

bool CObjectGraph::CheckSelf( void ) {
	CString str;

	for(const auto pc : m_listDynLink) {
		if ( pc->GetLinkMethod() != IDD_OUTPUT_GRAPH ) {
			ShowMessage(ID_WARN_LINK_METHOD_MISMATCH, (LPCTSTR)m_strName);
			pc->SetLinkMethod( IDD_OUTPUT_GRAPH );
		}
	}
  ASSERT( (m_ulInputDataType == tBOOL) ||
          (m_ulInputDataType == tWORD) ||
          (m_ulInputDataType == tDOUBLE) );
          
	return( TRUE );
}
