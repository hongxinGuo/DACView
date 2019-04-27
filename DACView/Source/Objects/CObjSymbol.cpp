#include "stdafx.h"

#include "globedef.h"
#include "..\\resource.h"

#include "cObjSymbol.h"
#include "DlgObjSymbol.h"	  

IMPLEMENT_SERIAL(CObjectSymbol, CObjectComponentBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

CObjectSymbol::CObjectSymbol(CString s, CRect r) 
  : CObjectComponentBase(s, r) {
	m_fChangeSize = FALSE;
  m_clrBkGrd = RGB(255, 255, 255);
  m_fCreateMemoryDC = FALSE;
}

CObjectSymbol::CObjectSymbol( void ) : CObjectComponentBase( ) {
	m_fChangeSize = FALSE;
  m_clrBkGrd = RGB(255, 255, 255);
  m_fCreateMemoryDC = FALSE;
}
	
CObjectSymbol::~CObjectSymbol() {
	// release object's memory
  for (auto pcObj : m_CObjectList) {
		delete pcObj;
	}                   
	// release list's memory
	m_CObjectList.clear();

  POSITION po = m_CRectList.GetHeadPosition();
  INT_PTR iCount = m_CRectList.GetCount();
  CRect * prect;
  for ( int i = 0; i < iCount; i++ ) {
    prect = m_CRectList.GetNext( po );
    delete prect;
    prect = nullptr;
  }
  m_CRectList.RemoveAll();
}

bool CObjectSymbol::IsNeedUpdate( void ) {

  if ( m_fNeedUpdate ) return( TRUE );
  for (auto pcObj : m_CObjectList) {
		if ( pcObj->IsNeedUpdate() ) {
			return( TRUE );
		}
	}  
	return(FALSE);
}

bool CObjectSymbol::IsDrawAll( void ) {

  if ( m_fDrawAll ) return( TRUE );
  for (auto pcobj : m_CObjectList) {
		if ( pcobj->IsDrawAll() ) {
			return( TRUE );
		}
	}  
	return(FALSE);
}

void CObjectSymbol::SetDrawAll( bool fFlag ) {
  for (auto pcobj : m_CObjectList) {
    pcobj->SetDrawAll( fFlag );
  }
}

/*
  return the clip region.
  if this symbol's background is transparent, the clip region is
  the combination of all its children's region, else the clip
  region is the rect area of this symbol.
*/

CRgn * CObjectSymbol::GetClipRgn( const CPoint& ptScrollPos ) {
	CRect rect;
	CObjectBase * pcobj;
  CPoint pt = GetOffset();

  m_rgnClip.DeleteObject();
	if ( m_fTransparent ) {
    if ( m_CObjectList.size() < 1 ) {
      m_rgnClip.CreateRectRgn(0, 0, 0, 0);
      return( &m_rgnClip );
    }
    auto it = m_CObjectList.begin();
    pcobj = *it++;
    rect = pcobj->GetSize();
    pt = pcobj->GetOffset();
	  rect += pt - ptScrollPos;
    m_rgnClip.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
    for (; it != m_CObjectList.end(); it++) {
		  pcobj = *it;
		  m_rgnClip.CombineRgn( &m_rgnClip, 
                            pcobj->GetClipRgn( ptScrollPos ), 
                            RGN_OR );
	  }
  }
  else {
    rect = m_rectArea + GetOffset() - ptScrollPos;
	  m_rgnClip.CreateRectRgn(rect.left, rect.top, 
                            rect.right, rect.bottom);
  }
  return( &m_rgnClip );
}

void CObjectSymbol::ViewOut( void ) {
  CRect rectTemp, rect;
  
  // set my area to union of all my children              
  rectTemp.SetRectEmpty();                
  for (auto pcobj : m_CObjectList) {
    rectTemp |= pcobj->GetSize();
  }
  for (auto pcobj : m_CObjectList) {
    rect = pcobj->GetSize();
    rect -= rectTemp.TopLeft();
    pcobj->SetAllSize( rect );
  }
  m_rectOrigin.left = m_rectArea.left = m_rectLastTime.left = m_rectOrigin.left + rectTemp.left;
  m_rectOrigin.top = m_rectArea.top = m_rectLastTime.top = m_rectOrigin.top + rectTemp.top;
  m_rectOrigin.right = m_rectArea.right = m_rectLastTime.right = m_rectOrigin.left + rectTemp.Width();
  m_rectOrigin.bottom = m_rectArea.bottom = m_rectLastTime.bottom = m_rectOrigin.top + rectTemp.Height();
}
	     	               
/////////////////////////////////////////////////////////////////////////////
// CObjectSymbol diagnostics

#ifdef _DEBUG
void CObjectSymbol::AssertValid() const
{
	CObjectComponentBase::AssertValid();
}

void CObjectSymbol::Dump(CDumpContext& dc) const
{
	CObjectComponentBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectRect member function

void CObjectSymbol::Serialize( CArchive& ar ) {	
	CObjectComponentBase::Serialize( ar );
  CRect * prect;

  for (auto pcobj : m_CObjectList) {
    prect = new CRect;
    *prect = pcobj->GetSize();
    m_CRectList.AddTail( prect );
    pcobj->SetSymbolThatHaveMe( this );
  }
  m_rectSymbolOrigin = m_rectOrigin;
}

bool CObjectSymbol::SetInteger(ULONG index, LONG lValue) {
  CObjRectBase::SetInteger(index, lValue);

	if ( m_fChangeSize ) {
    m_fNeedUpdate = TRUE;
    m_fDrawAll = TRUE;
		AdjustDynamicInnerSize();
		m_fChangeSize = FALSE;
	}
	return ( TRUE );
}

bool CObjectSymbol::SetRectPosition(ULONG index, LONG lValue) {
  CObjRectBase::SetRectPosition( index, lValue );

  return( FALSE );
}

const CString& CObjectSymbol::GetClassNameStr( void ) {
  static CString s_strClassName = "Symbol";

  return(s_strClassName);
}

bool CObjectSymbol::ExectiveDynLink( ) {
	return( CObjectComponentBase::ExectiveDynLink() );
}

void CObjectSymbol::InsertObject( CObjectBase * pcobj ) {
  m_CObjectList.push_back(pcobj); 
	
  TRACE("Current objects in symbol is %d\n", m_CObjectList.size());
}

void CObjectSymbol::BreakSymbol( CObjectList * pObjList ) {
  CRect rect;
                
  for (auto pcobj : m_CObjectList) {
    rect = pcobj->GetSize();
    rect += m_rectArea.TopLeft();
    pcobj->SetAllSize( rect );
    pcobj->SetSymbolThatHaveMe( GetSymbolThatHaveMe() );
  	pcobj->SetSelect(TRUE);
  	pObjList->push_back( pcobj );	
  }
	m_CObjectList.clear();  
}

void CObjectSymbol::ToShowStatic( CDC * const pdc, CPoint ptScrollPos ) {
  CRect rectArea = m_rectArea;
  CPoint pt = GetOffset();

  rectArea += pt;
	if ( pdc->RectVisible(rectArea) ) {
		// draw back ground color
		if ( !m_fTransparent ) {
      CBrush cb;
	    cb.CreateSolidBrush( m_clrBkGrd );
	    pdc->FillRect(rectArea, &cb);           // Yes, to draw myself
    }

		// draw objects
    for (auto pcobj : m_CObjectList) {
	  	pcobj->ToShowStatic( pdc, ptScrollPos ); 
	  }           
	}    
	if ( m_fSelected ) SetFocus( pdc );
}  

void CObjectSymbol::ToShowDynamic( CDC * const pdc ) {
	CObjectBase * pcobjTemp;
	POSITION pos;
  CBrush cbb, cb;
  CBitmap * pOldBitmap;
	INT_PTR i, iCoun;
  CRect rectThis, rectLast, rectIntersect;

  CRect rectArea = m_rectArea;
  m_rectLastTime = m_rectArea;

  if ( !m_fCreateMemoryDC ) {
    rectThis = m_rectSymbolOrigin;
    rectThis -= m_rectSymbolOrigin.TopLeft();
    m_MemoryDC.CreateCompatibleDC( pdc );
    m_Bitmap.CreateCompatibleBitmap( pdc, rectThis.Width(), rectThis.Height());
    pOldBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
    if ( !m_fTransparent ) {
      cb.CreateSolidBrush( m_clrBkGrd );
    }
    else {
      cb.CreateSolidBrush( RGB(255, 255, 255) );
    }
    m_MemoryDC.FillRect(rectThis, &cb);
		m_MemoryDC.SelectObject( pOldBitmap );
    m_fCreateMemoryDC = TRUE;
  }
  
  // ASSERT( GetSymbolThatHaveMe() != nullptr );
  // show object
  pOldBitmap = m_MemoryDC.SelectObject( &m_Bitmap );
  if ( !m_fTransparent ) {
    cbb.CreateSolidBrush( m_clrBkGrd );		// use symbol's back ground color
  }
  else {
		CPoint pt( 0, 0 );
    cbb.CreateSolidBrush( RGB(255, 255, 255) ); // use white color as back ground
		// set current clip reign as symbol's reign
		pdc->SelectClipRgn( GetClipRgn(pt), RGN_AND ); 
  }
	// clear last view
  for (auto pcobj : m_CObjectList) {
    rectThis = pcobj->GetLastSize();
    m_MemoryDC.FillRect( rectThis, & cbb );
  }
	// draw current view
  for (auto pcobj : m_CObjectList) {
    pcobj->ToShowDynamic( &m_MemoryDC );
  }
	// show it on upper layer's screen
  pdc->BitBlt(rectArea.left, rectArea.top, 
              rectArea.Width(), rectArea.Height(), 
              &m_MemoryDC, 0, 0, SRCCOPY);
	if ( m_fTransparent ) {
		// set clip reign to all screen
		CRgn rgnTemp;
		rgnTemp.CreateRectRgn(0, 0, gl_sizeDoc.cx, gl_sizeDoc.cy );
		pdc->SelectClipRgn( &rgnTemp, RGN_OR );
	}
	m_MemoryDC.SelectObject( pOldBitmap );	// reset bitmap
  m_fDrawAll = FALSE;
  m_fNeedUpdate = FALSE;
}  

void CObjectSymbol::AdjustDynamicInnerSize( void ) {
	CRect rectTemp;
	CObjectBase * pcobjTemp;
  CRect * prect;
  INT64 iCount;
	POSITION pos, po;                          
	int w1, h1, w2, h2;
	
  if ( m_rectArea != m_rectSymbolOrigin ) {
	  w1 = m_rectSymbolOrigin.Width();
	  h1 = m_rectSymbolOrigin.Height();
		if ( (w1 == 0) || (h1 == 0) ) return;		// 
	  w2 = m_rectArea.Width();
	  h2 = m_rectArea.Height();
		if ( (w2 <= 0) || (h2 <= 0) ) return;

    for (auto pcobj : m_CObjectList) {
      po = m_CRectList.GetHeadPosition();
      iCount = m_CRectList.GetCount();
      for (int i = 0; i < iCount; i++) { // 此段可能有问题。
        prect = m_CRectList.GetNext(po);
        rectTemp = *prect;
        rectTemp.left = (rectTemp.left * w2) / w1;
        rectTemp.right = (rectTemp.right * w2) / w1;
        rectTemp.top = (rectTemp.top * h2) / h1;
        rectTemp.bottom = (rectTemp.bottom * h2) / h1;
        pcobj->SetOriginSize(rectTemp);
        pcobj->SetDynamicSize(rectTemp);
        pcobj->AdjustDynamicInnerSize();
      }
	  }
	}                                
}

void CObjectSymbol::AdjustInnerSize( void ) {
	CRect rectTemp, rect;
	POSITION pos;
	int w1, h1, w2, h2;
		                        
	rectTemp.SetRectEmpty();
  for (auto pcobj : m_CObjectList) {
  	rectTemp |= pcobj->GetSize(); 
  }

  w1 = rectTemp.Width();
  h1 = rectTemp.Height();
	if ( (w1 == 0) || (h1 == 0) ) return;		// 
  w2 = m_rectArea.Width();
  h2 = m_rectArea.Height();
	if ( (w2 <= 0) || (h2 <= 0) ) return;
  for (auto pcobj : m_CObjectList) {
  	rect = pcobj->GetSize(); 
  	rect.left   = rect.left * w2 / w1;
  	rect.right  = rect.right * w2 / w1;
  	rect.top    = rect.top * h2 / h1;
  	rect.bottom = rect.bottom * h2 / h1;
  	pcobj->SetAllSize( rect ); 
  	pcobj->AdjustInnerSize();
  }                                
}

bool CObjectSymbol::DumpCurrentStatus( char * pch ) const {
  sprintf(pch, "%s:  ", m_strName );
#ifdef _DEBUG
  Dump( afxDump );
#endif
  return( TRUE );
}

bool CObjectSymbol::SetProperty( void ) {
  CDlgSetSymbolProperty CDlg;
  CDlg.SetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_lScanRate, m_fTransparent );
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_lScanRate, m_fTransparent );
    m_fCreateMemoryDC = FALSE;
    return( TRUE );
  }
  return( FALSE );
}

bool CObjectSymbol::CheckSelf( void ) {
  return( TRUE );
}

