#include "stdafx.h"    

#include "CUnitBase.h"

#include "cObjRectBase.h"
#include "DlgObjRect.h"

#include "CObjSymbol.h"

#include "cObjDynLink.h"

IMPLEMENT_SERIAL(CObjRectBase, CObjectBase, 1 | VERSIONABLE_SCHEMA);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

COLORREF rgbArray[16] = {	RGB(0, 0, 0), RGB(32, 32, 32), RGB(64, 64, 64), RGB(96, 96, 96),
													RGB(128, 128, 128), RGB(160, 160, 160), RGB(192, 192, 192), RGB(224, 224, 224),
													RGB(255, 0, 0), RGB(255, 32, 32), RGB(255, 64, 64), RGB(255, 96, 96),
													RGB(255, 128, 128), RGB(255, 160, 160), RGB(255, 192, 192), RGB(255, 224, 224),

												};	

ParaName CObjRectBase::sm_ptrParaName[] = 
    { {"Visibility", tINPUT | tMODIFIABLE | tBOOL, 0},
      {"Height", tINPUT | tMODIFIABLE | tWORD | tDOUBLE,  1}, 
      {"Width",  tINPUT | tMODIFIABLE | tWORD | tDOUBLE,  2},
      {"BackColor",tINPUT | tMODIFIABLE | tWORD,  3}, 
      {"ForeColor",tINPUT | tMODIFIABLE | tWORD,  4}, 
      {"Title", tINPUT | tMODIFIABLE | tDOUBLE | tWORD | tBOOL | tSTRING,  5},
      {""     , 0, 6},
    }; 
    
INT32 CObjRectBase::sm_aulSuitable[] = { -1, -1, -1, -1, -1, -1, -1};

const ULONG CObjRectBase::sm_ulDoubleEnd = 0;
const ULONG CObjRectBase::sm_ulBoolEnd   = 1;
const ULONG CObjRectBase::sm_ulWordEnd   = 4;
const ULONG CObjRectBase::sm_ulStringEnd = 5;                    

CObjRectBase::CObjRectBase(const CString& Name, const CRect& Area,
                         COLORREF BkGrd, COLORREF ForeGrd)
  : CObjectBase(Name, BkGrd, ForeGrd) {
  m_rectArea = m_rectLastTime = m_rectOrigin = Area;
  m_strTitle.Empty();
  
  dm_strName = Name;
  dm_clrBkGrd  = BkGrd;
  dm_clrForeGrd = ForeGrd;

  m_pfSelected = new bool[6];
  for (int i = 0; i < 6; i++) {
    m_pfSelected[i] = false;
  }

}      

CObjRectBase::CObjRectBase( void ) : CObjectBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
	m_strTitle.Empty();
  m_rectArea = m_rectLastTime = m_rectOrigin = rectTemp;
  m_clrBkGrd = RGB(0, 0, 0);
  m_clrForeGrd = RGB(255, 255, 255);


  m_pfSelected = new bool[6];
  for (int i = 0; i < 6; i++) {
    m_pfSelected[i] = false;
  }

}

CObjRectBase::~CObjRectBase() {
  delete[] m_pfSelected;
  m_pfSelected = nullptr;
}

bool CObjRectBase::IsRectShape( void ) {
  return( TRUE );
}

void CObjRectBase::Serialize( CArchive& ar ) {
  CObjectBase::Serialize( ar );
  if( ar.IsStoring() ) {
    ar << m_rectOrigin << m_strTitle;
  }
  else {
    ar >> m_rectOrigin >> m_strTitle;
    m_rectArea = m_rectLastTime = m_rectOrigin;
#ifdef _DEBUG
		m_rectTest = m_rectArea;
#endif    
    m_rgnClip.DeleteObject();
    m_rgnClip.CreateRectRgn(m_rectArea.left, m_rectArea.top, m_rectArea.right, m_rectArea.bottom);
  }                                  
}

