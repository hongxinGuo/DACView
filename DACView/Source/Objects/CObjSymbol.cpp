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
	POSITION po = m_CObjectList.GetHeadPosition();
	INT_PTR i, iCount = m_CObjectList.GetCount();
	CObjectBase * pcobjTemp;
	for ( i = 0; i < iCount; i++ ) {
		pcobjTemp = m_CObjectList.GetNext(po);
		delete pcobjTemp;
		pcobjTemp = nullptr;
	}                   
	// release list's memory
	m_CObjectList.RemoveAll();

  po = m_CRectList.GetHeadPosition();
  iCount = m_CRectList.GetCount();
  CRect * prect;
  for ( i = 0; i < iCount; i++ ) {
    prect = m_CRectList.GetNext( po );
    delete prect;
    prect = nullptr;
  }
  m_CRectList.RemoveAll();
}

bool CObjectSymbol::IsNeedUpdate( void ) {
	POSITION Po = m_CObjectList.GetHeadPosition();
	CObjectBase * pcobjTemp;

  if ( m_fNeedUpdate ) return( TRUE );
	for ( int i = 0; i < m_CObjectList.GetCount(); i++ ) {
		pcobjTemp = m_CObjectList.GetNext(Po);
		if ( pcobjTemp->IsNeedUpdate() ) {
			return( TRUE );
		}
	}  
	return(FALSE);
}

bool CObjectSymbol::IsDrawAll( void ) {
  POSITION Po = m_CObjectList.GetHeadPosition();
	CObjectBase * pcobjTemp;

  if ( m_fDrawAll ) return( TRUE );
	for ( int i = 0; i < m_CObjectList.GetCount(); i++ ) {
		pcobjTemp = m_CObjectList.GetNext(Po);
		if ( pcobjTemp->IsDrawAll() ) {
			return( TRUE );
		}
	}  
	return(FALSE);
}

