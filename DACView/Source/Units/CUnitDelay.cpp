#include "stdafx.h"    

#include "..\\resource.h"
#include "CUnitBase.h"
#include "cUnitDelay.h" 
#include "DlgUnitDelay.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitDelay, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitDelay::sm_ptrParaName[] = 
  {{"Input", tINPUT |  tMODIFIABLE | tBOOL, 0},
   {"Output", tOUTPUT | tMODIFIABLE | tBOOL, 1},
   {"AutoManual",tINPUT | tMODIFIABLE | tBOOL, 2},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 3},
   {"",         0, 4},
   }; 
   
INT32 CUnitDelay::sm_aulSuitable[] = { 0, 0, 0, 0 };
   
const ULONG CUnitDelay::sm_ulDoubleEnd = 0;
const ULONG CUnitDelay::sm_ulBoolEnd = 2;
const ULONG CUnitDelay::sm_ulWordEnd   = 3;
const ULONG CUnitDelay::sm_ulStringEnd = 3;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitDelay::CUnitDelay(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 15;
  m_rectArea.right  = pt.x + 15;
  m_rectArea.top    = pt.y - 15;
  m_rectArea.bottom = pt.y + 15;
  
  // initial parameter
  m_lDelayOn = 1;
  m_lDelayOff = 1;
	m_fInput = m_fOutput = false;
  m_fInputBefore = false;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitDelay::CUnitDelay( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;

  // initial parameter
  m_lDelayOn = 1;
  m_lDelayOff = 1;
  m_fInput = m_fOutput = false;
  m_fInputBefore = false;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitDelay::~CUnitDelay() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitDelay::Serialize( CArchive& ar ) {
  CUnitBase::Serialize( ar );

	INT64 a, b;
  if( ar.IsStoring() ) {
    ar << (INT64)m_lDelayOn << (INT64)m_lDelayOff;
  }
  else {
		ar >> a >> b;
		m_lDelayOn = a;
		m_lDelayOff = b;
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitDelay::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 4));
  return(m_vfSelected[ulIndex]);
}

void CUnitDelay::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 4; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitDelay::GetClassNameStr( void ) {
  static CString str = "UnitDelay";
  return(str);
}

CString CUnitDelay::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitDelay::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitDelay::Exective( void ) {
  if ( m_fAutoExective ) {
    if ( m_fInputBefore == m_fInput ) {
      if ( m_fInput == true ) {
        if ( m_fCountDelayOn == true ) {
          if ( --m_lCountDelayOn == 0 ) {
            m_fOutput = false;
            m_fCountDelayOn = false;
          }
        }
      }
      else {
        if ( m_fCountDelayOff ) {
          if ( --m_lCountDelayOff == 0 ) {
            m_fOutput = true;
            m_fCountDelayOff = false;
          }
        }
      }
    }
    else {
      if ( m_fInput == false ) {
        m_fCountDelayOff = true;
        m_lCountDelayOff = m_lDelayOff;
      }
      else {
        m_fCountDelayOn = true;
        m_lCountDelayOn = m_lDelayOff;
      }
    }
		m_fInputBefore = m_fInput;
  }    

  ExectiveDynLink();
}

INT32 CUnitDelay::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitDelay::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitDelay::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitDelay::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitDelay::GetBool(ULONG index) {
  ASSERT((index <= sm_ulBoolEnd));
  switch ( index ) {
  case 0: 
    return ( m_fInput );
  case 1 :              
    return ( m_fOutput );
  case 2 :              
    return ( m_fAutoExective );
  default :
    break;
  };
  return(0);
}

bool CUnitDelay::SetBool(ULONG index, bool fValue) {
  ASSERT(index <= sm_ulBoolEnd);
  switch ( index ) {
  case 0: 
    m_fInput = fValue;
    break;
  case 1 :              
    m_fOutput = fValue;
    break;
  case 2 :
    m_fAutoExective = fValue;
    break;
  default :
    return( false );
  }
  return( true );
}
         
LONG CUnitDelay::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && ( index > sm_ulBoolEnd ) );
  switch ( index ) {
  case 3 :               
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  return(0);
}

bool CUnitDelay::SetInteger(ULONG index, LONG lValue) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 3 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

void CUnitDelay::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_DELAY ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitDelay::SetProperty( void ) {
  CDlgDlay CDlg;
  
  CDlg.SetData(m_strName, m_lDelayOn, m_lDelayOff, m_lScanRate, m_cstrComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_lDelayOn, m_lDelayOff, m_lScanRate, m_cstrComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitDelay diagnostics

#ifdef _DEBUG
void CUnitDelay::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitDelay::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

