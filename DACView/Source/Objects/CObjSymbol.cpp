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

ParaName CObjectSymbol::sm_ptrParaName[] =
{ {"Visibility", tINPUT | tMODIFIABLE | tBOOL, 0},
  {"Height", tINPUT | tMODIFIABLE | tWORD | tDOUBLE,  1},
  {"Width",  tINPUT | tMODIFIABLE | tWORD | tDOUBLE,  2},
  {"BackColor",tINPUT | tMODIFIABLE | tWORD,  3},
  {""     , 0, 4},
};

INT32 CObjectSymbol::sm_aulSuitable[] = { -1, -1, -1, -1, -1 };

const ULONG CObjectSymbol::sm_ulDoubleEnd = 0;
const ULONG CObjectSymbol::sm_ulBoolEnd = 1;
const ULONG CObjectSymbol::sm_ulWordEnd = 3;
const ULONG CObjectSymbol::sm_ulStringEnd = 3;

CObjectSymbol::CObjectSymbol(CString s, CRect r) 
  : CObjectComponentBase(s, r) {
	m_fChangeSize = FALSE;
  m_clrBkGrd = RGB(255, 255, 255);
  m_fCreateMemoryDC = FALSE;

  for (int i = 0; i < sm_ulStringEnd + 1; i++) {
    m_vfSelected.push_back(false);
  }
}

CObjectSymbol::CObjectSymbol( void ) : CObjectComponentBase( ) {
	m_fChangeSize = FALSE;
  m_clrBkGrd = RGB(255, 255, 255);
  m_fCreateMemoryDC = FALSE;

  for (int i = 0; i < sm_ulStringEnd + 1; i++) {
    m_vfSelected.push_back(false);
  }
}
	
CObjectSymbol::~CObjectSymbol() {
	// release object's memory
  for (auto pcObj : m_CObjectList) {
		delete pcObj;
	}                   
	// release list's memory
	m_CObjectList.clear();

  m_CRectList.clear();
}

bool CObjectSymbol::IsNeedUpdate( void ) {

  if ( m_fNeedUpdate ) return(true);
  for (auto pobj : m_CObjectList) {
		if ( pobj->IsNeedUpdate() ) {
			return(true);
		}
	}  
	return(false);
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

CPoint CObjectSymbol::GetOffset(void) {
  CPoint pt(0, 0);
  CRect rect;

  if (m_pSymbolHaveMe != nullptr) {
    rect = m_pSymbolHaveMe->GetSize();
    pt = m_pSymbolHaveMe->GetOffset();
    pt.x += rect.left;
    pt.y += rect.top;
  }
  return(pt);
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
  shared_ptr<CRect> prect;

  if (ar.IsLoading()) {
    for (auto pobj : m_CObjectList) {
      prect = make_shared<CRect>(pobj->GetSize());
      m_CRectList.push_back(prect);
      pobj->SetSymbolThatHaveMe(this);
    }
    m_rectSymbolOrigin = m_rectOrigin;
  }
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
  CBrush cbb, cb;
  CBitmap * pOldBitmap;
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
	int w1, h1, w2, h2;
	
  if ( m_rectArea != m_rectSymbolOrigin ) {
	  w1 = m_rectSymbolOrigin.Width();
	  h1 = m_rectSymbolOrigin.Height();
		if ( (w1 == 0) || (h1 == 0) ) return;		// 
	  w2 = m_rectArea.Width();
	  h2 = m_rectArea.Height();
		if ( (w2 <= 0) || (h2 <= 0) ) return;

    for (auto pcobj : m_CObjectList) {
      for (const auto prect : m_CRectList) { // 此段可能有问题。
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
  sprintf_s(pch, 50, "%s:  ", m_strName );
#ifdef _DEBUG
  Dump( afxDump );
#endif
  return( true );
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


ParaName* CObjectSymbol::GetParaNameAddress(void) {
  return(sm_ptrParaName);
}

CString CObjectSymbol::GetParaName(ULONG index) {
  ASSERT(index <= CObjectSymbol::sm_ulStringEnd);
  return(CObjectSymbol::sm_ptrParaName[index].Name);
}

ULONG CObjectSymbol::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

void CObjectSymbol::SelectParameter(ULONG ulType) {
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

INT32 CObjectSymbol::GetIndex(ULONG ulIndex) {
  return(CObjectSymbol::sm_aulSuitable[ulIndex]);
}