void CObjectSymbol::SetDrawAll( bool fFlag ) {
  POSITION Po = m_CObjectList.GetHeadPosition();
  CObjectBase * pcobjTemp;

  for ( int i = 0; i < m_CObjectList.GetCount(); i++ ) {
    pcobjTemp = m_CObjectList.GetNext(Po);
    pcobjTemp->SetDrawAll( fFlag );
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
	POSITION Po = m_CObjectList.GetHeadPosition();
	CObjectBase * pcobjTemp;
	INT_PTR iCount = m_CObjectList.GetCount();
  CPoint pt = GetOffset();

  m_rgnClip.DeleteObject();
	if ( m_fTransparent ) {
    if ( iCount < 1 ) {
      m_rgnClip.CreateRectRgn(0, 0, 0, 0);
      return( &m_rgnClip );
    }
    pcobjTemp = m_CObjectList.GetNext(Po);
    rect = pcobjTemp->GetSize();
    pt = pcobjTemp->GetOffset();
	  rect += pt - ptScrollPos;
    m_rgnClip.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
	  for ( int i = 1; i < iCount; i++ ) {
		  pcobjTemp = m_CObjectList.GetNext(Po);
		  m_rgnClip.CombineRgn( &m_rgnClip, 
                            pcobjTemp->GetClipRgn( ptScrollPos ), 
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
  CObjectBase * pcobjTemp;
  POSITION pos = m_CObjectList.GetHeadPosition();                          
	INT_PTR i, iTemp = m_CObjectList.GetCount();
  CRect rectTemp, rect;
  
  // set my area to union of all my children              
  rectTemp.SetRectEmpty();                
  for ( i = 0; i < iTemp; i ++ ) {                            
    pcobjTemp = m_CObjectList.GetNext(pos);
    rectTemp |= pcobjTemp->GetSize();
  }
  pos = m_CObjectList.GetHeadPosition();
  for ( i = 0; i < iTemp; i++ ) {
    pcobjTemp = m_CObjectList.GetNext( pos );
    rect = pcobjTemp->GetSize();
    rect -= rectTemp.TopLeft();
    pcobjTemp->SetAllSize( rect );
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
  POSITION po = m_CObjectList.GetHeadPosition();
  INT_PTR i, iCount = m_CObjectList.GetCount();
  CObjectBase * pcobj;
  CRect * prect;

  for ( i = 0; i < iCount; i++ ) {
    pcobj = m_CObjectList.GetNext( po );
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
  m_CObjectList.AddTail(pcobj); 
	
  TRACE("Current objects in symbol is %d\n", m_CObjectList.GetCount());
}

void CObjectSymbol::BreakSymbol( CObjectList * pObjList ) {
	CObjectBase * pcobjTemp;
	POSITION pos = m_CObjectList.GetHeadPosition();                          
	INT_PTR iTemp = m_CObjectList.GetCount();
  CRect rect;
                
  for ( int i = 0; i < iTemp; i ++ ) {                            
  	pcobjTemp = m_CObjectList.GetNext(pos);
    rect = pcobjTemp->GetSize();
    rect += m_rectArea.TopLeft();
    pcobjTemp->SetAllSize( rect );
    pcobjTemp->SetSymbolThatHaveMe( GetSymbolThatHaveMe() );
  	pcobjTemp->SetSelect(TRUE);
  	pObjList->AddTail( pcobjTemp );	
  }
	m_CObjectList.RemoveAll();  
}

void CObjectSymbol::ToShowStatic( CDC * const pdc, CPoint ptScrollPos ) {
	CObjectBase * pcobjTemp;
	POSITION pos = m_CObjectList.GetHeadPosition();                          
	INT_PTR iTemp = m_CObjectList.GetCount();
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
		for ( int i = 0; i < iTemp; i ++ ) {                            
	  	pcobjTemp = m_CObjectList.GetNext(pos);
	  	pcobjTemp->ToShowStatic( pdc, ptScrollPos ); 
	  }           
	}    
	if ( m_fSelected ) SetFocus( pdc );
}  

void CObjectSymbol::ToShowDynamic( CDC * const pdc ) {
	CObjectBase * pcobjTemp;
	POSITION pos;
  CBrush cbb, cb;
  CBitmap * pOldBitmap;
	INT_PTR i, iCount = m_CObjectList.GetCount();
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
  pos = m_CObjectList.GetHeadPosition();
  for ( i = 0; i < iCount; i++ ) {
    pcobjTemp = m_CObjectList.GetNext( pos );
    rectThis = pcobjTemp->GetLastSize();
    m_MemoryDC.FillRect( rectThis, & cbb );
  }
	// draw current view
  pos = m_CObjectList.GetHeadPosition();
  for ( i = 0; i < iCount; i++ ) {
    pcobjTemp = m_CObjectList.GetNext( pos );
    pcobjTemp->ToShowDynamic( &m_MemoryDC );
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
	POSITION pos, po;                          
	INT_PTR iTemp = m_CObjectList.GetCount(), i;
	int w1, h1, w2, h2;
	
  if ( m_rectArea != m_rectSymbolOrigin ) {
	  w1 = m_rectSymbolOrigin.Width();
	  h1 = m_rectSymbolOrigin.Height();
		if ( (w1 == 0) || (h1 == 0) ) return;		// 
	  w2 = m_rectArea.Width();
	  h2 = m_rectArea.Height();
		if ( (w2 <= 0) || (h2 <= 0) ) return;

	  pos = m_CObjectList.GetHeadPosition();
    po = m_CRectList.GetHeadPosition();
	  for ( i = 0; i < iTemp; i ++ ) {                            
	  	pcobjTemp = m_CObjectList.GetNext(pos);
      prect = m_CRectList.GetNext( po );
      rectTemp = *prect;
	  	rectTemp.left   = (rectTemp.left * w2) / w1;
	  	rectTemp.right  = (rectTemp.right * w2) / w1;
	  	rectTemp.top    = (rectTemp.top * h2) / h1;
	  	rectTemp.bottom = (rectTemp.bottom * h2) / h1;
	  	pcobjTemp->SetOriginSize( rectTemp );
      pcobjTemp->SetDynamicSize( rectTemp );
	  	pcobjTemp->AdjustDynamicInnerSize();
	  }
	}                                
}

void CObjectSymbol::AdjustInnerSize( void ) {
	CRect rectTemp, rect;
	CObjectBase * pcobjTemp;
	POSITION pos;
	INT_PTR iTemp = m_CObjectList.GetCount(), i;
	int w1, h1, w2, h2;
		                        
	rectTemp.SetRectEmpty();
	pos = m_CObjectList.GetHeadPosition(); 
	for ( i = 0; i < iTemp; i ++ ) {                            
  	pcobjTemp = m_CObjectList.GetNext(pos);
  	rectTemp |= pcobjTemp->GetSize(); 
  }

  w1 = rectTemp.Width();
  h1 = rectTemp.Height();
	if ( (w1 == 0) || (h1 == 0) ) return;		// 
  w2 = m_rectArea.Width();
  h2 = m_rectArea.Height();
	if ( (w2 <= 0) || (h2 <= 0) ) return;
  pos = m_CObjectList.GetHeadPosition();
  for ( i = 0; i < iTemp; i ++ ) {                            
  	pcobjTemp = m_CObjectList.GetNext(pos);
  	rect = pcobjTemp->GetSize(); 
  	rect.left   = rect.left * w2 / w1;
  	rect.right  = rect.right * w2 / w1;
  	rect.top    = rect.top * h2 / h1;
  	rect.bottom = rect.bottom * h2 / h1;
  	pcobjTemp->SetAllSize( rect ); 
  	pcobjTemp->AdjustInnerSize();
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