//////////////////////////////////////////////////////////////////////////////
//
// 执行各项动态链接
//
//////////////////////////////////////////////////////////////////////////////
bool CObjRectBase::ExectiveDynLink( void ) {
	POSITION po = m_listDynLink.GetHeadPosition();
	CObjectDynLink * pcobjDynLink;
	CUnitBase * pcunit;
	ULONG ulUnitIndex, ulObjectIndex;
	CString strTemp;
	char str[256];
	LONG lTemp;
	double eTemp;
	bool fTemp;
	bool fUpdate = FALSE, fEndUpdate = FALSE;
	INT_PTR iTemp = m_listDynLink.GetCount();

  if ( !m_fNeedUpdate ) {
    m_fChangeSize = FALSE;
    for ( int i = 0; i < iTemp; i++ ) {
  	  pcobjDynLink = m_listDynLink.GetNext(po);
  	  pcunit = pcobjDynLink->GetUnit();                    
  	  ulUnitIndex = pcobjDynLink->GetUnitIndex();
  	  ulObjectIndex = pcobjDynLink->GetObjectIndex();
      ASSERT((pcunit->GetDynLinkType(ulUnitIndex) & (tINPUT | tOUTPUT | tMODIFIABLE)) == 0);
  	  switch ( pcunit->GetDynLinkType(ulUnitIndex) ) {
  	  case tDOUBLE :
  		  eTemp = pcunit->GetDouble(ulUnitIndex);
  		  switch ( pcobjDynLink->GetLinkMethod() ) {
    ////////////////////////////////////////////////////////////////////////////////////////
  		  case IDD_OUTPUT_OBJECT_SIZE :
  			  switch ( pcobjDynLink->GetUnitDynLinkType() ) {
  			  case tWORD :
  			  case tDOUBLE :
  			    fUpdate = SetInteger(ulObjectIndex, (LONG)eTemp);
  				  break;
  			  default :
  				  ASSERT(FALSE);
  			  } // switch Link Type
  			  break;
    /////////////////////////////////////////////////////////////////////////////////////////
  		  case IDD_OUTPUT_LOCATION :
  			  switch ( pcobjDynLink->GetUnitDynLinkType() ) {
  			  case tWORD :
          case tDOUBLE :
  				  fUpdate = SetRectPosition(ulObjectIndex, (LONG)eTemp);
  				  break;
  			  default :
  				  ASSERT(FALSE);
  			  }	// switch link type
  			  break;
    /////////////////////////////////////////////////////////////////////////////////////////
  		  case IDD_OUTPUT_FILL_COLOR :
  			  switch ( pcobjDynLink->GetUnitDynLinkType() ) {
  			  case tWORD :
  			  case tDOUBLE :
  				  fUpdate = SetColor( ulObjectIndex, (LONG)eTemp);	
  				  break;
  			  default :
  				  ASSERT(FALSE);
  			  } // switch link type
  			  break;
    //////////////////////////////////////////////////////////////////////////////////////////
  		  case IDD_OUTPUT_STRING :
  			  gcvt( eTemp, 8, str );
  			  strTemp = str;
  			  fUpdate = SetString( ulObjectIndex, strTemp );
  			  break;
		////////////////////////////////////////////////////////////////////////////
				case IDD_OUTPUT_VISIBILITY :
					fUpdate = SetBool( 0, (bool)eTemp );
					break;
    /////////////////////////////////////////////////////////////////////////////////////////
  		  default :
          ASSERT(FALSE);
  			  break;
  		  } // switch Link Method
  		  break;
  	  case tWORD :
  		  lTemp = pcunit->GetInteger(ulUnitIndex);
  		  switch ( pcobjDynLink->GetLinkMethod() ) {
    ////////////////////////////////////////////////////////////////////////////////////////
  		  case IDD_OUTPUT_OBJECT_SIZE :
  			  switch ( pcobjDynLink->GetUnitDynLinkType() ) {
  			  case tWORD :
  			  case tDOUBLE :
  				  fUpdate = SetDouble(ulObjectIndex, lTemp);
  				  break;
  			  default :
  				  ASSERT(FALSE);
  			  } // switch Link Type
  			  break;
    /////////////////////////////////////////////////////////////////////////////////////////
  		  case IDD_OUTPUT_LOCATION :
  			  switch ( pcobjDynLink->GetUnitDynLinkType() ) {
  			  case tWORD :
          case tDOUBLE :
  				  fUpdate = SetRectPosition(ulObjectIndex, lTemp);
  				  break;
  			  default :
  				  ASSERT(FALSE);
  			  }	// switch link type
  			  break;
    /////////////////////////////////////////////////////////////////////////////////////////
  		  case IDD_OUTPUT_FILL_COLOR :
  			  switch ( pcobjDynLink->GetUnitDynLinkType() ) {
  			  case tWORD :
  			  case tDOUBLE :
  				  fUpdate = SetColor( ulObjectIndex, lTemp);
  				  break;
  			  default :
  				  ASSERT(FALSE);
  			  } // switch link type
  			  break;
    //////////////////////////////////////////////////////////////////////////////////////////
  		  case IDD_OUTPUT_STRING :
  			  lTemp = pcunit->GetInteger(ulUnitIndex);
  			  _itoa( iTemp, str, 10 );
  			  strTemp = str;
  			  fUpdate = SetString( ulObjectIndex, strTemp );
  			  break;
    /////////////////////////////////////////////////////////////////////////////////////////
				case IDD_OUTPUT_VISIBILITY :
					fUpdate = SetBool( 0, lTemp );
					break;
		////////////////////////////////////////////////////////////////////////
				default :
          ASSERT(FALSE);
  			  break;
  		  } // switch Link Method
  		  break;
  	  case tBOOL :
  		  fTemp = pcunit->GetBool(ulUnitIndex);
  		  switch ( pcobjDynLink->GetLinkMethod() ) {
    //////////////////////////////////////////////////////////////////////////////////////////
  		  case IDD_OUTPUT_STRING :
  			  if ( pcunit->GetBool(ulUnitIndex) == TRUE ) {
  				  strTemp = "TRUE";
  			  }
  			  else {
  				  strTemp = "FALSE";
  			  }
  			  fUpdate = SetString( ulObjectIndex, strTemp );
  			  break;
    /////////////////////////////////////////////////////////////////////////////////////////
        case IDD_OUTPUT_VISIBILITY :
          fUpdate = SetBool(ulObjectIndex, fTemp);
          break;
  		  default :
          ASSERT(FALSE);
  			  break;
  		  } // switch Link Method
  		  break;
  	  case tSTRING :		
  		  switch ( pcobjDynLink->GetLinkMethod() ) {
  		  case IDD_OUTPUT_STRING :
  			  strTemp = pcunit->GetString( ulUnitIndex );
  			  fUpdate = SetString( ulObjectIndex, strTemp );
  			  break;
    /////////////////////////////////////////////////////////////////////////////////////////
  		  default :
          ASSERT(FALSE);
  			  break;
  		  } // switch Link Method
  	  default :
        ASSERT(FALSE);
  		  break;
  	  } // switch pcunit->GetDynLinkType
  	  if ( fUpdate ) fEndUpdate = TRUE;
    } // for 
    if ( fEndUpdate ) m_fNeedUpdate = TRUE;
  }
  		  
	return( TRUE );
}

