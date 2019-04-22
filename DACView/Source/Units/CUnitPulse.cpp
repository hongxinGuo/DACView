#include "stdafx.h"    

#include "..\\resource.h"
#include "CUnitBase.h"
#include "cUnitPulse.h" 
#include "DlgUnitPulse.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitPulse, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitPulse::sm_ptrParaName[] = 
  {{"Input", tINPUT |tMODIFIABLE | tBOOL, 0},
   {"Output", tOUTPUT |tMODIFIABLE | tBOOL,1},
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 2},
   {"Width", tINPUT |tMODIFIABLE | tWORD, 3},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 4},
   {"",         0, 5},
   }; 
   
INT32 CUnitPulse::sm_aulSuitable[] = { 0, 0, 0, 0, 0 };
   
const ULONG CUnitPulse::sm_ulDoubleEnd = 0;
const ULONG CUnitPulse::sm_ulBoolEnd   = 2; 
const ULONG CUnitPulse::sm_ulWordEnd   = 4;
const ULONG CUnitPulse::sm_ulStringEnd = 4;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitPulse::CUnitPulse(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 15;
  m_rectArea.right  = pt.x + 15;
  m_rectArea.top    = pt.y - 15;
  m_rectArea.bottom = pt.y + 15;

  // initial parameter
  m_ulWidth = 0;
	m_fInput = m_fOutput = false;
	m_fInputed = false;
	m_fReady = false;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitPulse::CUnitPulse( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;

  // initial parameter
  m_ulWidth = 0;
  m_fInput = m_fOutput = false;
	m_fInputed = false;
	m_fReady = false;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitPulse::~CUnitPulse() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitPulse::Serialize( CArchive& ar ) {
  
  CUnitBase::Serialize( ar );
  if( ar.IsStoring() ) {
    ar << m_ulWidth;
  }
  else {
    ar >> m_ulWidth;
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitPulse::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 5));
  return(m_vfSelected[ulIndex]);
}

void CUnitPulse::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 5; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitPulse::GetClassNameStr( void ) {
  static CString str = "Pulse";
  return(str);
}

CString CUnitPulse::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitPulse::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

/*
	generate a high level pulse.
	high level input set.
*/
void CUnitPulse::Exective( void ) {
  if ( m_fAutoExective ) {
    if ( (m_fInputed == false) && m_fReady ) {
      if ( m_fInput == true ) {
        m_fOutput = true;
        m_fInputed = true;
        m_ulWidthCount = m_ulWidth;
      }
    }
    else {
      if ( --m_ulWidthCount == 0 ) {
      	m_fOutput = false;
				m_fReady = false;
      	m_fInputed = false;
			}
    } 
		if ( (m_fInput == false) && ( !m_fInputed )  ) {
			m_fReady = true;
		}
  }
  
  ExectiveDynLink();
}

INT32 CUnitPulse::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitPulse::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitPulse::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitPulse::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitPulse::GetBool(ULONG index) {
  ASSERT( index <= sm_ulBoolEnd );
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

bool CUnitPulse::SetBool(ULONG index, bool fValue) {
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
         
LONG CUnitPulse::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 3 :
    return ( (LONG)m_ulWidth );
  case 4 :
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  return(0);
}

bool CUnitPulse::SetInteger(ULONG index, LONG lValue) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 3 :              
    m_ulWidth = lValue;
    break;
  case 4 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

void CUnitPulse::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_PULSE ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitPulse::SetProperty( void ) {
  CDlgPulse CDlg;
  
  CDlg.SetData(m_strName, m_ulWidth, m_lScanRate, m_cstrComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_ulWidth, m_lScanRate, m_cstrComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitPulse diagnostics

#ifdef _DEBUG
void CUnitPulse::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitPulse::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