ParaName* CObjRectBase::GetParaNameAddress( void ) {
  return( sm_ptrParaName );
}     

CString CObjRectBase::GetParaName( ULONG index ) {
	ASSERT( index <= CObjRectBase::sm_ulStringEnd );
	return( CObjRectBase::sm_ptrParaName[index].Name );
}

ULONG CObjRectBase::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

void CObjRectBase::SelectParameter(ULONG ulType) {
  int i = 0;
  int j = 0;

  for (int k = 0; k <= sm_ulStringEnd; k++) {
    sm_aulSuitable[k] = -1;
  }
  while ( sm_ptrParaName[i].ulType != 0 ) {
    if ( (sm_ptrParaName[i].ulType | ulType) == sm_ptrParaName[i].ulType ) {
			if ( ulType & tINPUT ) {
				if ( m_pfSelected[i] == FALSE ) {
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

INT32 CObjRectBase::GetIndex( ULONG ulIndex ) {
	return( CObjRectBase::sm_aulSuitable[ulIndex] );
}

bool CObjRectBase::SetRectPosition(ULONG index, LONG lValue) {
	ASSERT((index > 0 ) && (index <= 2)); 	// only height and width
  int iTemp;
	
  if ( index == 1 ) {		// height
    iTemp = m_rectArea.Height();
		m_rectArea.top = m_rectOrigin.top + lValue;
		m_rectArea.bottom = m_rectArea.top + iTemp; 
	}
	else {								// width
    iTemp = m_rectArea.Width();
		m_rectArea.left = m_rectOrigin.left + lValue;
		m_rectArea.right = m_rectArea.left + iTemp;
	}

	if ( m_rectLastTime != m_rectArea ) {
    m_fDrawAll = TRUE;
    return( TRUE );
  }
	else return( FALSE );
}

bool CObjRectBase::SetBool(ULONG index, bool fValue) {
  ASSERT( index == 0 );		// must been 0
	if ( m_fTransparent != fValue ) {
		m_fTransparent = fValue;
		m_fNeedUpdate = m_fDrawAll = fValue;
		return( TRUE );
	}
	else return( FALSE );
}

///////////////////////////////////////////////////////////////////////
//
// SetInteger
//
// Description :
//   Change the width or/and height of this object.
//   
///////////////////////////////////////////////////////////////////////
bool CObjRectBase::SetInteger(ULONG index, LONG lValue) {
  ASSERT((index > 0 ) && (index <= 2));

	switch ( index ) {
	case 1 : // height
		if ( (lValue > 0) && (lValue < m_rectOrigin.Height()) ) {
	 		m_rectArea.top = m_rectArea.bottom - lValue;
		}
	  break;
	case 2 : // width
	  if ( (lValue < m_rectOrigin.Width()) && (lValue > 0) ) {
      m_rectArea.right = m_rectArea.left + lValue;
		}
	  break;
	default :
	  ASSERT( FALSE );
  }

  if ( m_rectLastTime != m_rectArea ) {
    m_fDrawAll = TRUE;
		m_fChangeSize = TRUE;
    return( TRUE );
  }
	else return( FALSE );
}

bool CObjRectBase::SetDouble(ULONG , double ) {
  ASSERT( FALSE );
  return(FALSE);
}
double CObjRectBase::GetDouble(ULONG index)
{
	ASSERT(false);
	return 0.0;
}


bool CObjRectBase::SetString(ULONG ulIndex, const CString & strValue) {
  ASSERT( ulIndex == 5 );
  m_strTitle = strValue;
  return(TRUE);
}   

bool CObjRectBase::SetColor(ULONG index, DWORD lValue) {
  ASSERT((index >= 3) && (index <= 4));

  switch ( index ) {
  case 4 :
		if ( (lValue < 16) && ( lValue >= 0) ) {
			m_clrForeGrd = rgbArray[lValue];
		}
		else m_clrForeGrd = lValue;
		m_fChangeSize = TRUE;
    break;
  case 3 :
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

bool CObjRectBase::InIt( CDC * const , POINT const pt, int ) {
  CRect rectArea = m_rectArea;
  CPoint ptOffsetBase = GetOffset();

  rectArea += ptOffsetBase;
  return ( rectArea.PtInRect(pt) );
} 

void CObjRectBase::ToShowStatic( CDC * const, CPoint   ) {
  ASSERT( FALSE );
}            

void CObjRectBase::ToShowDynamic( CDC * const ) {
  ASSERT( FALSE );
}


void CObjRectBase::AdjustInnerSize( void ) {
  // do nothing , use by CObjectSymbol
}

void CObjRectBase::AdjustDynamicInnerSize( void ) {
	// do nothing, use by CObjectSymbol
}

void CObjRectBase::SetFocus( CDC *pdc) {
  int left, top, bottom,  right;
  CRect arrayRect[8];
  CRect rect = m_rectArea;

  rect += GetOffset();
  left   = rect.left;
  right = rect.right;
  top    = rect.top;
  bottom = rect.bottom;
                          
  arrayRect[0].top    = top;
  arrayRect[0].left   = left;     
  
  arrayRect[2].top    = top;
  arrayRect[2].left   = right-8; 
  
  arrayRect[1].top    = top;
  arrayRect[1].left   = left + (right - left)/2 - 4;  
  
  arrayRect[3].top    = top + (bottom - top)/2 - 4;
  arrayRect[3].left   = left;     
  
  arrayRect[4].top    = top + (bottom - top)/2 - 4;
  arrayRect[4].left   = right -8;                 
  
  arrayRect[5].top    = bottom - 8;
  arrayRect[5].left   = left;     
  
  arrayRect[7].top    = bottom - 8;
  arrayRect[7].left   = right - 8; 
  
  arrayRect[6].top    = bottom - 8 ;
  arrayRect[6].left   = left + (right - left)/2 - 4; 
  
  for ( int i = 0; i < 8; i ++ ) {
		pdc->PatBlt(arrayRect[i].left, arrayRect[i].top, 8, 8, DSTINVERT);
  }
}
 
bool CObjRectBase::SetProperty( void ) {
  CDlgSetProperty CDlg;
  CDlg.SetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_lScanRate );
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_lScanRate );
    return( TRUE );
  }
  return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
// CObjRectBase diagnostics

#ifdef _DEBUG
void CObjRectBase::AssertValid() const
{
  CObjectBase::AssertValid();
	ASSERT((m_rectOrigin.Width() > 0) && (m_rectOrigin.Height() > 0));
}

void CObjRectBase::Dump(CDumpContext& dc) const
{
  CObjectBase::Dump(dc);
  dc << "\nm_rectOrigin = " << m_rectOrigin;
  dc << "\nm_rectArea = " << m_rectArea;
  dc << "\nm_strTitle = " << m_strTitle;
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

const CRect& CObjRectBase::GetSize( void ) {
  return( m_rectArea );
}

const CRect& CObjRectBase::GetOriginSize( void ) {
	return ( m_rectOrigin );
} 

const CRect& CObjRectBase::GetLastSize( void ) {
	return ( m_rectLastTime );
} 

CRgn * CObjRectBase::GetClipRgn( const CPoint& ptScrollPosition ) {
	CRect rect;
  CPoint pt = GetOffset();
	m_rgnClip.DeleteObject();
	rect = m_rectArea;
	rect += pt -ptScrollPosition;
	m_rgnClip.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
	return( &m_rgnClip );
}

void CObjRectBase::SetAllSize( const CRect&rectArea ) {
  m_rectOrigin = m_rectArea = m_rectLastTime = rectArea;
}

void CObjRectBase::SetOriginSize( const CRect& rectOrigin ) {
  m_rectOrigin = rectOrigin;
}

void CObjRectBase::SetDynamicSize( const CRect& rectArea ) {
	m_rectArea = rectArea;
	m_fDrawAll = TRUE;
}

bool CObjRectBase::CheckSelf( void ) {
	POSITION poDL = m_listDynLink.GetHeadPosition();
	INT_PTR i, iCount = m_listDynLink.GetCount();
	CObjectDynLink * pc;
	CString str;

	for( i = 0; i < iCount; i++ ) {
		pc = m_listDynLink.GetNext( poDL );
		switch ( pc->GetUnitDynLinkType() ) {
		case tBOOL :
			break;
		}
	}
	return( TRUE );
}
